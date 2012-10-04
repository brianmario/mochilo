/*
 * Copyright (C) 2009-2012 the libgit2 contributors
 *
 * This file is part of libgit2, distributed under the GNU GPL v2 with
 * a Linking Exception. For full terms see the included COPYING file.
 */
#ifndef INCLUDE_buffer_h__
#define INCLUDE_buffer_h__

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

typedef unsigned char bool;
#define true 1
#define false 0
#define min(a,b) ((a < b) ? a : b)

static inline bool mochilo__isspace(int c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\f' ||
      c == '\r' || c == '\v' || c == 0x85 /* Unicode CR+LF */);
}

typedef struct {
  char *ptr;
  size_t asize, size;
} mochilo_buf;

extern char mochilo_buf__initbuf[];
extern char mochilo_buf__oom[];

#define MOCHILO_BUF_INIT { mochilo_buf__initbuf, 0, 0 }

/**
 * Initialize a mochilo_buf structure.
 *
 * For the cases where MOCHILO_BUF_INIT cannot be used to do static
 * initialization.
 */
void mochilo_buf_init(mochilo_buf *buf, size_t initial_size);

/**
 * Grow the buffer to hold at least `target_size` bytes.
 *
 * If the allocation fails, this will return an error and the buffer
 * will be marked as invalid for future operations.  The existing
 * contents of the buffer will be preserved however.
 * @return 0 on success or -1 on failure
 */
int mochilo_buf_grow(mochilo_buf *buf, size_t target_size);

void mochilo_buf_free(mochilo_buf *buf);

/**
 * Test if there have been any reallocation failures with this mochilo_buf.
 *
 * Any function that writes to a mochilo_buf can fail due to memory allocation
 * issues.  If one fails, the mochilo_buf will be marked with an OOM error and
 * further calls to modify the buffer will fail.  Check mochilo_buf_oom() at the
 * end of your sequence and it will be true if you ran out of memory at any
 * point with that buffer.
 *
 * @return false if no error, true if allocation error
 */
static inline bool mochilo_buf_oom(const mochilo_buf *buf)
{
  return (buf->ptr == mochilo_buf__oom);
}

/*
 * Functions below that return int value error codes will return 0 on
 * success or -1 on failure (which generally means an allocation failed).
 * Using a mochilo_buf where the allocation has failed with result in -1 from
 * all further calls using that buffer.  As a result, you can ignore the
 * return code of these functions and call them in a series then just call
 * mochilo_buf_oom at the end.
 */
int mochilo_buf_putc(mochilo_buf *buf, char c);
int mochilo_buf_put(mochilo_buf *buf, const char *data, size_t len);
#define mochilo_buf_append mochilo_buf_put
void mochilo_buf_clear(mochilo_buf *buf);
void mochilo_buf_consume(mochilo_buf *buf, const char *end);


static inline const char * mochilo_buf_cstr(const mochilo_buf *buf)
{
  return buf->ptr;
}

static inline size_t mochilo_buf_len(const mochilo_buf *buf)
{
  return buf->size;
}

#endif
