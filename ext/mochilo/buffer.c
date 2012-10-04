/*
 * Copyright (C) 2009-2012 the libgit2 contributors
 *
 * This file is part of libgit2, distributed under the GNU GPL v2 with
 * a Linking Exception. For full terms see the included COPYING file.
 */
#include <stdarg.h>
#include <ctype.h>
#include "buffer.h"

/* Used as default value for mochilo_buf->ptr so that people can always
 * assume ptr is non-NULL and zero terminated even for new mochilo_bufs.
 */
char mochilo_buf__initbuf[1];

char mochilo_buf__oom[1];

#define ENSURE_SIZE(b, d) \
  if ((d) > buf->asize && mochilo_buf_grow(b, (d)) < 0)\
    return -1;


void mochilo_buf_init(mochilo_buf *buf, size_t initial_size)
{
  buf->asize = 0;
  buf->size = 0;
  buf->ptr = mochilo_buf__initbuf;

  if (initial_size)
    mochilo_buf_grow(buf, initial_size);
}

static int mochilo_buf_try_grow(mochilo_buf *buf, size_t target_size)
{
  char *new_ptr;
  size_t new_size;

  if (buf->ptr == mochilo_buf__oom)
    return -1;

  if (target_size <= buf->asize)
    return 0;

  if (buf->asize == 0) {
    new_size = target_size;
    new_ptr = NULL;
  } else {
    new_size = buf->asize;
    new_ptr = buf->ptr;
  }

  /* grow the buffer size by 1.5, until it's big enough
   * to fit our target size */
  while (new_size < target_size)
    new_size = (new_size << 1) - (new_size >> 1);

  /* round allocation up to multiple of 8 */
  new_size = (new_size + 7) & ~7;

  new_ptr = realloc(new_ptr, new_size);
  if (!new_ptr)
    return -1;

  buf->asize = new_size;
  buf->ptr   = new_ptr;

  /* truncate the existing buffer size if necessary */
  if (buf->size >= buf->asize)
    buf->size = buf->asize - 1;
  buf->ptr[buf->size] = '\0';

  return 0;
}

int mochilo_buf_grow(mochilo_buf *buf, size_t target_size)
{
  int error = mochilo_buf_try_grow(buf, target_size);
  if (error != 0)
    buf->ptr = mochilo_buf__oom;
  return error;
}

void mochilo_buf_free(mochilo_buf *buf)
{
  if (!buf) return;

  if (buf->ptr != mochilo_buf__initbuf && buf->ptr != mochilo_buf__oom)
    free(buf->ptr);

  mochilo_buf_init(buf, 0);
}

void mochilo_buf_clear(mochilo_buf *buf)
{
  buf->size = 0;
  if (buf->asize > 0)
    buf->ptr[0] = '\0';
}

int mochilo_buf_putc(mochilo_buf *buf, char c)
{
  ENSURE_SIZE(buf, buf->size + 2);
  buf->ptr[buf->size++] = c;
  buf->ptr[buf->size] = '\0';
  return 0;
}

int mochilo_buf_put(mochilo_buf *buf, const char *data, size_t len)
{
  ENSURE_SIZE(buf, buf->size + len + 1);
  memmove(buf->ptr + buf->size, data, len);
  buf->size += len;
  buf->ptr[buf->size] = '\0';
  return 0;
}

void mochilo_buf_consume(mochilo_buf *buf, const char *end)
{
  if (end > buf->ptr && end <= buf->ptr + buf->size) {
    size_t consumed = end - buf->ptr;
    memmove(buf->ptr, end, buf->size - consumed);
    buf->size -= consumed;
    buf->ptr[buf->size] = '\0';
  }
}
