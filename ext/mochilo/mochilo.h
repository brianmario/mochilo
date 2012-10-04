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
	MSGPACK_T_RAW16	= 0xda,
	MSGPACK_T_RAW32	 = 0xdb,
	MSGPACK_T_ARRAY16 = 0xdc,
	MSGPACK_T_ARRAY32 = 0xdd,
	MSGPACK_T_MAP16	= 0xde,
	MSGPACK_T_MAP32	= 0xdf
};

typedef void * mo_value;
typedef uint64_t mo_integer;

struct mochilo_parse_buf {
	const uint8_t *ptr;
	const uint8_t *end;
};

int mochilo_unpack_one(mo_value *_value, struct mochilo_parse_buf *buf, void *opaque);

#endif
