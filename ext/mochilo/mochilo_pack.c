#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mochilo.h"
#include "intern.h"
#include <ruby.h>
#include <st.h>

void mochilo_pack_one(mochilo_buf *buf, VALUE rb_object);

static void pack_64(mochilo_buf *buf, const void *fixnum64)
{
	uint8_t be[8];
	swap64(fixnum64, be);
	mochilo_buf_append(buf, &be[0], sizeof(be));
}

static void pack_32(mochilo_buf *buf, const void *fixnum32)
{
	uint8_t be[4];
	swap32(fixnum32, be);
	mochilo_buf_append(buf, &be[0], sizeof(be));
}

static void pack_16(mochilo_buf *buf, const void *fixnum16)
{
	uint8_t be[2];
	swap16(fixnum16, be);
	mochilo_buf_append(buf, &be[0], sizeof(be));
}

void mochilo_pack_fixnum(mochilo_buf *buf, VALUE rb_fixnum)
{
	long fixnum = NUM2LONG(rb_fixnum);

	if(fixnum < -0x20L) {
		if(fixnum < -0x8000L) {
			if(fixnum < -0x80000000L) {
				mochilo_buf_putc(buf, MSGPACK_T_INT64);
				pack_64(buf, &fixnum);
			} else {
				mochilo_buf_putc(buf, MSGPACK_T_INT32);
				pack_32(buf, &fixnum);
			}
		} else {
			if(fixnum < -0x80L) {
				mochilo_buf_putc(buf, MSGPACK_T_INT16);
				pack_16(buf, &fixnum);
			} else {
				mochilo_buf_putc(buf, MSGPACK_T_INT8);
				mochilo_buf_putc(buf, (uint8_t)fixnum);
			}
		}
	} else if(fixnum <= 0x7fL) {
		mochilo_buf_putc(buf, (uint8_t)fixnum);
	} else {
		if(fixnum <= 0xffffL) {
			if(fixnum <= 0xffL) {
				mochilo_buf_putc(buf, MSGPACK_T_UINT8);
				mochilo_buf_putc(buf, (uint8_t)fixnum);
			} else {
				mochilo_buf_putc(buf, MSGPACK_T_UINT16);
				pack_16(buf, &fixnum);
			}
		} else {
			if(fixnum <= 0xffffffffL) {
				mochilo_buf_putc(buf, MSGPACK_T_UINT32);
				pack_32(buf, &fixnum);
			} else {
				mochilo_buf_putc(buf, MSGPACK_T_UINT64);
				pack_64(buf, &fixnum);
			}
		}
	}
}

void mochilo_pack_bignum(mochilo_buf *buf, VALUE rb_bignum)
{
	if(RBIGNUM_POSITIVE_P(rb_bignum)) {
		uint64_t bignum;
		bignum = rb_big2ull(rb_bignum);

		mochilo_buf_putc(buf, MSGPACK_T_UINT64);
		pack_64(buf, &bignum);
	} else {
		int64_t bignum;
		bignum = rb_big2ll(rb_bignum);

		mochilo_buf_putc(buf, MSGPACK_T_INT64);
		pack_64(buf, &bignum);
	}
}


static int hash_callback(VALUE key, VALUE val, VALUE opaque)
{
	mochilo_buf *buf = (mochilo_buf *)opaque;
	mochilo_pack_one(buf, key);
	mochilo_pack_one(buf, val);
	return 0;
}

void mochilo_pack_double(mochilo_buf *buf, VALUE rb_double)
{
	double d = RFLOAT_VALUE(rb_double);
	mochilo_buf_putc(buf, MSGPACK_T_DOUBLE);
	pack_64(buf, &d);
}

void mochilo_pack_hash(mochilo_buf *buf, VALUE rb_hash)
{
	long size = RHASH_SIZE(rb_hash);

	if (size < 0x10) {
		uint8_t lead = 0x80 | size;
		mochilo_buf_putc(buf, lead);
	}

	else if (size < 0x10000) {
		uint16_t lead = size;
		mochilo_buf_putc(buf, MSGPACK_T_MAP16);
		pack_16(buf, &lead);
	}

	else {
		mochilo_buf_putc(buf, MSGPACK_T_MAP32);
		pack_32(buf, &size);
	}

	rb_hash_foreach(rb_hash, &hash_callback, (VALUE)buf);
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
		pack_16(buf, &lead);
	}

	else {
		mochilo_buf_putc(buf, MSGPACK_T_RAW32);
		pack_32(buf, &size);
	}

	mochilo_buf_append(buf, RSTRING_PTR(rb_bytes), size);
}

void mochilo_pack_array(mochilo_buf *buf, VALUE rb_array)
{
	long i, size = RARRAY_LEN(rb_array);

	if (size < 0x10) {
		uint8_t lead = 0x90 | size;
		mochilo_buf_putc(buf, lead);
	}

	else if (size < 0x10000) {
		uint16_t lead = size;
		mochilo_buf_putc(buf, MSGPACK_T_ARRAY16);
		pack_16(buf, &lead);
	}

	else {
		mochilo_buf_putc(buf, MSGPACK_T_ARRAY32);
		pack_32(buf, &size);
	}

	for (i = 0; i < size; i++) {
		mochilo_pack_one(buf, RARRAY_PTR(rb_array)[i]);
	}
}

void mochilo_pack_one(mochilo_buf *buf, VALUE rb_object)
{
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

		case T_STRING:
			mochilo_pack_bytes(buf, rb_object);
			return;

		case T_HASH:
			mochilo_pack_hash(buf, rb_object);
			return;

		case T_ARRAY:
			mochilo_pack_array(buf, rb_object);
			return;

		case T_FLOAT:
			mochilo_pack_double(buf, rb_object);
			return;

		default:
			return;
	}
}

