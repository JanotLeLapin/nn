#ifndef _NN_H
#define _NN_H

#include "block.h"
#include "instruction.h"

#define ACT_TYPE(X) X (RELU)

typedef struct
{
  instr_t *instrs;
  block_t *stack;
  int instr_head;
  int stack_head;
} nn_network_t;

typedef enum
{
  ACT_TYPE_NONE,
#define X(variant) ACT_TYPE_##variant,
  ACT_TYPE (X)
#undef X
} act_type_t;

int nn_network_alloc (nn_network_t *net, int instr_count, int stack_size);
void nn_network_free (nn_network_t *net);

int nn_layer_input (nn_network_t *net, int input_dim);
int nn_layer_dense (nn_network_t *net, int input_dim, int output_dim,
                    const float *w, const float *b, act_type_t activation);

#endif
