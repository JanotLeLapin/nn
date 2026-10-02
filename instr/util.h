#define ELEMENT_WISE(dst, src, func)                                          \
  {                                                                           \
    size_t i;                                                                 \
    for (i = 0; i < src->dims[0] * src->dims[1]; i++)                         \
      {                                                                       \
        dst->data[i] = func (src->data[i]);                                   \
      }                                                                       \
  }
