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
