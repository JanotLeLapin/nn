#include <stdio.h>
#include <string.h>

#include "block.h"
#include "instruction.h"
#include "nn.h"
#include "vec.h"

int
main ()
{
  nn_network_t net;
  size_t i;
  block_t *b;

  nn_network_alloc (&net, 8, 32);

  nn_layer_input (&net, 784);
  nn_layer_dense (&net, 784, 128, 0, 0, ACT_TYPE_RELU);
  nn_layer_dense (&net, 128, 64, 0, 0, ACT_TYPE_RELU);
  nn_layer_dense (&net, 64, 10, 0, 0, ACT_TYPE_SOFTMAX);

  block_load (net.buffer.data, vec_get (&net.blocks, 1),
              "model/layer1_weights.bin");
  block_load (net.buffer.data, vec_get (&net.blocks, 2),
              "model/layer1_biases.bin");
  block_load (net.buffer.data, vec_get (&net.blocks, 4),
              "model/layer2_weights.bin");
  block_load (net.buffer.data, vec_get (&net.blocks, 5),
              "model/layer2_biases.bin");
  block_load (net.buffer.data, vec_get (&net.blocks, 9),
              "model/layer3_weights.bin");
  block_load (net.buffer.data, vec_get (&net.blocks, 10),
              "model/layer3_biases.bin");

  b = vec_get (&net.blocks, 0);
  for (i = 0; i < 16; i++)
    {
      net.buffer.data[b->offset + i] = (float)i;
    }

  instr_summary (net.instrs.len, net.instrs.data);
  instr_forward_seq (net.buffer.data, net.instrs.len, net.instrs.data,
                     net.blocks.data);

  fprintf (stderr, "res:\n");
  block_print (net.buffer.data, vec_get (&net.blocks, net.blocks.len - 1));

  nn_network_free (&net);
}
