#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mochilo.h"

extern VALUE rb_eMochiloPackError;

void mochilo_pack_one(mochilo_buf *buf, VALUE rb_object, int trusted);

void mochilo_pack_fixnum(mochilo_buf *buf, VALUE rb_fixnum)
{
	long fixnum = NUM2LONG(rb_fixnum);

	if (fixnum < -0x20L) {
		if (fixnum < -0x8000L) {
			if (sizeof(long) > 4 && fixnum < -0x80000000L) {
				mochilo_buf_putc(buf, MSGPACK_T_INT64);
				mochilo_buf_put64be(buf, &fixnum);
			} else {
				int32_t fixnum32 = (int32_t)fixnum;
				mochilo_buf_putc(buf, MSGPACK_T_INT32);
				mochilo_buf_put32be(buf, &fixnum32);
			}
		} else {
			if (fixnum < -0x80L) {
				int16_t fixnum16 = (int16_t)fixnum;
				mochilo_buf_putc(buf, MSGPACK_T_INT16);
				mochilo_buf_put16be(buf, &fixnum16);
			} else {
				mochilo_buf_putc(buf, MSGPACK_T_INT8);
				mochilo_buf_putc(buf, (int8_t)fixnum);
			}
		}
	} else if (fixnum <= 0x7fL) {
		mochilo_buf_putc(buf, (uint8_t)fixnum);
	} else {
		if (fixnum <= 0xffffL) {
			if (fixnum <= 0xffL) {
				mochilo_buf_putc(buf, MSGPACK_T_UINT8);
				mochilo_buf_putc(buf, (uint8_t)fixnum);
			} else {
				uint16_t fixnum16 = (uint16_t)fixnum;
				mochilo_buf_putc(buf, MSGPACK_T_UINT16);
				mochilo_buf_put16be(buf, &fixnum16);
			}
		} else {
			if (fixnum <= 0xffffffffL) {
				uint32_t fixnum32 = (uint32_t)fixnum;
				mochilo_buf_putc(buf, MSGPACK_T_UINT32);
				mochilo_buf_put32be(buf, &fixnum32);
			} else if (sizeof(long) > 4) {
				mochilo_buf_putc(buf, MSGPACK_T_UINT64);
				mochilo_buf_put64be(buf, &fixnum);
			}
		}
	}
}

void mochilo_pack_bignum(mochilo_buf *buf, VALUE rb_bignum)
{
	if (RBIGNUM_POSITIVE_P(rb_bignum)) {
		uint64_t bignum = rb_big2ull(rb_bignum);

		mochilo_buf_putc(buf, MSGPACK_T_UINT64);
		mochilo_buf_put64be(buf, &bignum);
	} else {
		int64_t bignum = rb_big2ll(rb_bignum);

		mochilo_buf_putc(buf, MSGPACK_T_INT64);
		mochilo_buf_put64be(buf, &bignum);
	}
}

struct mochilo_hash_pack {
	mochilo_buf *buf;
	int trusted;
};

static int hash_callback(VALUE key, VALUE val, VALUE opaque)
{
	struct mochilo_hash_pack *hash_pack = (struct mochilo_hash_pack*)opaque;
	mochilo_pack_one(hash_pack->buf, key, hash_pack->trusted);
	mochilo_pack_one(hash_pack->buf, val, hash_pack->trusted);
	return 0;
}

void mochilo_pack_double(mochilo_buf *buf, VALUE rb_double)
{
	double d = RFLOAT_VALUE(rb_double);
	mochilo_buf_putc(buf, MSGPACK_T_DOUBLE);
	mochilo_buf_put64be(buf, &d);
}

void mochilo_pack_hash(mochilo_buf *buf, VALUE rb_hash, int trusted)
{
	struct mochilo_hash_pack hash_pack;
	long size = RHASH_SIZE(rb_hash);

	hash_pack.buf = buf;
	hash_pack.trusted = trusted;

	if (size < 0x10) {
		uint8_t lead = 0x80 | size;
		mochilo_buf_putc(buf, lead);
	}

	else if (size < 0x10000) {
		uint16_t lead = size;
		mochilo_buf_putc(buf, MSGPACK_T_MAP16);
		mochilo_buf_put16be(buf, &lead);
	}

	else {
		mochilo_buf_putc(buf, MSGPACK_T_MAP32);
		mochilo_buf_put32be(buf, &size);
	}

	rb_hash_foreach(rb_hash, &hash_callback, (VALUE)&hash_pack);
}

void mochilo_pack_bytes(mochilo_buf *buf, VALUE rb_bytes)
{
	long size = RSTRING_LEN(rb_bytes);

	if (size < 0x20) {
		uint8_t lead = 0xA0 | size;
		mochilo_buf_putc(buf, lead);
	}

	else if (size < 0x10000) {
		uint16_t lead = size;
		mochilo_buf_putc(buf, MSGPACK_T_RAW16);
		mochilo_buf_put16be(buf, &lead);
	}

	else {
		mochilo_buf_putc(buf, MSGPACK_T_RAW32);
		mochilo_buf_put32be(buf, &size);
	}

	mochilo_buf_put(buf, RSTRING_PTR(rb_bytes), size);
}

void mochilo_pack_symbol(mochilo_buf *buf, VALUE rb_symbol)
{
	long size;
	const char *name;

	name = rb_id2name(SYM2ID(rb_symbol));
	size = strlen(name);

	if (size < 0x100) {
		uint8_t lead = size;
		mochilo_buf_putc(buf, MSGPACK_T_SYM);
		mochilo_buf_putc(buf, lead);
	} else {
		rb_raise(rb_eMochiloPackError,
			"Symbol too long: must be under %d bytes, %ld given", 0x100, size);
	}

	mochilo_buf_put(buf, name, size);
}

#ifdef HAVE_RUBY_ENCODING_H
void mochilo_pack_str(mochilo_buf *buf, VALUE rb_str)
{
	long size = RSTRING_LEN(rb_str);
	rb_encoding *encoding;

	const struct mochilo_enc_map *enc2id;
	const char *enc_name;

	if (size < 0x10000) {
		uint16_t lead = size;
		mochilo_buf_putc(buf, MSGPACK_T_STR16);
		mochilo_buf_put16be(buf, &lead);
	}

	else {
		mochilo_buf_putc(buf, MSGPACK_T_STR32);
		mochilo_buf_put32be(buf, &size);
	}

	encoding = rb_enc_get(rb_str);
	enc_name = rb_enc_name(encoding);
	enc2id = mochilo_encoding_to_id(enc_name, (unsigned int)strlen(enc_name));

	mochilo_buf_putc(buf, enc2id ? enc2id->id : 0);
	mochilo_buf_put(buf, RSTRING_PTR(rb_str), size);
}
#endif

void mochilo_pack_array(mochilo_buf *buf, VALUE rb_array, int trusted)
{
	long i, size = RARRAY_LEN(rb_array);

	if (size < 0x10) {
		uint8_t lead = 0x90 | size;
		mochilo_buf_putc(buf, lead);
	}

	else if (size < 0x10000) {
		uint16_t lead = size;
		mochilo_buf_putc(buf, MSGPACK_T_ARRAY16);
		mochilo_buf_put16be(buf, &lead);
	}

	else {
		mochilo_buf_putc(buf, MSGPACK_T_ARRAY32);
		mochilo_buf_put32be(buf, &size);
	}

	for (i = 0; i < size; i++) {
		mochilo_pack_one(buf, rb_ary_entry(rb_array, i), trusted);
	}
}

void mochilo_pack_one(mochilo_buf *buf, VALUE rb_object, int trusted)
{
#ifndef RUBINIUS
	if (rb_object == Qnil) {
		mochilo_buf_putc(buf, MSGPACK_T_NIL);
	}
	else if (rb_object == Qtrue) {
		mochilo_buf_putc(buf, MSGPACK_T_TRUE);
	}
	else if (rb_object == Qfalse) {
		mochilo_buf_putc(buf, MSGPACK_T_FALSE);
	}
	else if (FIXNUM_P(rb_object)) {
		mochilo_pack_fixnum(buf, rb_object);
	}
	else if (SYMBOL_P(rb_object)) {
		if (trusted)
			mochilo_pack_symbol(buf, rb_object);
		else
			mochilo_pack_str(buf, rb_obj_as_string(rb_object));
	}
	else {
		switch (BUILTIN_TYPE(rb_object)) {
		case T_STRING:
#ifdef HAVE_RUBY_ENCODING_H
			if (ENCODING_GET(rb_object) != 0)
				mochilo_pack_str(buf, rb_object);
			else
#endif
				mochilo_pack_bytes(buf, rb_object);
			return;

		case T_HASH:
			mochilo_pack_hash(buf, rb_object, trusted);
			return;

		case T_ARRAY:
			mochilo_pack_array(buf, rb_object, trusted);
			return;

		case T_FLOAT:
			mochilo_pack_double(buf, rb_object);
			return;

		case T_BIGNUM:
			mochilo_pack_bignum(buf, rb_object);
			return;

		default:
			rb_raise(rb_eMochiloPackError,
				"Unsupported object type: %s", rb_obj_classname(rb_object));
			return;
		}
	}
#else // RUBINIUS
	switch (rb_type(rb_object)) {
		case T_NIL:
			mochilo_buf_putc(buf, MSGPACK_T_NIL);
			return;

		case T_FALSE:
			mochilo_buf_putc(buf, MSGPACK_T_FALSE);
			return;

		case T_TRUE:
			mochilo_buf_putc(buf, MSGPACK_T_TRUE);
			return;

		case T_FIXNUM:
			mochilo_pack_fixnum(buf, rb_object);
			return;

		case T_BIGNUM:
			mochilo_pack_bignum(buf, rb_object);
			return;

		case T_SYMBOL:
			if (trusted)
				mochilo_pack_symbol(buf, rb_object);
			else
				mochilo_pack_str(buf, rb_obj_as_string(rb_object));
			return;

		case T_STRING:
#ifdef HAVE_RUBY_ENCODING_H
			if (ENCODING_GET(rb_object) != 0)
				mochilo_pack_str(buf, rb_object);
			else
#endif
				mochilo_pack_bytes(buf, rb_object);
			return;

		case T_HASH:
			mochilo_pack_hash(buf, rb_object, trusted);
			return;

		case T_ARRAY:
			mochilo_pack_array(buf, rb_object, trusted);
			return;

		case T_FLOAT:
			mochilo_pack_double(buf, rb_object);
			return;

		default:
			rb_raise(rb_eMochiloPackError,
				"Unsupported object type: %s", rb_obj_classname(rb_object));
			return;
	}
#endif
}

