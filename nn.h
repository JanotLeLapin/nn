#ifndef _NN_H
#define _NN_H

#include "vec.h"

#define ACT_TYPE(X)                                                           \
  X (RELU)                                                                    \
  X (SOFTMAX)

typedef struct
{
  float *data;
  size_t length, capacity;
} buffer_t;

typedef struct
{
  buffer_t buffer;
  vec_t instrs;
  vec_t blocks;
} nn_network_t;

typedef enum
{
  ACT_TYPE_NONE,
#define X(variant) ACT_TYPE_##variant,
  ACT_TYPE (X)
#undef X
} act_type_t;

int buffer_alloc (buffer_t *b, size_t capacity);
int buffer_resize (buffer_t *b, size_t newcapacity);
void buffer_free (buffer_t *b);

int nn_network_alloc (nn_network_t *net, int instr_count, int stack_size);
void nn_network_free (nn_network_t *net);

int nn_layer_input (nn_network_t *net, int input_dim);
int nn_layer_dense (nn_network_t *net, int input_dim, int output_dim,
                    const float *w, const float *b, act_type_t activation);

#endif
