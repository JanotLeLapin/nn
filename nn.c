#include "nn.h"
#include "block.h"
#include "instruction.h"
#include <string.h>

int
nn_network_alloc (nn_network_t *net, int instr_count, int stack_size)
{
  net->instrs = malloc (instr_count * sizeof (instr_t));
  if (0 == net->instrs)
    {
      return -1;
    }
  net->stack = malloc (stack_size * sizeof (block_t));
  if (0 == net->stack)
    {
      free (net->instrs);
      return -1;
    }

  net->instr_head = 0;
  net->stack_head = 0;

  return 0;
}

void
nn_network_free (nn_network_t *net)
{
  size_t i;

  free (net->instrs);
  net->instrs = 0;

  for (i = 0; i <= net->stack_head; i++)
    {
      block_free (&net->stack[i]);
    }
  free (net->stack);
  net->stack = 0;

  net->instr_head = 0;
  net->stack_head = 0;
}

int
nn_layer_input (nn_network_t *net, int input_dim)
{
  block_t *b = net->stack;

  if (0 < net->stack_head)
    {
      return -1;
    }

  *b = block_alloc (1, input_dim);
  if (0 == b->data)
    {
      return -1;
    }
  memset (b->data, 0, input_dim * sizeof (float));

  return 0;
}

int
nn_layer_dense (nn_network_t *net, int input_dim, int output_dim,
                const float *w, const float *b, act_type_t activation)
{
  instr_t *instr = &net->instrs[net->instr_head];
  int sp = net->stack_head;
  block_t *wb = &net->stack[sp + 1], *bb = &net->stack[sp + 2],
          *rb = &net->stack[sp + 3];

  instr->t = INSTR_TYPE_GEMM;
  instr->dst = sp + 3;
  instr->src = sp;

  *wb = block_alloc (input_dim, output_dim);
  if (0 == wb->data)
    {
      return -1;
    }
  if (0 != w)
    {
      memcpy (wb->data, w, input_dim * output_dim * sizeof (float));
    }

  *bb = block_alloc (1, output_dim);
  if (0 == bb->data)
    {
      block_free (wb);
      return -1;
    }
  if (0 != b)
    {
      memcpy (bb->data, b, output_dim * sizeof (float));
    }

  *rb = block_alloc (1, output_dim);
  if (0 == rb->data)
    {
      block_free (wb);
      block_free (bb);
      return -1;
    }
  memset (rb->data, 0, output_dim * sizeof (float));

  net->instr_head += 1;
  net->stack_head += 3;

  return 0;
}
