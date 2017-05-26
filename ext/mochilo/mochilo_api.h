
MOAPI mo_value moapi_bytes_new(const char *src, size_t len)
{
	return (mo_value)rb_str_new(src, len);
}

MOAPI mo_value moapi_symbol_new(const char *src, size_t len)
{
	char *name = malloc(len + 1);
	VALUE symbol;
	strncpy(name, src, len);
	name[len] = '\0';
	symbol = ID2SYM(rb_intern(name));
	free(name);
	return symbol;
}

MOAPI mo_value moapi_regexp_new(const char *src, size_t len, enum msgpack_enc_t encoding, int reg_options)
{
	int index = 0;
	VALUE re;

	if (encoding < sizeof(mochilo_enc_lookup)/sizeof(mochilo_enc_lookup[0]))
		index = rb_enc_find_index(mochilo_enc_lookup[encoding]);

	re = rb_reg_new(src, len, reg_options);
	rb_enc_set_index(re, index);

	return (mo_value)re;
}

MOAPI mo_value moapi_str_new(const char *src, size_t len, enum msgpack_enc_t encoding)
{
	int index = 0;
	VALUE str;

	if (encoding < sizeof(mochilo_enc_lookup)/sizeof(mochilo_enc_lookup[0]))
		index = rb_enc_find_index(mochilo_enc_lookup[encoding]);

	str = rb_str_new(src, len);
	rb_enc_set_index(str, index);

	return (mo_value)str;
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

MOAPI mo_value moapi_uint64_new(uint64_t value)
{
	return (mo_value)rb_ull2inum((long long)value);
}

MOAPI mo_value moapi_int64_new(int64_t value)
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


#define moapi_uint8_new(n) (mo_value)INT2FIX((int)n)
#define moapi_uint16_new(n) (mo_value)INT2FIX((int)n)
#define moapi_uint32_new(n) (mo_value)ULONG2NUM((unsigned long)n)

#define moapi_int8_new(n) (mo_value)INT2FIX((int)n)
#define moapi_int16_new(n) (mo_value)INT2FIX((int)n)
#define moapi_int32_new(n) (mo_value)LONG2FIX((long)n)

#define moapi_float_new moapi_double_new
