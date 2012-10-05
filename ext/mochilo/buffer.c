/*
 * Copyright (C) 2012 GitHub, Inc
 */
#include <stdarg.h>
#include <ctype.h>
#include "buffer.h"

void mochilo_buf_init(mochilo_buf *buf,
	size_t buffer_size,
	int (*flush)(const char *data, size_t len, void *opaque),
	void *opaque)
{
	if (buffer_size < 32)
		buffer_size = 32;

	buf->asize = buffer_size;
	buf->ptr = malloc(buffer_size);

	buf->size = 0;
	buf->flush = flush;
	buf->opaque = opaque;
}

void mochilo_buf_free(mochilo_buf *buf)
{
	if (!buf) return;
	free(buf->ptr);
}

int mochilo_buf_put(mochilo_buf *buf, const char *data, size_t len)
{
	if (buf->size + len > buf->asize) {
		if (mochilo_buf_flush(buf) < 0)
			return -1;

		while (len >= buf->asize) {
			if (buf->flush(data, len, buf->opaque) < 0)
				return -1;

			len -= buf->asize;
		}
	}

	memmove(buf->ptr + buf->size, data, len);
	buf->size += len;
	return 0;
}

int mochilo_buf_flush(mochilo_buf *buf)
{
	if (buf->size && buf->flush(buf->ptr, buf->size, buf->opaque) < 0)
		return -1;

	buf->size = 0;
	return 0;
}

int mochilo_src_refill(mochilo_src *buf, size_t need)
{
	if (!buf->alloc)
		return -1;

	memmove(buf->ptr, buf->ptr + buf->pos, buf->avail - buf->pos);

	buf->pos = 0;
	buf->avail = 0;

	if (need > buf->alloc)
		need = buf->alloc;

	while (buf->avail < need) {
		int r = buf->refill(buf->ptr + buf->avail, buf->alloc - buf->avail, buf->opaque);
		if (r < 0)
			return -1;

		buf->avail += r;
	}

	return 0;
}

int mochilo_src_read(mochilo_src *buf, char *out, size_t need)
{
	if (buf->pos + need > buf->avail) {
		int written = buf->avail - buf->pos;

		if (!buf->alloc)
			return -1;

		if (!written && mochilo_src_refill(buf, need) < 0)
			return -1;

		written = buf->avail - buf->pos;
		memcpy(out, buf->ptr + buf->pos, written);

		buf->pos = 0;
		buf->avail = 0;
		return written;
	}

	memcpy(out, buf->ptr + buf->pos, need);
	buf->pos += need;
	return need;
}

void mochilo_src_init_static(mochilo_src *buf, uint8_t *data, size_t len)
{
	buf->ptr = data;
	buf->pos = 0;
	buf->avail = len;
	buf->alloc = 0;
}

void mochilo_src_free(mochilo_src *buf)
{
	if (buf && buf->alloc)
		free(buf->ptr);
}
