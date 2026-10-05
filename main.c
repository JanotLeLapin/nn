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

  nn_layer_input (&net, 16);
  nn_layer_dense (&net, 16, 12, 0, 0, ACT_TYPE_RELU);
  nn_layer_dense (&net, 12, 12, 0, 0, ACT_TYPE_NONE);

  b = vec_get (&net.blocks, 0);
  for (i = 0; i < 16; i++)
    {
      net.buffer.data[b->offset + i] = (float)i;
    }

  block_randomize (net.buffer.data, vec_get (&net.blocks, 1), 4);
  block_randomize (net.buffer.data, vec_get (&net.blocks, 2), 3);
  block_randomize (net.buffer.data, vec_get (&net.blocks, 4), 412);
  block_randomize (net.buffer.data, vec_get (&net.blocks, 5), 77);

  instr_summary (net.instrs.len, net.instrs.data);

  instr_forward_seq (net.buffer.data, net.instrs.len, net.instrs.data,
                     net.blocks.data);

  fprintf (stderr, "res:\n");
  block_print (net.buffer.data, vec_get (&net.blocks, net.blocks.len - 1));

  nn_network_free (&net);
}
