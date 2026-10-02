#include "block.h"

block_t
block_alloc (unsigned int width, unsigned int height)
{
  float *data = (float *)malloc (width * height * sizeof (float));
  if (0 == data)
    {
      return (block_t){ .data = 0, .dims = { 0, 0 } };
    }

  return (block_t){ .data = data, .dims = { width, height } };
}

void
block_free (block_t *block)
{
  size_t i;

  free (block->data);
  block->data = 0;

  for (i = 0; i < 2; i++)
    {
      block->dims[i] = 0;
    }
}
