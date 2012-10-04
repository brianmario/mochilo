#ifndef __INTERN_H__
#define __INTERN_H__

static inline void swap8(const uint8_t *buffer, void *out)
{
    unsigned char *ptr = (unsigned char *)out;
  *ptr = *buffer;
}

static inline void swap16(const uint8_t *buffer, void *out)
{
    unsigned char *ptr = (unsigned char *)out;
    ptr[0] = buffer[1];
    ptr[1] = buffer[0];
}

static inline void swap32(const uint8_t *buffer, void *out)
{
    unsigned char *ptr = (unsigned char *)out;
    ptr[0] = buffer[3];
    ptr[1] = buffer[2];
    ptr[2] = buffer[1];
    ptr[3] = buffer[0];
}

static inline void swap64(const uint8_t *buffer, void *out)
{
  unsigned char *ptr = (unsigned char *)out;
  ptr[0] = buffer[7];
  ptr[1] = buffer[6];
  ptr[2] = buffer[5];
  ptr[3] = buffer[4];
  ptr[4] = buffer[3];
  ptr[5] = buffer[2];
  ptr[6] = buffer[1];
  ptr[7] = buffer[0];
}

#endif
