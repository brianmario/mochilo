#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MOAPI static inline
#define unlikely(x)     __builtin_expect((x),0)

#include "mochilo.h"
#include "mochilo_api.h"

static inline void swap8(const uint8_t *buffer, void *out)
{
    unsigned char *ptr = (unsigned char *)out;
	*ptr = *buffer;
}

static inline void swap16(const uint8_t *buffer, void *out)
{
    unsigned char *ptr = (unsigned char *)out;
    ptr[0] = buffer[1];
    ptr[1] = buffer[0];
}

static inline void swap32(const uint8_t *buffer, void *out)
{
    unsigned char *ptr = (unsigned char *)out;
    ptr[0] = buffer[3];
    ptr[1] = buffer[2];
    ptr[2] = buffer[1];
    ptr[3] = buffer[0];
}

static inline void swap64(const uint8_t *buffer, void *out)
{
	unsigned char *ptr = (unsigned char *)out;
	ptr[0] = buffer[7];
	ptr[1] = buffer[6];
	ptr[2] = buffer[5];
	ptr[3] = buffer[4];
	ptr[4] = buffer[3];
	ptr[5] = buffer[2];
	ptr[6] = buffer[1];
	ptr[7] = buffer[0];
}

static inline int unpack_array(mo_value *_array, size_t elements, struct mochilo_buf *buf, void *opaque)
{
	size_t i;
	int error;

	mo_value array = moapi_array_new(elements, opaque);

	for (i = 0; i < elements; ++i) {
		mo_value element;

		if ((error = mochilo_unpack_one(&element, buf, opaque)) < 0)
			return error;

		moapi_array_append(array, element, opaque);
	}

	*_array = array;
	return 0;
}

static inline int unpack_hash(mo_value *_hash, size_t elements, struct mochilo_buf *buf, void *opaque)
{
	size_t i;
	int error;

	mo_value hash = moapi_hash_new(opaque);

	for (i = 0; i < elements; ++i) {
		mo_value key, value;

		if ((error = mochilo_unpack_one(&key, buf, opaque)) < 0)
			return error;

		if ((error = mochilo_unpack_one(&value, buf, opaque)) < 0)
			return error;

		moapi_hash_set(hash, key, value, opaque);
	}

	*_hash = hash;
	return 0;
}

#define WARD_BYTES(n) \
	if (unlikely(buf->ptr + n > buf->end)) { return -2; }

#define UNPACK_INT(sign, bits) { \
	sign##bits##_t integer; \
	WARD_BYTES((bits / 8)); \
	swap##bits(buf->ptr, &integer); \
	buf->ptr += (bits / 8); \
	*_value = moapi_##sign##bits##_new(integer, leader, opaque); \
	return 0; \
}

int mochilo_unpack_one(mo_value *_value, struct mochilo_buf *buf, void *opaque)
{
	uint8_t leader;

	WARD_BYTES(1);
	leader = *(buf->ptr)++;

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
			*_value = moapi_atom_new(leader, opaque);
			return 0;

		case MSGPACK_T_FLOAT:
		{
			float flt;

			WARD_BYTES(4);
			swap32(buf->ptr, &flt);
			buf->ptr += 4;

			*_value = moapi_float_new(flt, opaque);
			return 0;
		}

		case MSGPACK_T_DOUBLE:
		{
			double flt;

			WARD_BYTES(8);
			swap64(buf->ptr, &flt);
			buf->ptr += 8;

			*_value = moapi_double_new(flt, opaque);
			return 0;
		}

		case MSGPACK_T_ARRAY16:
		{
			uint16_t length;

			WARD_BYTES(2);
			swap16(buf->ptr, &length);
			buf->ptr += 2;

			return unpack_array(_value, (size_t)length, buf, opaque);
		}

		case MSGPACK_T_ARRAY32:
		{
			uint32_t length;

			WARD_BYTES(4);
			swap32(buf->ptr, &length);
			buf->ptr += 4;

			return unpack_array(_value, (size_t)length, buf, opaque);
		}

		case MSGPACK_T_MAP16:
		{
			uint16_t length;

			WARD_BYTES(2);
			swap16(buf->ptr, &length);
			buf->ptr += 2;

			return unpack_hash(_value, (size_t)length, buf, opaque);
		}

		case MSGPACK_T_MAP32:
		{
			uint32_t length;

			WARD_BYTES(4);
			swap32(buf->ptr, &length);
			buf->ptr += 4;

			return unpack_hash(_value, (size_t)length, buf, opaque);
		}

		case MSGPACK_T_RAW16:
		{
			uint16_t length;

			WARD_BYTES(2);
			swap16(buf->ptr, &length);
			buf->ptr += 2;

			WARD_BYTES(length);
			*_value = moapi_bytes_new(buf->ptr, (size_t)length, opaque);
			buf->ptr += length;

			return 0;
		}

		case MSGPACK_T_RAW32:
		{
			uint32_t length;

			WARD_BYTES(4);
			swap32(buf->ptr, &length);
			buf->ptr += 4;

			WARD_BYTES(length);
			*_value = moapi_bytes_new(buf->ptr, (size_t)length, opaque);
			buf->ptr += length;

			return 0;
		}

		default:
		{
			if (leader < 0x80 || leader >= 0xe0) {
				*_value = moapi_int8_new((int8_t)leader, MSGPACK_T_INT8, opaque);
				return 0;
			}

			else if (leader < 0x90) {
				uint8_t length = leader & (~0x80);
				return unpack_hash(_value, length, buf, opaque);
			}

			else if (leader < 0xa0) {
				uint8_t length = leader & (~0x90);
				return unpack_array(_value, length, buf, opaque);
			}

			else if (leader < 0xc0) { 
				uint8_t length = leader & (~0xa0);

				WARD_BYTES(length);
				*_value = moapi_bytes_new(buf->ptr, length, opaque);
				buf->ptr += length;

				return 0;
			}

			return -1;
		}
	}

#undef WARD_BYTES
}

