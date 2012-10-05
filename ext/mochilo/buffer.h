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

typedef struct {
	char *ptr;
	size_t pos, avail, alloc;
	int (*refill)(char *, size_t, void *);
	void *opaque;
} mochilo_src;

void mochilo_buf_init(mochilo_buf *buf,
	size_t buffer_size,
	int (*flush)(const char *data, size_t len, void *opaque),
	void *opaque);

void mochilo_buf_free(mochilo_buf *buf);
int mochilo_buf_put(mochilo_buf *buf, const char *data, size_t len);
int mochilo_buf_flush(mochilo_buf *buf);

void mochilo_src_init_static(mochilo_src *buf, uint8_t *data, size_t len);

#define BUF_ENSURE_AVAIL(b, d) \
	if (b->size + (d) > b->asize && mochilo_buf_flush(b) < 0)\
		return;

#define SRC_ENSURE_AVAIL(src, bytes) \
	if (src->pos + bytes > src->avail && mochilo_src_refill(src, bytes) < 0) \
		return -1;

static inline void mochilo_buf_putc(mochilo_buf *buf, uint8_t c)
{
	BUF_ENSURE_AVAIL(buf, 1);
	buf->ptr[buf->size++] = c;
}

static inline void mochilo_buf_put16be(mochilo_buf *buf, void *src16)
{
	BUF_ENSURE_AVAIL(buf, 2);
	swap16(src16, buf->ptr + buf->size);
	buf->size += 2;
}

static inline void mochilo_buf_put32be(mochilo_buf *buf, void *src32)
{
	BUF_ENSURE_AVAIL(buf, 4);
	swap32(src32, buf->ptr + buf->size);
	buf->size += 4;
}

static inline void mochilo_buf_put64be(mochilo_buf *buf, void *src64)
{
	BUF_ENSURE_AVAIL(buf, 8);
	swap64(src64, buf->ptr + buf->size);
	buf->size += 8;
}

static inline void mochilo_src_get8be(mochilo_src *buf, uint8_t *dst8)
{
	*dst8 = buf->ptr[buf->pos++];
}

static inline void mochilo_src_get16be(mochilo_src *buf, void *dst16)
{
	swap16(buf->ptr + buf->pos, dst16);
	buf->pos += 2;
}

static inline void mochilo_src_get32be(mochilo_src *buf, void *dst32)
{
	swap32(buf->ptr + buf->pos, dst32);
	buf->pos += 4;
}

static inline void mochilo_src_get64be(mochilo_src *buf, void *dst64)
{
	swap64(buf->ptr + buf->pos, dst64);
	buf->pos += 8;
}

#endif
