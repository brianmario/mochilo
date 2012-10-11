/*
 * Copyright (c) 2012, GitHub, Inc
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#define RSTRING_NOT_MODIFIED

#include <stdio.h>
#include "ruby.h"

#define CSTR2SYM(s) (ID2SYM(rb_intern((s))))

#include "mochilo.h"
#include "buffer.h"

static VALUE rb_mMochilo;

extern void mochilo_pack_one(mochilo_buf *buf, VALUE rb_object);

static VALUE rb_mochilo_unpack(VALUE self, VALUE rb_buffer)
{
	VALUE rb_result;
	int error = -1;
	mochilo_src source;

	Check_Type(rb_buffer, T_STRING);

	source.ptr = RSTRING_PTR(rb_buffer);
	source.end = source.ptr + RSTRING_LEN(rb_buffer);

	error = mochilo_unpack_one((mo_value)&rb_result, &source);
	if (error < 0)
		rb_raise(rb_eRuntimeError, "unpack failed (%d)", error);

	return rb_result;
}

static VALUE rb_mochilo_pack(VALUE self, VALUE rb_obj)
{
	mochilo_buf buf;

	mochilo_buf_init(&buf);
	mochilo_pack_one(&buf, rb_obj);
	return mochilo_buf_flush(&buf);
}

void Init_mochilo()
{
	rb_mMochilo = rb_define_module("Mochilo");
	rb_define_module_function(rb_mMochilo, "unpack", rb_mochilo_unpack, 1);
	rb_define_module_function(rb_mMochilo, "pack", rb_mochilo_pack, 1);
}

