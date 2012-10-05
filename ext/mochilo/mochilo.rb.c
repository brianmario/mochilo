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

static VALUE rb_mMochilo;
static VALUE rb_cMochiloPacker;
static VALUE rb_cMochiloUnpacker;

extern void mochilo_pack_one(mochilo_buf *buf, VALUE rb_object);


static void rb_mochilo__buf_free(void *buf)
{
	mochilo_buf_free(buf);
	xfree(buf);
}

static VALUE rb_mochilo_unpack(VALUE self, VALUE rb_buffer)
{
	VALUE rb_result;
	int error = -1;
	mochilo_src source;

	Check_Type(rb_buffer, T_STRING);

	mochilo_src_init_static(&source, RSTRING_PTR(rb_buffer), RSTRING_LEN(rb_buffer));

	error = mochilo_unpack_one((mo_value)&rb_result, &source);
	if (error < 0)
		rb_raise(rb_eRuntimeError, "unpack failed (%d)", error);

	return rb_result;
}

static int rb_mochilo__src_readio(char *buffer, size_t need, void *io)
{
	size_t read_count;
	VALUE rbr = rb_funcall((VALUE)io, rb_intern("read"), 1, INT2FIX((long)need));

	if (NIL_P(rbr))
		return -1;

	read_count = (size_t)FIX2INT(rbr);
	if (read_count < need)
		return -1;

	return need;
}

static VALUE rb_mochilo_unpacker_new(int argc, VALUE *argv, VALUE self)
{
	mochilo_buf *source;
	long buffer_size = 1024;
	VALUE rb_io_object, rb_buf_size, rb_block;

	rb_scan_args(argc, argv, "02&", &rb_io_object, &rb_buf_size, &rb_block);

	if (!NIL_P(rb_buf_size)) {
		Check_Type(rb_buf_size, T_FIXNUM);
		buffer_size = FIX2INT(rb_buf_size);
	}

	source = xmalloc(sizeof(mochilo_src));

	if (!NIL_P(rb_io_object)) {
		mochilo_src_init_stream(source, (size_t)buffer_size, &rb_mochilo__src_readio, (void *)rb_io_object);
	}
	else if (!NIL_P(rb_block)) {
		rb_raise(rb_eArgError, "not supported");
		// mochilo_src_init_stream(source, (size_t)buffer_size, &rb_mochilo__src_yield, (void *)rb_block);
	}
	else {
		rb_raise(rb_eArgError, "expected either IO or block");
	}

	return Data_Wrap_Struct(self, NULL, &rb_mochilo__buf_free, source);
}

static VALUE rb_mochilo_unpacker_each(VALUE self)
{
	mochilo_src *source;
	int error = -1;
	VALUE rb_result;

	if (!rb_block_given_p())
		return rb_funcall(self, rb_intern("to_enum"), 1, CSTR2SYM("each"));

	Data_Get_Struct(self, mochilo_src, source);

	while ((error = mochilo_unpack_one((mo_value)&rb_result, source)) == 0) {
		rb_yield(rb_result);
	}

	return Qnil;
}


static int rb_mochilo__buf_catstr(const char *data, size_t len, void *str)
{
	rb_str_buf_cat((VALUE)str, data, (long)len);
	return 0;
}

static int rb_mochilo__buf_catio(const char *data, size_t len, void *io)
{
	rb_io_write((VALUE)io, rb_str_new(data, len));
	return 0;
}

static int rb_mochilo__buf_yield(const char *data, size_t len, void *func)
{
	rb_funcall((VALUE)func, rb_intern("call"), 1, rb_str_new(data, len));
	return 0;
}

static VALUE rb_mochilo_pack(VALUE self, VALUE rb_obj)
{
	mochilo_buf buf;
	VALUE rb_buffer;

	rb_buffer = rb_str_buf_new(1024);

	mochilo_buf_init(&buf, 1024, &rb_mochilo__buf_catstr, (void *)rb_buffer);
	mochilo_pack_one(&buf, rb_obj);
	mochilo_buf_flush(&buf);
	mochilo_buf_free(&buf);

	return rb_buffer;
}

static VALUE rb_mochilo_packer_new(int argc, VALUE *argv, VALUE self)
{
	mochilo_buf *buf;
	long buffer_size = 1024;
	VALUE rb_io_object, rb_buf_size, rb_block;

	rb_scan_args(argc, argv, "02&", &rb_io_object, &rb_buf_size, &rb_block);

	if (!NIL_P(rb_buf_size)) {
		Check_Type(rb_buf_size, T_FIXNUM);
		buffer_size = FIX2INT(rb_buf_size);
	}

	buf = xmalloc(sizeof(mochilo_buf));

	if (!NIL_P(rb_io_object)) {
		mochilo_buf_init(buf, (size_t)buffer_size, &rb_mochilo__buf_catio, (void *)rb_io_object);
	}
	else if (!NIL_P(rb_block)) {
		mochilo_buf_init(buf, (size_t)buffer_size, &rb_mochilo__buf_yield, (void *)rb_block);
	}
	else {
		rb_raise(rb_eArgError, "expected either IO or block");
	}

	return Data_Wrap_Struct(self, NULL, &rb_mochilo__buf_free, buf);
}

static VALUE rb_mochilo_packer_write(VALUE self, VALUE rb_object)
{
	mochilo_buf *buf;
	Data_Get_Struct(self, mochilo_buf, buf);
	mochilo_pack_one(buf, rb_object);
	return self;
}

static VALUE rb_mochilo_packer_flush(VALUE self)
{
	mochilo_buf *buf;
	Data_Get_Struct(self, mochilo_buf, buf);
	mochilo_buf_flush(buf);
	return Qnil;
}

void __attribute__ ((visibility ("default"))) Init_mochilo()
{
	rb_mMochilo = rb_define_module("Mochilo");
	rb_define_module_function(rb_mMochilo, "unpack", rb_mochilo_unpack, 1);
	rb_define_module_function(rb_mMochilo, "pack", rb_mochilo_pack, 1);

	rb_cMochiloPacker = rb_define_class_under(rb_mMochilo, "Packer", rb_cObject);
	rb_define_singleton_method(rb_cMochiloPacker, "new", rb_mochilo_packer_new, -1);

	rb_define_method(rb_cMochiloPacker, "<<", rb_mochilo_packer_write, 1);
	rb_define_method(rb_cMochiloPacker, "flush", rb_mochilo_packer_flush, 0);
}

