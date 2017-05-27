#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mochilo.h"
#include "mochilo_api.h"

extern VALUE rb_eMochiloUnpackError;

static inline int unpack_array(mo_value *_array, size_t elements, mochilo_src *buf)
{
	size_t i;
	int error;

	mo_value array = moapi_array_new(elements);

	for (i = 0; i < elements; ++i) {
		mo_value element;

		if ((error = mochilo_unpack_one(&element, buf)) < 0)
			return error;

		moapi_array_append(array, element);
	}

	*_array = array;
	return 0;
}

static inline int unpack_hash(mo_value *_hash, size_t elements, mochilo_src *buf)
{
	size_t i;
	int error;

	mo_value hash = moapi_hash_new();

	for (i = 0; i < elements; ++i) {
		mo_value key, value;

		if ((error = mochilo_unpack_one(&key, buf)) < 0)
			return error;

		if ((error = mochilo_unpack_one(&value, buf)) < 0)
			return error;

		moapi_hash_set(hash, key, value);
	}

	*_hash = hash;
	return 0;
}

#define UNPACK_INT(sign, bits) { \
	sign##bits##_t integer; \
	SRC_ENSURE_AVAIL(src, (bits / 8)); \
	mochilo_src_get##bits##be(src, &integer); \
	*_value = moapi_##sign##bits##_new(integer); \
	return 0; \
}

static int mochilo_unpack_custom(mo_value *_value, mochilo_src *src, size_t length)
{
	uint8_t custom_type;

	SRC_ENSURE_AVAIL(src, length);

	if (length < 1)
		return -1;
	length--;
	mochilo_src_get8be(src, &custom_type);

	switch (custom_type) {
		case MOCHILO_T_SYMBOL:
		{
			const char *ptr;
			if (!(ptr = mochilo_src_peek(src, length)))
				return -1;
			*_value = moapi_symbol_new(ptr, length);
			return 0;
		}

		case MOCHILO_T_REGEXP:
		{
			uint32_t options;
			uint8_t encoding;
			const char *ptr;

			if (length < 5)
				return -1;
			mochilo_src_get32be(src, &options);
			mochilo_src_get8be(src, &encoding);
			length -= 5;

			if (!(ptr = mochilo_src_peek(src, length)))
				return -1;

			*_value = moapi_regexp_new(ptr, length, encoding, options);
			return 0;
		}

		case MOCHILO_T_TIME:
		{
			uint64_t sec;
			uint64_t usec;
			int32_t utc_offset;

			if (length != 8+8+4)
				return -1;

			mochilo_src_get64be(src, &sec);
			mochilo_src_get64be(src, &usec);
			mochilo_src_get32be(src, &utc_offset);

			*_value = moapi_time_new(sec, usec, utc_offset);
			return 0;
		}

		default:
			rb_raise(rb_eMochiloUnpackError, "unknown custom type 0x%02x", custom_type);
			return -1;
	}
}

int mochilo_unpack_one(mo_value *_value, mochilo_src *src)
{
	uint8_t leader;

	if (!SRC_CHECK_AVAIL(src, 1))
		return MSGPACK_ENOTHING;

	mochilo_src_get8be(src, &leader);

	switch (leader) {
		case MSGPACK_T_UINT8:
			UNPACK_INT(uint, 8);

		case MSGPACK_T_INT8:
			UNPACK_INT(int, 8);

		case MSGPACK_T_UINT16:
			UNPACK_INT(uint, 16);

		case MSGPACK_T_INT16:
			UNPACK_INT(int, 16);

		case MSGPACK_T_UINT32:
			UNPACK_INT(uint, 32);

		case MSGPACK_T_INT32:
			UNPACK_INT(int, 32);

		case MSGPACK_T_UINT64:
			UNPACK_INT(uint, 64);

		case MSGPACK_T_INT64:
			UNPACK_INT(int, 64);

		case MSGPACK_T_NIL:
		case MSGPACK_T_TRUE:
		case MSGPACK_T_FALSE:
			*_value = moapi_atom_new(leader);
			return 0;

		case MSGPACK_T_FLOAT:
		{
			float flt;

			SRC_ENSURE_AVAIL(src, 4);
			mochilo_src_get32be(src, &flt);

			*_value = moapi_float_new(flt);
			return 0;
		}

		case MSGPACK_T_DOUBLE:
		{
			double flt;

			SRC_ENSURE_AVAIL(src, 8);
			mochilo_src_get64be(src, &flt);

			*_value = moapi_double_new(flt);
			return 0;
		}

		case MSGPACK_T_ARRAY16:
		{
			uint16_t length;

			SRC_ENSURE_AVAIL(src, 2);
			mochilo_src_get16be(src, &length);

			return unpack_array(_value, (size_t)length, src);
		}

		case MSGPACK_T_ARRAY32:
		{
			uint32_t length;

			SRC_ENSURE_AVAIL(src, 4);
			mochilo_src_get32be(src, &length);

			return unpack_array(_value, (size_t)length, src);
		}

		case MSGPACK_T_MAP16:
		{
			uint16_t length;

			SRC_ENSURE_AVAIL(src, 2);
			mochilo_src_get16be(src, &length);

			return unpack_hash(_value, (size_t)length, src);
		}

		case MSGPACK_T_MAP32:
		{
			uint32_t length;

			SRC_ENSURE_AVAIL(src, 4);
			mochilo_src_get32be(src, &length);

			return unpack_hash(_value, (size_t)length, src);
		}

		case MSGPACK_T_STR8:
		{
			uint8_t length;
			const char *ptr;

			SRC_ENSURE_AVAIL(src, 1);
			mochilo_src_get8be(src, &length);

			if (!(ptr = mochilo_src_peek(src, length)))
				return -1;

			*_value = moapi_str_new(ptr, length, MSGPACK_ENC_UTF_8);
			return 0;
		}

		case MSGPACK_T_STR16:
		{
			uint16_t length;
			const char *ptr;

			SRC_ENSURE_AVAIL(src, 2 + 1);
			mochilo_src_get16be(src, &length);

			if (!(ptr = mochilo_src_peek(src, length)))
				return -1;

			*_value = moapi_str_new(ptr, length, MSGPACK_ENC_UTF_8);
			return 0;
		}

		case MSGPACK_T_STR32:
		{
			uint32_t length;
			const char *ptr;

			SRC_ENSURE_AVAIL(src, 4 + 1);
			mochilo_src_get32be(src, &length);

			if (!(ptr = mochilo_src_peek(src, length)))
				return -1;

			*_value = moapi_str_new(ptr, length, MSGPACK_ENC_UTF_8);
			return 0;
		}

		case MSGPACK_T_ENC8:
		{
			uint8_t length;
			uint8_t encoding;
			const char *ptr;

			SRC_ENSURE_AVAIL(src, 1 + 1);
			mochilo_src_get8be(src, &length);
			mochilo_src_get8be(src, &encoding);

			if (encoding == MOCHILO_EXT_TYPE)
				return mochilo_unpack_custom(_value, src, length);

			if (!(ptr = mochilo_src_peek(src, length)))
				return -1;

			*_value = moapi_str_new(ptr, length, encoding);
			return 0;
		}

		case MSGPACK_T_ENC16:
		{
			uint16_t length;
			uint8_t encoding;
			const char *ptr;

			SRC_ENSURE_AVAIL(src, 2 + 1);
			mochilo_src_get16be(src, &length);
			mochilo_src_get8be(src, &encoding);

			if (encoding == MOCHILO_EXT_TYPE)
				return mochilo_unpack_custom(_value, src, length);

			if (!(ptr = mochilo_src_peek(src, length)))
				return -1;

			*_value = moapi_str_new(ptr, length, encoding);
			return 0;
		}

		case MSGPACK_T_ENC32:
		{
			uint32_t length;
			uint8_t encoding;
			const char *ptr;

			SRC_ENSURE_AVAIL(src, 4 + 1);
			mochilo_src_get32be(src, &length);
			mochilo_src_get8be(src, &encoding);

			if (encoding == MOCHILO_EXT_TYPE)
				return mochilo_unpack_custom(_value, src, length);

			if (!(ptr = mochilo_src_peek(src, length)))
				return -1;

			*_value = moapi_str_new(ptr, length, encoding);
			return 0;
		}

		case MSGPACK_T_BIN8:
		{
			uint8_t length;
			const char *ptr;

			SRC_ENSURE_AVAIL(src, 1);
			mochilo_src_get8be(src, &length);

			if (!(ptr = mochilo_src_peek(src, length)))
				return -1;

			*_value = moapi_bytes_new(ptr, length);
			return 0;
		}

		case MSGPACK_T_BIN16:
		{
			uint16_t length;
			const char *ptr;

			SRC_ENSURE_AVAIL(src, 2);
			mochilo_src_get16be(src, &length);

			if (!(ptr = mochilo_src_peek(src, length)))
				return -1;

			*_value = moapi_bytes_new(ptr, length);
			return 0;
		}

		case MSGPACK_T_BIN32:
		{
			uint32_t length;
			const char *ptr;

			SRC_ENSURE_AVAIL(src, 4);
			mochilo_src_get32be(src, &length);

			if (!(ptr = mochilo_src_peek(src, length)))
				return -1;

			*_value = moapi_bytes_new(ptr, length);
			return 0;
		}

		default:
		{
			if (leader < 0x80 || leader >= 0xe0) {
				*_value = moapi_int8_new((int8_t)leader);
				return 0;
			}

			else if (leader < 0x90) {
				uint8_t length = leader & (~0x80);
				return unpack_hash(_value, length, src);
			}

			else if (leader < 0xa0) {
				uint8_t length = leader & (~0x90);
				return unpack_array(_value, length, src);
			}

			else if (leader < 0xc0) {
				uint8_t length = leader & (~0xa0);
				const char *ptr;

				if (!(ptr = mochilo_src_peek(src, length)))
					return -1;

				*_value = moapi_str_new(ptr, length, MSGPACK_ENC_UTF_8);
				return 0;
			}

			return MSGPACK_EINVALID;
		}
	}
}
