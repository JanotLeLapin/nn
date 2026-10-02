#include "block.h"
#include <stdio.h>

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

void
block_print (const block_t *block)
{
  size_t i, j;

  fprintf (stderr, "  ");
  for (i = 0; i < block->dims[1]; i++)
    {
      fprintf (stderr, "    %ld", i);
    }

  fprintf (stderr, "\n");
  for (i = 0; i < block->dims[0]; i++)
    {
      fprintf (stderr, " %ld ", i);
      for (j = 0; j < block->dims[1]; j++)
        {
          fprintf (stderr, " %.2f", block->data[i * block->dims[0] + j]);
        }

      fprintf (stderr, "\n");
    }
}
