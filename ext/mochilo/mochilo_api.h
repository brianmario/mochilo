#define RSTRING_NOT_MODIFIED
#include <ruby.h>

#ifdef HAVE_RUBY_ENCODING_H
#include <ruby/encoding.h>
#else
#define rb_enc_copy(dst, src)
#endif

static const char *mochilo_enc_lookup[] = {
	"UTF-8",
	"US-ASCII",
	"Big5",
	"Big5-HKSCS",
	"Big5-UAO",
	"CP949",
	"Emacs-Mule",
	"EUC-JP",
	"EUC-KR",
	"EUC-TW",
	"GB18030",
	"GBK",
	"ISO-8859-1",
	"ISO-8859-2",
	"ISO-8859-3",
	"ISO-8859-4",
	"ISO-8859-5",
	"ISO-8859-6",
	"ISO-8859-7",
	"ISO-8859-8",
	"ISO-8859-9",
	"ISO-8859-10",
	"ISO-8859-11",
	"ISO-8859-13",
	"ISO-8859-14",
	"ISO-8859-15",
	"ISO-8859-16",
	"KOI8-R",
	"KOI8-U",
	"Shift_JIS",
	"UTF-16BE",
	"UTF-16LE",
	"UTF-32BE",
	"UTF-32LE",
	"Windows-1251",
	"IBM437",
	"IBM737",
	"IBM775",
	"CP850",
	"IBM852",
	"CP852",
	"IBM855",
	"CP855",
	"IBM857",
	"IBM860",
	"IBM861",
	"IBM862",
	"IBM863",
	"IBM864",
	"IBM865",
	"IBM866",
	"IBM869",
	"Windows-1258",
	"GB1988",
	"macCentEuro",
	"macCroatian",
	"macCyrillic",
	"macGreek",
	"macIceland",
	"macRoman",
	"macRomania",
	"macThai",
	"macTurkish",
	"macUkraine",
	"CP950",
	"CP951",
	"stateless-ISO-2022-JP",
	"eucJP-ms",
	"CP51932",
	"GB2312",
	"GB12345",
	"ISO-2022-JP",
	"ISO-2022-JP-2",
	"CP50220",
	"CP50221",
	"Windows-1252",
	"Windows-1250",
	"Windows-1256",
	"Windows-1253",
	"Windows-1255",
	"Windows-1254",
	"TIS-620",
	"Windows-874",
	"Windows-1257",
	"Windows-31J",
	"MacJapanese",
	"UTF-7",
	"UTF8-MAC",
	"UTF-16",
	"UTF-32",
	"UTF8-DoCoMo",
	"SJIS-DoCoMo",
	"UTF8-KDDI",
	"SJIS-KDDI",
	"ISO-2022-JP-KDDI",
	"stateless-ISO-2022-JP-KDDI",
	"UTF8-SoftBank",
	"SJIS-SoftBank"
};

MOAPI int moapi_bytes_new(mo_value *value, mochilo_src *src, size_t len)
{
	VALUE buffer = rb_str_buf_new((long)len);
	uint8_t heap[512];
	int r;

	while (len > sizeof(heap)) {
		if ((r = mochilo_src_read(src, heap, sizeof(heap))) < 0)
			return -1;

		rb_str_buf_cat(buffer, heap, r);
		len -= r;
	}

	while (len) {
		if ((r = mochilo_src_read(src, heap, len)) < 0)
			return -1;

		rb_str_buf_cat(buffer, heap, r);
		len -= r;
	}

	*value = (mo_value)buffer;
	return 0;
}

MOAPI int moapi_str_new(
	mo_value *value,
	enum msgpack_enc_t encoding,
	mochilo_src *src, size_t len)
{
	int rb_enc_index;
	VALUE rb_str;

	if (moapi_bytes_new(value, src, len) < 0)
		return -1;

#ifdef HAVE_RUBY_ENCODING_H
	/* TODO: do range checking so we don't overflow a lookup */
	rb_enc_index = rb_enc_find_index(mochilo_enc_lookup[encoding]);
	rb_enc_set_index((VALUE)*value, rb_enc_index);
#endif
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
