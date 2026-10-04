#include <stdio.h>
#include <string.h>

#include "block.h"
#include "instruction.h"
#include "nn.h"

int
main ()
{
  nn_network_t net;
  size_t i;

  nn_network_alloc (&net, 8, 32);

  nn_layer_input (&net, 16);
  nn_layer_dense (&net, 16, 12, 0, 0, ACT_TYPE_RELU);
  nn_layer_dense (&net, 12, 12, 0, 0, ACT_TYPE_NONE);

  for (i = 0; i < 16; i++)
    {
      net.stack[0].data[i] = (float)i;
    }

  block_randomize (&net.stack[1], 4);
  block_randomize (&net.stack[2], 3);
  block_randomize (&net.stack[4], 412);
  block_randomize (&net.stack[5], 77);

  instr_forward_seq (net.instr_head, net.instrs, net.stack);

  fprintf (stderr, "res:\n");
  block_print (&net.stack[net.stack_head]);

  nn_network_free (&net);
}
