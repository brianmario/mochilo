#define RSTRING_NOT_MODIFIED
#include <ruby.h>

#ifdef HAVE_RUBY_ENCODING_H
#include <ruby/encoding.h>
#else
#define rb_enc_copy(dst, src)
#endif

MOAPI int moapi_bytes_new(mo_value *value, mochilo_src *src, size_t len)
{
	VALUE buffer = rb_str_buf_new((long)len);
	uint8_t heap[512];
	int r;

	while (len > sizeof(heap)) {
		if ((r = mochilo_src_read(src, heap, sizeof(heap))) < 0)
			return -1;

		rb_str_buf_cat(buffer, &heap[0], r);
		len -= r;
	}

	while (len) {
		if ((r = mochilo_src_read(src, heap, len)) < 0)
			return -1;

		rb_str_buf_cat(buffer, &heap[0], r);
		len -= r;
	}

	*value = (mo_value)buffer;
	return 0;
}

MOAPI int moapi_str_new(mo_value *value, enum msgpack_enc_t encoding, mochilo_src *src, size_t len)
{
	int err;

	err = moapi_bytes_new(value, src, len);
	if (err < 0)
		return err;

	/* TODO: tag string with encoding */

	return 0;
}

MOAPI mo_value moapi_array_new(size_t array_size)
{
	return (mo_value)rb_ary_new2(array_size);
}

MOAPI void moapi_array_append(mo_value array, mo_value element)
{
	rb_ary_push((VALUE)array, (VALUE)element);
}

MOAPI mo_value moapi_hash_new(void)
{
	return (mo_value)rb_hash_new();
}

MOAPI void moapi_hash_set(mo_value hash, mo_value key, mo_value val)
{
	rb_hash_aset((VALUE)hash, (VALUE)key, (VALUE)val);
}

MOAPI mo_value moapi_uint64_new(uint64_t value, enum msgpack_t int_type)
{
	return (mo_value)rb_ull2inum((long long)value);
}

MOAPI mo_value moapi_int64_new(int64_t value, enum msgpack_t int_type)
{
	return (mo_value)rb_ll2inum(value);
}

MOAPI mo_value moapi_atom_new(enum msgpack_t type)
{
	switch (type) {
		case MSGPACK_T_NIL: return (mo_value)Qnil; 
		case MSGPACK_T_TRUE: return (mo_value)Qtrue;
		case MSGPACK_T_FALSE: return (mo_value)Qfalse;
		default: return NULL;
	}
}

MOAPI mo_value moapi_double_new(double d)
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
