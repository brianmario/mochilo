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
#include <stdio.h>
#include "mochilo.h"

#define CSTR2SYM(s) (ID2SYM(rb_intern((s))))

static VALUE rb_mMochilo;
static VALUE rb_mMochiloCompat12;
static VALUE rb_eMochiloError;
VALUE rb_eMochiloPackError;
VALUE rb_eMochiloUnpackError;

extern void mochilo_pack_one(mochilo_buf *buf, VALUE rb_object, mochilo_pack_opts_t opts);

static VALUE rb_mochilo__unpack(VALUE rb_buffer, int trusted)
{
	VALUE rb_result;
	int error = -1;
	mochilo_src source;

	Check_Type(rb_buffer, T_STRING);

	source.ptr = RSTRING_PTR(rb_buffer);
	source.end = source.ptr + RSTRING_LEN(rb_buffer);
	source.trusted = trusted;

	error = mochilo_unpack_one((mo_value)&rb_result, &source);
	if (error < 0)
		rb_raise(rb_eMochiloUnpackError, "unpack failed (%d)", error);

	return rb_result;
}

/* Document-method: unpack
 *
 * call-seq:
 *     Mochilo.unpack(banana_pack_str) -> Object
 *
 * Unpacks a BananaPack stream into a Ruby object.
 */
static VALUE rb_mochilo_unpack(VALUE self, VALUE rb_buffer)
{
	return rb_mochilo__unpack(rb_buffer, 0);
}

/* Document-method: unpack_unsafe
 *
 * call-seq:
 *     Mochilo.unpack_unsafe(banana_pack_str) -> Object
 *
 * Unpacks a BananaPack stream into a Ruby object, in unsafe mode.
 * Only use this function if +banana_pack_str+ is trusted; otherwise
 * symbol DoS attacks are possible.
 */
static VALUE rb_mochilo_unpack_unsafe(VALUE self, VALUE rb_buffer)
{
	return rb_mochilo__unpack(rb_buffer, 1);
}

/* Document-method: pack
 *
 * call-seq:
 *     Mochilo.pack(obj) -> String
 *
 * Packs a Ruby object into BananaPack format.
 */
static VALUE rb_mochilo_pack(VALUE self, VALUE rb_obj)
{
	mochilo_buf buf;

	mochilo_buf_init(&buf);
	mochilo_pack_one(&buf, rb_obj, MOCHILO_PACK_V_1_3);
	return mochilo_buf_flush(&buf);
}

/* Document-method: pack_unsafe
 *
 * call-seq:
 *     Mochilo.pack_unsafe(obj) -> String
 *
 * Packs a Ruby object into BananaPack format, in unsafe mode.
 * This enables the Symbol type durring serialization and will
 * have to be deserialized in unsafe mode as well.
 */
static VALUE rb_mochilo_pack_unsafe(VALUE self, VALUE rb_obj)
{
	mochilo_buf buf;

	mochilo_buf_init(&buf);
	mochilo_pack_one(&buf, rb_obj, MOCHILO_PACK_V_1_3|MOCHILO_PACK_TRUSTED);
	return mochilo_buf_flush(&buf);
}

/* Document-method: pack
 *
 * call-seq:
 *     Mochilo::Compat_1_2.pack(obj) -> String
 *
 * Packs a Ruby object into BananaPack format, without
 * support for the new types (Time, Regexp) that were
 * added in 1.3.
 */
static VALUE rb_mochilo_pack_1_2(VALUE self, VALUE rb_obj)
{
	mochilo_buf buf;

	mochilo_buf_init(&buf);
	mochilo_pack_one(&buf, rb_obj, 0);
	return mochilo_buf_flush(&buf);
}

/* Document-method: pack_unsafe
 *
 * call-seq:
 *     Mochilo.pack_unsafe(obj) -> String
 *
 * Packs a Ruby object into BananaPack format, without
 * support for the new types (Time, Regexp) that were
 * added in 1.3, in unsafe mode.
 * This enables the Symbol type durring serialization and will
 * have to be deserialized in unsafe mode as well.
 */
static VALUE rb_mochilo_pack_unsafe_1_2(VALUE self, VALUE rb_obj)
{
	mochilo_buf buf;

	mochilo_buf_init(&buf);
	mochilo_pack_one(&buf, rb_obj, MOCHILO_PACK_TRUSTED);
	return mochilo_buf_flush(&buf);
}

void Init_mochilo()
{
	rb_mMochilo = rb_define_module("Mochilo");
	rb_define_method(rb_mMochilo, "unpack", rb_mochilo_unpack, 1);
	rb_define_method(rb_mMochilo, "unpack_unsafe", rb_mochilo_unpack_unsafe, 1);
	rb_define_method(rb_mMochilo, "pack", rb_mochilo_pack, 1);
	rb_define_method(rb_mMochilo, "pack_unsafe", rb_mochilo_pack_unsafe, 1);

	rb_mMochiloCompat12 = rb_define_module_under(rb_mMochilo, "Compat_1_2");
	rb_define_method(rb_mMochiloCompat12, "unpack", rb_mochilo_unpack, 1);
	rb_define_method(rb_mMochiloCompat12, "unpack_unsafe", rb_mochilo_unpack_unsafe, 1);
	rb_define_method(rb_mMochiloCompat12, "pack", rb_mochilo_pack_1_2, 1);
	rb_define_method(rb_mMochiloCompat12, "pack_unsafe", rb_mochilo_pack_unsafe_1_2, 1);

	rb_eMochiloError = rb_define_class_under(rb_mMochilo, "Error", rb_eStandardError);
	rb_eMochiloPackError = rb_define_class_under(rb_mMochilo, "PackError", rb_eMochiloError);
	rb_eMochiloUnpackError = rb_define_class_under(rb_mMochilo, "UnpackError", rb_eMochiloError);
}

