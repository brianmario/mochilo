#define RSTRING_NOT_MODIFIED
#include <ruby.h>

#ifdef HAVE_RUBY_ENCODING_H
#include <ruby/encoding.h>
#else
#define rb_enc_copy(dst, src)
#endif

MOAPI mo_value moapi_array_new(size_t array_size, void *opaque)
{
	return (mo_value)rb_ary_new2(array_size);
}

MOAPI void moapi_array_append(mo_value array, mo_value element, void *opaque)
{
	rb_ary_push((VALUE)array, (VALUE)element);
}

MOAPI mo_value moapi_hash_new(void *opaque)
{
	return (mo_value)rb_hash_new();
}

MOAPI void moapi_hash_set(mo_value hash, mo_value key, mo_value val, void *opaque)
{
	rb_hash_aset((VALUE)hash, (VALUE)key, (VALUE)val);
}

MOAPI mo_value moapi_bytes_new(const char *bytes, size_t len, void *opaque)
{
	return (mo_value)rb_str_new(bytes, (long)len);
}

MOAPI mo_value moapi_uint64_new(uint64_t value, enum msgpack_t int_type, void *opaque)
{
	return (mo_value)rb_ull2inum((long long)value);
}

MOAPI mo_value moapi_int64_new(int64_t value, enum msgpack_t int_type, void *opaque)
{
	return (mo_value)rb_ll2inum(value);
}

MOAPI mo_value moapi_atom_new(enum msgpack_t type, void *opaque)
{
	switch (type) {
		case MSGPACK_T_NIL: return (mo_value)Qnil; 
		case MSGPACK_T_TRUE: return (mo_value)Qtrue;
		case MSGPACK_T_FALSE: return (mo_value)Qfalse;
		default: return NULL;
	}
}

MOAPI mo_value moapi_double_new(double d, void *opaque)
{
	return (mo_value)rb_float_new(d);
}


#define moapi_uint8_new moapi_uint64_new
#define moapi_uint16_new moapi_uint64_new
#define moapi_uint32_new moapi_uint64_new

#define moapi_int8_new moapi_int64_new
#define moapi_int16_new moapi_int64_new
#define moapi_int32_new moapi_int64_new

#define moapi_float_new moapi_double_new
