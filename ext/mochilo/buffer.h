#ifndef INCLUDE_buffer_h__
#define INCLUDE_buffer_h__

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>

/**
 * Byteswap code
 */
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

/**
 * Buffer code 
 */
typedef struct {
	char *ptr;
	size_t asize, size;
	int (*flush)(const char *, size_t, void *);
	void *opaque;
} mochilo_buf;

void mochilo_buf_init(mochilo_buf *buf,
	size_t buffer_size,
	int (*flush)(const char *data, size_t len, void *opaque),
	void *opaque);

void mochilo_buf_free(mochilo_buf *buf);
int mochilo_buf_put(mochilo_buf *buf, const char *data, size_t len);
int mochilo_buf_flush(mochilo_buf *buf);

#define BUF_ENSURE_SIZE(b, d) \
  if ((d) > buf->asize && mochilo_buf_flush(b) < 0)\
    return -1;

static inline int mochilo_buf_putc(mochilo_buf *buf, uint8_t c)
{
	BUF_ENSURE_SIZE(buf, buf->size + 1);
	buf->ptr[buf->size++] = c;
	return 0;
}

static inline int mochilo_buf_put16be(mochilo_buf *buf, void *src16)
{
	BUF_ENSURE_SIZE(buf, buf->size + 2);
	swap16(src16, buf->ptr + buf->size);
	buf->size += 2;
	return 0;
}

static inline int mochilo_buf_put32be(mochilo_buf *buf, void *src32)
{
	BUF_ENSURE_SIZE(buf, buf->size + 4);
	swap32(src32, buf->ptr + buf->size);
	buf->size += 4;
	return 0;
}

static inline int mochilo_buf_put64be(mochilo_buf *buf, void *src64)
{
	BUF_ENSURE_SIZE(buf, buf->size + 8);
	swap64(src64, buf->ptr + buf->size);
	buf->size += 8;
	return 0;
}

#endif
