#include "nn.h"
#include "block.h"
#include "instruction.h"
#include <stdlib.h>
#include <string.h>

int
buffer_alloc (buffer_t *b, size_t capacity)
{
  b->data = malloc (capacity * sizeof (float));
  if (0 == b->data)
    {
      return -1;
    }

  b->length = 0;
  b->capacity = capacity;

  return 0;
}

int
buffer_resize (buffer_t *b, size_t newcapacity)
{
  float *newdata;

  if (newcapacity <= b->capacity)
    {
      return 0;
    }

  newdata = realloc (b->data, newcapacity * sizeof (float));
  if (0 == newdata)
    {
      return -1;
    }
  b->data = newdata;
  b->capacity = newcapacity;

  return 0;
}

void
buffer_free (buffer_t *b)
{
  free (b->data);
  b->data = 0;
  b->length = 0;
  b->capacity = 0;
}

int
nn_network_alloc (nn_network_t *net, int instr_count, int stack_size)
{
  if (-1 == buffer_alloc (&net->buffer, 64))
    {
      return -1;
    }

  net->instrs = malloc (instr_count * sizeof (instr_t));
  if (0 == net->instrs)
    {
      buffer_free (&net->buffer);
      return -1;
    }
  net->stack = malloc (stack_size * sizeof (block_t));
  if (0 == net->stack)
    {
      buffer_free (&net->buffer);
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
  buffer_free (&net->buffer);
  free (net->instrs);
  free (net->stack);
  net->instrs = 0;
  net->stack = 0;
  net->instr_head = 0;
  net->stack_head = 0;
}

int
nn_layer_input (nn_network_t *net, int input_dim)
{
  float *buffer_head;
  block_t *b = net->stack;

  if (0 < net->stack_head)
    {
      return -1;
    }

  if (-1 == buffer_resize (&net->buffer, net->buffer.capacity + input_dim))
    {
      return -1;
    }
  buffer_head = &net->buffer.data[net->buffer.length];
  net->buffer.length += input_dim;

  *b = (block_t){ .data = buffer_head, .dims = { 1, input_dim } };
  memset (b->data, 0, input_dim * sizeof (float));

  return 0;
}

int
nn_layer_dense (nn_network_t *net, int input_dim, int output_dim,
                const float *w, const float *b, act_type_t activation)
{
  float *buffer_head;
  int sp = net->stack_head;
  block_t *wb = &net->stack[sp + 1], *bb = &net->stack[sp + 2],
          *rb = &net->stack[sp + 3], *fb = &net->stack[sp + 4];
  instr_t *instr = &net->instrs[net->instr_head];

  if (-1
      == buffer_resize (&net->buffer, net->buffer.capacity
                                          + input_dim * output_dim
                                          + 3 * output_dim))
    {
      return -1;
    }
  buffer_head = &net->buffer.data[net->buffer.length];
  net->buffer.length += input_dim * output_dim + 3 * output_dim;

  *wb = (block_t){ .data = &buffer_head[0],
                   .dims = { input_dim, output_dim } };
  *bb = (block_t){ .data = &buffer_head[input_dim * output_dim],
                   .dims = { 1, output_dim } };
  *rb = (block_t){ .data = &buffer_head[input_dim * output_dim + output_dim],
                   .dims = { 1, output_dim } };
  *fb = (block_t){ .data
                   = &buffer_head[input_dim * output_dim + 2 * output_dim],
                   .dims = { 1, output_dim } };

  if (0 != w)
    {
      memcpy (wb->data, w, input_dim * output_dim * sizeof (float));
    }
  if (0 != b)
    {
      memcpy (bb->data, b, output_dim * sizeof (float));
    }

  memset (rb->data, 0, output_dim * sizeof (float));

  if (ACT_TYPE_NONE != activation)
    {
      memset (fb->data, 0, output_dim * sizeof (float));
      net->stack_head += 4;
    }
  else
    {
      net->stack_head += 3;
    }

  instr[0] = (instr_t){ .t = INSTR_TYPE_GEMM, .dst = sp + 3, .src = sp };

  switch (activation)
    {
    case ACT_TYPE_NONE:
      net->instr_head += 1;
      break;
#define X(variant)                                                            \
  case ACT_TYPE_##variant:                                                    \
    instr[1] = (instr_t){ .t = INSTR_TYPE_##variant,                          \
                          .dst = sp + 4,                                      \
                          .src = sp + 3 };                                    \
    net->instr_head += 2;                                                     \
    break;
      ACT_TYPE (X)
#undef X
    }

  return 0;
}
