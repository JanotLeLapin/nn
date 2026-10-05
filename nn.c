#include "nn.h"
#include "block.h"
#include "instruction.h"
#include "vec.h"
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

  if (-1 == vec_alloc (&net->instrs, sizeof (instr_t), instr_count))
    {
      buffer_free (&net->buffer);
      return -1;
    }

  if (-1 == vec_alloc (&net->blocks, sizeof (block_t), stack_size))
    {
      buffer_free (&net->buffer);
      vec_free (&net->instrs);
      return -1;
    }

  return 0;
}

void
nn_network_free (nn_network_t *net)
{
  buffer_free (&net->buffer);
  vec_free (&net->instrs);
  vec_free (&net->blocks);
}

int
nn_layer_input (nn_network_t *net, int input_dim)
{
  block_t b;

  if (0 < net->instrs.len)
    {
      return -1;
    }

  if (-1 == buffer_resize (&net->buffer, net->buffer.capacity + input_dim))
    {
      return -1;
    }

  b = (block_t){ .offset = net->buffer.length, .dims = { 1, input_dim } };
  memset (&net->buffer.data[net->buffer.length], 0,
          input_dim * sizeof (float));
  net->buffer.length += input_dim;

  if (-1 == vec_append (&net->blocks, &b, 1))
    {
      return -1;
    }

  return 0;
}

int
nn_layer_dense (nn_network_t *net, int input_dim, int output_dim,
                const float *w, const float *b, act_type_t activation)
{
  block_t bs[4];
  instr_t instrs[2];
  size_t instr_count, block_count, stack_head = net->blocks.len - 1;

  if (-1
      == buffer_resize (&net->buffer, net->buffer.capacity
                                          + input_dim * output_dim
                                          + 3 * output_dim))
    {
      return -1;
    }

  bs[0] = (block_t){ .offset = net->buffer.length,
                     .dims = { input_dim, output_dim } };
  bs[1] = (block_t){ .offset = net->buffer.length + input_dim * output_dim,
                     .dims = { 1, output_dim } };
  bs[2] = (block_t){ .offset = net->buffer.length + input_dim * output_dim
                               + output_dim,
                     .dims = { 1, output_dim } };
  bs[3] = (block_t){ .offset = net->buffer.length + input_dim * output_dim
                               + 2 * output_dim,
                     .dims = { 1, output_dim } };
  net->buffer.length += input_dim * output_dim + 3 * output_dim;

  if (0 != w)
    {
      memcpy (&net->buffer.data[bs[0].offset], w,
              input_dim * output_dim * sizeof (float));
    }
  if (0 != b)
    {
      memcpy (&net->buffer.data[bs[1].offset], b, output_dim * sizeof (float));
    }

  memset (&net->buffer.data[bs[2].offset], 0, output_dim * sizeof (float));

  if (ACT_TYPE_NONE != activation)
    {
      memset (&net->buffer.data[bs[3].offset], 0, output_dim * sizeof (float));
      block_count = 4;
    }
  else
    {
      block_count = 3;
    }

  instrs[0] = (instr_t){ .t = INSTR_TYPE_GEMM,
                         .dst = stack_head + 3,
                         .src = stack_head };

  switch (activation)
    {
#define X(variant)                                                            \
  case ACT_TYPE_##variant:                                                    \
    instrs[1] = (instr_t){ .t = INSTR_TYPE_##variant,                         \
                           .dst = stack_head + 4,                             \
                           .src = stack_head + 3 };                           \
    instr_count = 2;                                                          \
    break;
      ACT_TYPE (X)
#undef X
    default:
      instr_count = 1;
      break;
    }

  if (-1 == vec_append (&net->blocks, bs, block_count))
    {
      return -1;
    }

  if (-1 == vec_append (&net->instrs, instrs, instr_count))
    {
      return -1;
    };

  return 0;
}
