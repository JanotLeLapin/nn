#include "image.h"
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

int
image_load (float *buf, int width, int height, const char *path)
{
  int x, y, channels;
  stbi_uc *uc;
  float sx, sy;
  size_t i, j;

  uc = stbi_load (path, &x, &y, &channels, 1);
  if (0 == uc)
    {
      return -1;
    }

  sx = (float)x / (float)width;
  sy = (float)y / (float)height;

  for (i = 0; i < width; i++)
    {
      for (j = 0; j < height; j++)
        {
          buf[j * width + i] = (float)uc[(int)(floorf ((float)j * sy)) * x
                                         + (int)(floorf ((float)i * sx))]
                               / 255.0;
        }
    }

  stbi_image_free (uc);

  return 0;
}
