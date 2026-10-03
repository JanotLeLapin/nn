#ifndef _NN_H
#define _NN_H

#include "block.h"
#include "instruction.h"

typedef struct
{
  instr_t *instrs;
  block_t *stack;
  int instr_head;
  int stack_head;
} nn_network_t;

int nn_network_alloc (nn_network_t *net, int instr_count, int stack_size);
void nn_network_free (nn_network_t *net);

int nn_layer_input (nn_network_t *net, int input_dim);
int nn_layer_dense (nn_network_t *net, int input_dim, int output_dim,
                    const float *w, const float *b);

#endif
