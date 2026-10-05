#include "vec.h"
#include <stdlib.h>
#include <string.h>

int
vec_alloc (vec_t *v, size_t elem_size, size_t capacity)
{
  v->data = malloc (elem_size * capacity);
  if (0 == v->data)
    {
      return -1;
    }
  v->elem_size = elem_size;
  v->capacity = capacity;
  v->len = 0;

  return 0;
}

int
vec_append (vec_t *v, const void *value, size_t count)
{
  size_t newcapacity;
  void *newdata;

  if (v->len + count >= v->capacity)
    {
      newcapacity = 2 * v->capacity;
      newdata = realloc (v->data, newcapacity);
      if (0 == newdata)
        {
          return -1;
        }

      v->data = newdata;
      v->capacity = newcapacity;
    }

  memcpy (v->data + v->elem_size * v->len, value, v->elem_size * count);
  v->len += count;

  return 0;
}

void
vec_free (vec_t *v)
{
  free (v->data);
  v->elem_size = 0;
  v->len = 0;
  v->capacity = 0;
}
