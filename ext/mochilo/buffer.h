#ifndef INCLUDE_buffer_h__
#define INCLUDE_buffer_h__

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>

#define unlikely(x)     __builtin_expect((x),0)

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
	char *end;
} mochilo_buf_chunk;

typedef struct {
	mochilo_buf_chunk *chunks;
	char *last_alloc;
	size_t total_size;
	uint32_t chunk_count, cur_chunk;
} mochilo_buf;

typedef struct {
	const char *ptr;
	const char *end;
	int trusted;
} mochilo_src;

void mochilo_buf_init(mochilo_buf *buf);
VALUE mochilo_buf_flush(mochilo_buf *buf);

mochilo_buf_chunk *mochilo_buf_rechunk(mochilo_buf *buf);
mochilo_buf_chunk *mochilo_buf_rechunk2(mochilo_buf *buf, size_t chunk_size);

void mochilo_buf_put(mochilo_buf *buf, const char *data, size_t len);

const char *mochilo_src_peek(mochilo_src *buf, size_t need);

#define BUF_ENSURE_AVAIL(b, d) \
	mochilo_buf_chunk *chunk = &b->chunks[b->cur_chunk]; \
	if (unlikely(chunk->ptr + (d) > chunk->end)) { \
		chunk = mochilo_buf_rechunk(b); };

#define SRC_CHECK_AVAIL(src, bytes) (src->ptr + bytes <= src->end) 

#define SRC_ENSURE_AVAIL(src, bytes) \
	if (unlikely(src->ptr + bytes > src->end)) \
		return -1;

static inline void mochilo_buf_putc(mochilo_buf *buf, uint8_t c)
{
	BUF_ENSURE_AVAIL(buf, 1);
	*chunk->ptr = c;
	chunk->ptr++;
}

static inline void mochilo_buf_put16be(mochilo_buf *buf, void *src16)
{
	BUF_ENSURE_AVAIL(buf, 2);
	swap16(src16, chunk->ptr);
	chunk->ptr += 2;
}

static inline void mochilo_buf_put32be(mochilo_buf *buf, void *src32)
{
	BUF_ENSURE_AVAIL(buf, 4);
	swap32(src32, chunk->ptr);
	chunk->ptr += 4;
}

static inline void mochilo_buf_put64be(mochilo_buf *buf, void *src64)
{
	BUF_ENSURE_AVAIL(buf, 8);
	swap64(src64, chunk->ptr);
	chunk->ptr += 8;
}



static inline void mochilo_src_get8be(mochilo_src *buf, uint8_t *dst8)
{
	*dst8 = *buf->ptr;
	buf->ptr += 1;
}

static inline void mochilo_src_get16be(mochilo_src *buf, void *dst16)
{
	swap16(buf->ptr, dst16);
	buf->ptr += 2;
}

static inline void mochilo_src_get32be(mochilo_src *buf, void *dst32)
{
	swap32(buf->ptr, dst32);
	buf->ptr += 4;
}

static inline void mochilo_src_get64be(mochilo_src *buf, void *dst64)
{
	swap64(buf->ptr, dst64);
	buf->ptr += 8;
}

#endif
