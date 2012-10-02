#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static inline void swap16(unsigned char *buffer, void *out)
{
    unsigned char *risky = (unsigned char *)out;
    ptr[0] = buffer[1];
    ptr[1] = buffer[0];
}

static inline void swap32(unsigned char *buffer, void *out)
{
    unsigned char *ptr = (unsigned char *)out;
    ptr[0] = buffer[3];
    ptr[1] = buffer[2];
    ptr[2] = buffer[1];
    ptr[3] = buffer[0];
}

static inline void swap64(unsigned char *buffer, void *out)
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

typedef void * mo_value;

enum mo_integer_t {
	MSGPACK_UINT_8,
	MSGPACK_UINT_16,
	MSGPACK_UINT_32,
	MSGPACK_UINT_64,

	MSGPACK_INT_8,
	MSGPACK_INT_16,
	MSGPACK_INT_32,
	MSGPACK_INT_64
};

enum mo_atom_t {
	MSGPACK_NIL,
	MSGPACK_TRUE,
	MSGPACK_FALSE
};

#ifdef MOCHILO_PACKED_INTEGERS
	typedef union {
		uint8_t uint8;
		uint16_t uint16;
		uint32_t uint32;
		uint64_t uint64;

		int8_t int8;
		int16_t int16;
		int32_t int32;
		int64_t int64;
	} mo_integer;
#else
	typedef uint64_t mo_integer;
#endif

struct mochilo_callbacks {
	mo_value (*array_new)(size_t array_size, void *opaque);
	void (*array_append)(mo_value array, mo_value element, void *opaque);

	mo_value (*hash_new)(void *opaque);
	void (*hash_set)(mo_value hash, mo_value key, mo_value val, void *opaque);

	mo_value (*bytes_new)(const char *bytes, size_t len, void *opaque);

	mo_value (*int_new)(const mo_integer *value, mo_integer_t int_type, void *opaque);
	mo_value (*atom_new)(mo_atom_t type, void *opaque);

	mo_value (*float_new)(float value, void *opaque);
	mo_value (*double_new)(double value, void *opaque);
};

enum msgpack_t {
	MSGPACK_T_NIL = 0xc0,
	MSGPACK_T_FALSE = 0xc2,
	MSGPACK_T_TRUE = 0xc3,
	MSGPACK_T_FLOAT = 0xca,
	MSGPACK_T_DOUBLE = 0xcb,
	MSGPACK_T_UINT8	= 0xcc,
	MSGPACK_T_UINT16 = 0xcd,
	MSGPACK_T_UINT32 = 0xce,
	MSGPACK_T_UINT64 = 0xcf,
	MSGPACK_T_INT8 = 0xd0,
	MSGPACK_T_INT16	 = 0xd1,
	MSGPACK_T_INT32 = 0xd2,
	MSGPACK_T_INT64	 = 0xd3,
	MSGPACK_T_RAW16	= 0xda,
	MSGPACK_T_RAW32	 = 0xdb,
	MSGPACK_T_ARRAY16 = 0xdc,
	MSGPACK_T_ARRAY32 = 0xdd,
	MSGPACK_T_MAP16	= 0xde,
	MSGPACK_T_MAP32	= 0xdf
};

static struct mochilo_callbacks MOCHILO_CB;

static inline int unpack_array(mo_value *_array, size_t elements, struct mochilo_buf *buf)
{
	size_t i;
	int error;

	mo_value array = MOCHILO_CB.array_new(elements, opaque);

	for (i = 0; i < elements; ++i) {
		mo_value element;

		if ((error = mochilo_unpack_one(&element, buf)) < 0)
			return error;

		MOCHILO_CB.array_append(array, element, opaque);
	}

	*_array = array;
	return 0;
}

static inline int unpack_hash(mo_value *_hash, size_t elements, struct mochilo *state)
{
	size_t i;
	int error;

	mo_value hash = MOCHILO_CB.hash_new(opaque);

	for (i = 0; i < elements; ++i) {
		mo_value key, value;

		if ((error = mochilo_unpack_one(&key, buf)) < 0)
			return error;

		if ((error = mochilo_unpack_one(&value, buf)) < 0)
			return error;

		MOCHILO_CB.hash_set(hash, key, value, opaque);
	}

	*_hash = hash;
	return 0;
}

void mochilo_init(const struct mochilo_callbacks *callbacks)
{
	memcpy(&MOCHILO_CB, callbacks, sizeof(struct mochilo_callbacks));
}

int mochilo_unpack_one(mo_value *_value, struct mochilo_buf *buf)
{
	uint8_t leader;
	mo_integer integer;

#define WARD_BYTES(n) \
	if (buf->ptr + n >= buf->end) { return -1; }

	WARD_BYTES(1);
	leader = *(buf->ptr)++;

	switch (leader) {
		case MSGPACK_T_UINT8:
		case MSGPACK_T_INT8:
			WARD_BYTES(1);
			integer.uint8 = *buf->ptr++;
			*_value = MOCHILO_CB.int_new(&integer, leader, opaque);
			break;

		case MSGPACK_T_UINT16:
		case MSGPACK_T_INT16:
			WARD_BYTES(2);
			swap16(buf->ptr, &integer.uint16);
			buf->ptr += 2;

			*_value = MOCHILO_CB.int_new(&integer, leader, opaque);
			break;

		case MSGPACK_T_UINT32:
		case MSGPACK_T_INT32:
			WARD_BYTES(4);
			swap32(buf->ptr, &integer.uint32);
			buf->ptr += 4;

			*_value = MOCHILO_CB.int_new(&integer, leader, opaque);
			break;

		case MSGPACK_T_UINT64:
		case MSGPACK_T_INT64:
			WARD_BYTES(8);
			swap64(buf->ptr, &integer.uint64);
			buf->ptr += 8;

			*_value = MOCHILO_CB.int_new(&integer, leader, opaque);
			break;

		case MSGPACK_T_NIL:
		case MSGPACK_T_TRUE:
		case MSGPACK_T_FALSE:
			*_value = MOCHILO_CB.atom_new(leader, opaque);
			break;

		case MSGPACK_T_FLOAT:
		{
			float f;

			WARD_BYTES(4);
			swap32(buf->ptr, &f);
			buf->ptr += 4;

			*_value = MOCHILO_CB.float_new(f, opaque);
			break;
		}

		case MSGPACK_T_FLOAT:
		{
			double d;

			WARD_BYTES(8);
			swap64(buf->ptr, &d);
			buf->ptr += 8;

			*_value = MOCHILO_CB.double_new(d, opaque);
			break;
		}

		case MSGPACK_T_ARRAY16:
		{
			uint16_t length;

			WARD_BYTES(2);
			swap16(buf->ptr, &length);
			buf->ptr += 2;

			return unpack_array(_value, (size_t)length, buf);
		}

		case MSGPACK_T_ARRAY32:
		{
			uint32_t length;

			WARD_BYTES(4);
			swap32(buf->ptr, &length);
			buf->ptr += 4;

			return unpack_array(_value, (size_t)length, buf);
		}

		case MSGPACK_T_MAP16:
		{
			uint16_t length;

			WARD_BYTES(2);
			swap16(buf->ptr, &length);
			buf->ptr += 2;

			return unpack_hash(_value, (size_t)length, buf);
		}

		case MSGPACK_T_MAP32:
		{
			uint32_t length;

			WARD_BYTES(4);
			swap32(buf->ptr, &length);
			buf->ptr += 4;

			return unpack_hash(_value, (size_t)length, buf);
		}

		case MSGPACK_T_RAW16:
		{
			uint16_t length;

			WARD_BYTES(2);
			swap16(buf->ptr, &length);
			buf->ptr += 2;

			*_value = MOCHILO_CB.bytes_new(buf->ptr, (size_t)length, opaque);
			break;
		}

		case MSGPACK_T_RAW32:
		{
			uint32_t length;

			WARD_BYTES(4);
			swap32(buf->ptr, &length);
			buf->ptr += 4;

			*_value = MOCHILO_CB.bytes_new(buf->ptr, (size_t)length, opaque);
			break;
		}

		default:
			return -1;
	}

#undef WARD_BYTES
}

int main()
{
  // unsigned char buffer[] = "\xCC\x80";
  // unsigned char buffer[] = "\xCD\x04\xD2";
  // unsigned char buffer[] = "\xCE\x00\x01\xE2@";
  // unsigned char buffer[] = "\xCF\x00\x00\x00\x01\x00\x00\x00\x00";
  // unsigned char buffer[] = "\xD0\x81";
  // unsigned char buffer[] = "\xD1\xFA\xF7";
  // unsigned char buffer[] = "\xD2\xFF\xFE\x1D\xC0";
  // unsigned char buffer[] = "\xD3\xFF\xFF\xFF\xFF\x00\x00\x00\x00";
  // unsigned char buffer[] = "\xC0";
  // unsigned char buffer[] = "\xC3";
  // unsigned char buffer[] = "\xC2";
  // unsigned char buffer[] = "\xCA@\x93J=";
  // unsigned char buffer[] = "\xCB@\x93J=p\xA3\xD7\n";
  // unsigned char buffer[] = "\xDA\x00\x01\x61";
  // unsigned char buffer[] = "\xDB\x00\x00\x00\x01\x61";
  // unsigned char buffer[] = "\xDC\x00\x01\241a";
  // unsigned char buffer[] = "\xDD\x00\x00\x00\x01\241a";
  // unsigned char buffer[] = "\xDE\x00\x01\241a\241a";
  // unsigned char buffer[] = "\241\x61";
  // unsigned char buffer[] = "\x91\x00";
  unsigned char buffer[] = "\x81\224\x61\x61";

  size_t len;

  len = sizeof(buffer);

  msgpacker_decode((unsigned char *)&buffer[0], len);

  return 0;
}
