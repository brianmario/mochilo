/*
 * Copyright (C) 2012 GitHub, Inc
 */
#include <stdarg.h>
#include <ctype.h>
#include "mochilo.h"

#define MOCHILO_CHUNK_SIZE 1024
#define MOCHILO_CHUNK_INIT 8

static mochilo_buf_chunk *init_cur_chunk(mochilo_buf *buf, size_t chunk_size)
{
	mochilo_buf_chunk *chunk = &buf->chunks[buf->cur_chunk];

	if (chunk_size <= MOCHILO_CHUNK_SIZE)
		chunk_size = MOCHILO_CHUNK_SIZE;
	else
		chunk_size += MOCHILO_CHUNK_SIZE;

	buf->last_alloc = chunk->ptr = malloc(chunk_size);
	if (!chunk->ptr)
		rb_raise(rb_eNoMemError, "Failed to allocate new chunk");

	chunk->end = chunk->ptr + chunk_size;
	return chunk;
}

static void skip_last_chunk(mochilo_buf *buf)
{
	mochilo_buf_chunk *chunk = &buf->chunks[buf->cur_chunk];

	buf->total_size += (chunk->ptr - buf->last_alloc);
	buf->cur_chunk++;

	chunk->end = chunk->ptr;
	chunk->ptr = buf->last_alloc;
}

static void free_buf(mochilo_buf *buf)
{
	uint32_t i;

	for (i = 0; i < buf->cur_chunk; ++i)
		free(buf->chunks[i].ptr);

	free(buf->chunks);
}


void mochilo_buf_init(mochilo_buf *buf)
{
	buf->chunks = malloc(MOCHILO_CHUNK_INIT * sizeof(mochilo_buf_chunk));
	buf->total_size = 0;
	buf->cur_chunk = 0;
	buf->chunk_count = MOCHILO_CHUNK_INIT;

	init_cur_chunk(buf, MOCHILO_CHUNK_SIZE);
}

VALUE mochilo_buf_flush(mochilo_buf *buf)
{
	VALUE rb_str;
	char *ptr;
	uint32_t i;

	skip_last_chunk(buf);

#ifdef RUBINIUS
	char *alloc;
	alloc = ptr = malloc(buf->total_size);
	if (alloc == NULL)
		rb_raise(rb_eNoMemError, "Failed to alloc temp buffer");
#else
	rb_str = rb_str_new(NULL, buf->total_size);
	ptr = RSTRING_PTR(rb_str);
#endif

	for (i = 0; i < buf->cur_chunk; ++i) {
		mochilo_buf_chunk *chunk = &buf->chunks[i];
		size_t chunk_len = chunk->end - chunk->ptr;

		memcpy(ptr, chunk->ptr, chunk_len);
		ptr += chunk_len;
		free(chunk->ptr);
	}

	free(buf->chunks);

#ifdef RUBINIUS
	rb_str = rb_str_new(alloc, buf->total_size);
#endif

	return rb_str;
}

mochilo_buf_chunk *mochilo_buf_rechunk2(mochilo_buf *buf, size_t chunk_size)
{
	mochilo_buf_chunk *chunks;

	skip_last_chunk(buf);

	if (buf->cur_chunk == buf->chunk_count) {
		if ((buf->chunk_count * 2) < buf->chunk_count)
			rb_raise(rb_eArgError, "Too many chunks required to serialize");

		chunks = realloc(buf->chunks, buf->chunk_count * 2 * sizeof(mochilo_buf_chunk));
		if (!chunks)
			rb_raise(rb_eNoMemError, "Failed to realloc chunks");

		buf->chunks = chunks;
		buf->chunk_count *= 2;
	}

	return init_cur_chunk(buf, chunk_size);
}

mochilo_buf_chunk *mochilo_buf_rechunk(mochilo_buf *buf)
{
	return mochilo_buf_rechunk2(buf, MOCHILO_CHUNK_SIZE);
}

void mochilo_buf_put(mochilo_buf *buf, const char *data, size_t len)
{
	mochilo_buf_chunk *chunk = &buf->chunks[buf->cur_chunk];

	if (unlikely(chunk->ptr + len > chunk->end))
		chunk = mochilo_buf_rechunk2(buf, len);

	memmove(chunk->ptr, data, len);
	chunk->ptr += len;
}

const char *mochilo_src_peek(mochilo_src *buf, size_t need)
{
	const char *ptr;

	if (unlikely(buf->ptr + need > buf->end))
		return NULL;

	ptr = buf->ptr;
	buf->ptr += need;
	return ptr;
}

