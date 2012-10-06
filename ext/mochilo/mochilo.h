#ifndef __MOCHILO_H__
#define __MOCHILO_H__

#include <ctype.h>
#include "buffer.h"

#define MOAPI static inline
#define unlikely(x)     __builtin_expect((x),0)

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
	MSGPACK_T_STR16 = 0xd8, /* reserved in the spec */
	MSGPACK_T_STR32 = 0xd9, /* reserved in the spec */
	MSGPACK_T_RAW16	= 0xda,
	MSGPACK_T_RAW32	 = 0xdb,
	MSGPACK_T_ARRAY16 = 0xdc,
	MSGPACK_T_ARRAY32 = 0xdd,
	MSGPACK_T_MAP16	= 0xde,
	MSGPACK_T_MAP32	= 0xdf
};

enum msgpack_enc_t {
	MSGPACK_ENC_ASCII,
	MSGPACK_ENC_UTF_8,
	MSGPACK_ENC_UTF_16BE,
	MSGPACK_ENC_UTF_16LE,
	MSGPACK_ENC_UTF_32BE,
	MSGPACK_ENC_UTF_32LE,
	MSGPACK_ENC_SHIFT_JIS,
	MSGPACK_ENC_ISO_2022_JP,
	MSGPACK_ENC_GB18030,
	MSGPACK_ENC_Big5,
	MSGPACK_ENC_EUC_JP,
	MSGPACK_ENC_EUC_KR,
	MSGPACK_ENC_ISO_8859_1,
	MSGPACK_ENC_ISO_8859_2,
	MSGPACK_ENC_ISO_8859_5,
	MSGPACK_ENC_ISO_8859_6,
	MSGPACK_ENC_ISO_8859_7,
	MSGPACK_ENC_ISO_8859_8,
	MSGPACK_ENC_ISO_8859_9,
	MSGPACK_ENC_WIN_1251,
	MSGPACK_ENC_WIN_1256,
	MSGPACK_ENC_KOI8_R
};

enum msgpack_err_t {
	MSGPACK_EEOF = -1,
	MSGPACK_EINVALID = -2,
	MSGPACK_ENOTHING = -3,
};

typedef void * mo_value;
typedef uint64_t mo_integer;
int mochilo_unpack_one(mo_value *_value, mochilo_src *src);

#endif
