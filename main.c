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
  float *img, max_value = 0.0;
  nn_network_t net;
  size_t i, max_idx;
  block_t *b;
  struct timespec ts;
  uint64_t start, end;

  if (2 > argc)
    {
      fprintf (stderr, "usage: %s <path_to_image>\n", argv[0]);
      return -1;
    }

  img = malloc (784 * sizeof (float));
  if (-1 == image_load (img, 28, 28, argv[1]))
    {
      fprintf (stderr, "could not load image\n");
      free (img);
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

  memcpy (net.buffer.data, img, 784 * sizeof (float));

  // instr_summary (net.instrs.len, net.instrs.data);
  clock_gettime (CLOCK_MONOTONIC, &ts);
  start = ts_ns (ts);
  instr_forward_seq (net.buffer.data, net.instrs.len, net.instrs.data,
                     net.blocks.data);
  clock_gettime (CLOCK_MONOTONIC, &ts);
  end = ts_ns (ts);

  b = vec_get (&net.blocks, net.blocks.len - 1);
  for (i = 0; i < 10; i++)
    {
      if (net.buffer.data[b->offset + i] > max_value)
        {
          max_idx = i;
          max_value = net.buffer.data[b->offset + i];
        }
    }

  fprintf (stdout, "prediction: %ld (took %.3f ms)\n", max_idx,
           (double)(end - start) / 1e6);

  nn_network_free (&net);
  free (img);
}
