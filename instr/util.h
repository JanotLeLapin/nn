#define ELEMENT_WISE(buf, dst, src, func)                                     \
  {                                                                           \
    size_t i;                                                                 \
    for (i = 0; i < src->dims[0] * src->dims[1]; i++)                         \
      {                                                                       \
        buf[dst->offset + i] = func (buf[src->offset + i]);                   \
      }                                                                       \
  }
