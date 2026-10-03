#include "block.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>

static inline void
print_spaces (FILE *file, size_t count)
{
  size_t i;

  for (i = 0; i < count; i++)
    {
      fprintf (file, " ");
    }
}

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

static inline int
digit_count (int v)
{
  return v == 0 ? 0 : (int)floorf (log10f (v));
}

void
block_print (const block_t *block)
{
  size_t i, j;

  int row_margin = digit_count (block->dims[1]);
  int max_cols = block->dims[1] > 100 ? 100 : block->dims[1];

  fprintf (stderr, "  ");
  for (i = 0; i < max_cols; i++)
    {
      print_spaces (stderr, 4 - digit_count (i));
      fprintf (stderr, "%ld", i);
    }

  fprintf (stderr, "\n");
  for (i = 0; i < block->dims[0]; i++)
    {
      fprintf (stderr, " %ld ", i);
      print_spaces (stderr, row_margin - digit_count (i));
      for (j = 0; j < max_cols; j++)
        {
          fprintf (stderr, " %.2f", block->data[i * block->dims[1] + j]);
        }

      fprintf (stderr, "\n");
    }
}

static inline uint32_t
xorshift32 (uint32_t *state)
{
  uint32_t x = *state;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  return *state = x;
}

void
block_randomize (block_t *block, int seed)
{
  size_t i;
  uint32_t state = seed;

  for (i = 0; i < block->dims[0] * block->dims[1]; i++)
    {
      block->data[i]
          = (float)xorshift32 (&state) / ((float)UINT32_MAX / 2.0) - 1.0;
    }
}
