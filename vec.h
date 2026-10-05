#ifndef _NN_VEC_H
#define _NN_VEC_H

#include <stddef.h>

typedef struct
{
  void *data;
  size_t elem_size, len, capacity;
} vec_t;

int vec_alloc (vec_t *v, size_t elem_size, size_t capacity);
int vec_append (vec_t *v, const void *value, size_t count);
void vec_free (vec_t *v);

static inline void *
vec_get (const vec_t *v, size_t idx)
{
  return v->data + v->elem_size * idx;
}

#endif
