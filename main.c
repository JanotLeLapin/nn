#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "block.h"
#include "image.h"
#include "instruction.h"
#include "nn.h"
#include "vec.h"

static inline uint64_t
ts_ns (struct timespec ts)
{
  return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
}

int
main (int argc, char **argv)
{
  float *img, max_value;
  nn_network_t net;
  size_t i, j, max_idx;
  block_t *b;
  instr_t instr;
  FILE *csv;
  struct timespec ts;
  uint64_t layer_start, layer_end;

  if (3 > argc)
    {
      fprintf (stderr, "usage: %s <path_to_csv> [path_to_images...]\n",
               argv[0]);
      return -1;
    }

  csv = fopen (argv[1], "wa");
  if (0 == csv)
    {
      fprintf (stderr, "could not open csv file\n");
      return -1;
    }

  img = malloc (784 * sizeof (float));
  if (0 == img)
    {
      fprintf (stderr, "could not alloc img buffer\n");
      fclose (csv);
      return -1;
    }

  nn_network_alloc (&net, 8, 32);

  nn_layer_input (&net, 784);
  nn_layer_dense (&net, 784, 128, 0, 0, ACT_TYPE_RELU);
  nn_layer_dense (&net, 128, 64, 0, 0, ACT_TYPE_RELU);
  nn_layer_dense (&net, 64, 10, 0, 0, ACT_TYPE_SOFTMAX);

  block_load (net.buffer.data, vec_get (&net.blocks, 1),
              "model/layer1_weights.bin");
  block_load (net.buffer.data, vec_get (&net.blocks, 2),
              "model/layer1_biases.bin");
  block_load (net.buffer.data, vec_get (&net.blocks, 5),
              "model/layer2_weights.bin");
  block_load (net.buffer.data, vec_get (&net.blocks, 6),
              "model/layer2_biases.bin");
  block_load (net.buffer.data, vec_get (&net.blocks, 9),
              "model/layer3_weights.bin");
  block_load (net.buffer.data, vec_get (&net.blocks, 10),
              "model/layer3_biases.bin");

  for (i = 0; i < net.instrs.len; i++)
    {
      fprintf (csv, "layer_%ld,", i + 1);
    }
  fprintf (csv, "\n");

  // instr_summary (net.instrs.len, net.instrs.data);

  for (i = 2; i < argc; i++)
    {
      if (-1 == image_load (img, 28, 28, argv[i]))
        {
          fprintf (stderr, "could not load image %s, skipping\n", argv[i]);
          continue;
        }
      memcpy (net.buffer.data, img, 784 * sizeof (float));

      for (j = 0; j < net.instrs.len; j++)
        {
          clock_gettime (CLOCK_MONOTONIC, &ts);
          layer_start = ts_ns (ts);
          instr = *(instr_t *)vec_get (&net.instrs, j);
          instr_forward (net.buffer.data, instr, net.blocks.data);
          clock_gettime (CLOCK_MONOTONIC, &ts);
          layer_end = ts_ns (ts);

          fprintf (csv, "%f,", (double)(layer_end - layer_start) / 1e6);

          fprintf (stderr, "%s: layer %ld took %.3fms\n", argv[i], j + 1,
                   (double)(layer_end - layer_start) / 1e6);
        }

      fprintf (csv, "\n");

      b = vec_get (&net.blocks, net.blocks.len - 1);
      max_idx = 0;
      max_value = 0.0;
      for (j = 0; j < 10; j++)
        {
          if (net.buffer.data[b->offset + j] > max_value)
            {
              max_idx = j;
              max_value = net.buffer.data[b->offset + j];
            }
        }

      fprintf (stdout, "%s: prediction: %ld\n", argv[i], max_idx);
    }

  nn_network_free (&net);
  fclose (csv);
  free (img);
}
