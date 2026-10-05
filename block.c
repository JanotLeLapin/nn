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

static inline int
digit_count (int v)
{
  return v == 0 ? 0 : (int)floorf (log10f (v));
}

void
block_print (const float *buf, const block_t *block)
{
  size_t i, j;

  int row_margin = digit_count (block->dims[1]);
  int max_cols = block->dims[1] > 100 ? 100 : block->dims[1];
  float v;

  fprintf (stderr, "  ");
  for (i = 0; i < max_cols; i++)
    {
      print_spaces (stderr, 4 - digit_count (i));
      fprintf (stderr, " %ld", i);
    }

  fprintf (stderr, "\n");
  for (i = 0; i < block->dims[0]; i++)
    {
      fprintf (stderr, " %ld ", i);
      print_spaces (stderr, row_margin - digit_count (i));
      for (j = 0; j < max_cols; j++)
        {
          v = buf[block->offset + i * block->dims[1] + j];
          if (v > 0.0)
            {
              fprintf (stderr, " ");
            }
          fprintf (stderr, " %.2f", v);
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
block_randomize (float *buf, const block_t *block, int seed)
{
  size_t i;
  uint32_t state = seed;

  for (i = 0; i < block->dims[0] * block->dims[1]; i++)
    {
      buf[block->offset + i]
          = (float)xorshift32 (&state) / ((float)UINT32_MAX / 2.0) - 1.0;
    }
}

int
block_load (float *buf, const block_t *block, const char *path)
{
  FILE *f;
  size_t n = block->dims[0] * block->dims[1];

  f = fopen (path, "rb");
  if (0 == f)
    {
      return -1;
    }

  if (0 == fread (&buf[block->offset], sizeof (float), n, f))
    {
      fclose (f);
      return -1;
    };

  fclose (f);
  return 0;
}

int
block_save (const float *buf, const block_t *block, const char *path)
{
  FILE *f;
  size_t n = block->dims[0] * block->dims[1];

  f = fopen (path, "wb");
  if (0 == f)
    {
      return -1;
    }

  if (0 == fwrite (&buf[block->offset], sizeof (float), n, f))
    {
      fclose (f);
      return -1;
    }

  fclose (f);
  return 0;
}
