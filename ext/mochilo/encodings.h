/* C code produced by gperf version 3.0.3 */
/* Command-line: gperf  */
/* Computed positions: -k'3-5,7,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

struct mochilo_enc_map { const char *name; uint8_t id; };
/* maximum key range = 326, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
mochilo_encoding_hash (str, len)
     register const char *str;
     register unsigned int len;
{
  static const unsigned short asso_values[] =
    {
      336, 336, 336, 336, 336, 336, 336, 336, 336, 336,
      336, 336, 336, 336, 336, 336, 336, 336, 336, 336,
      336, 336, 336, 336, 336, 336, 336, 336, 336, 336,
      336, 336, 336, 336, 336, 336, 336, 336, 336, 336,
      336, 336, 336, 336, 336,   5, 336, 336,  40,  50,
       30,  55,  90,   5,   0,  60,  10,  80, 336, 336,
      336, 336, 336, 336, 336,  10,  10,  15,   0,   0,
       90,   0, 336, 105,  12, 110,   0,   0, 336,   5,
       30, 336,  10,   0,   0,  35, 336,  17, 336, 336,
      336, 336, 336, 336, 336, 336, 336,   0, 336,   0,
        0,   5,  85,   0,   0,  70, 336,   0,  20,   5,
        0,   0, 336, 336,  12,   0,   0,   0, 336, 336,
      336,  70, 336, 336, 336, 336, 336, 336, 336, 336,
      336, 336, 336, 336, 336, 336, 336, 336, 336, 336,
      336, 336, 336, 336, 336, 336, 336, 336, 336, 336,
      336, 336, 336, 336, 336, 336, 336, 336, 336, 336,
      336, 336, 336, 336, 336, 336, 336, 336, 336, 336,
      336, 336, 336, 336, 336, 336, 336, 336, 336, 336,
      336, 336, 336, 336, 336, 336, 336, 336, 336, 336,
      336, 336, 336, 336, 336, 336, 336, 336, 336, 336,
      336, 336, 336, 336, 336, 336, 336, 336, 336, 336,
      336, 336, 336, 336, 336, 336, 336, 336, 336, 336,
      336, 336, 336, 336, 336, 336, 336, 336, 336, 336,
      336, 336, 336, 336, 336, 336, 336, 336, 336, 336,
      336, 336, 336, 336, 336, 336, 336, 336, 336, 336,
      336, 336, 336, 336, 336, 336
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[6]];
      /*FALLTHROUGH*/
      case 6:
      case 5:
        hval += asso_values[(unsigned char)str[4]];
      /*FALLTHROUGH*/
      case 4:
        hval += asso_values[(unsigned char)str[3]];
      /*FALLTHROUGH*/
      case 3:
        hval += asso_values[(unsigned char)str[2]];
        break;
    }
  return hval + asso_values[(unsigned char)str[len - 1]];
}

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


#ifdef __GNUC__
__inline
#ifdef __GNUC_STDC_INLINE__
__attribute__ ((__gnu_inline__))
#endif
#endif
static const struct mochilo_enc_map *
mochilo_encoding_to_id (str, len)
     register const char *str;
     register unsigned int len;
{
  enum
    {
      TOTAL_KEYWORDS = 99,
      MIN_WORD_LENGTH = 3,
      MAX_WORD_LENGTH = 26,
      MIN_HASH_VALUE = 10,
      MAX_HASH_VALUE = 335
    };

  static const struct mochilo_enc_map wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
      {"macTurkish", MSGPACK_ENC_MACTURKISH},
      {""},
      {"Windows-1256", MSGPACK_ENC_WINDOWS_1256},
      {""},
      {"Big5", MSGPACK_ENC_BIG5},
      {"Emacs-Mule", MSGPACK_ENC_EMACS_MULE},
      {"IBM866", MSGPACK_ENC_IBM866},
      {"Windows-1255", MSGPACK_ENC_WINDOWS_1255},
      {"macRoman", MSGPACK_ENC_MACROMAN},
      {""},
      {"macRomania", MSGPACK_ENC_MACROMANIA},
      {"IBM865", MSGPACK_ENC_IBM865},
      {"Windows-1258", MSGPACK_ENC_WINDOWS_1258},
      {"Windows-31J", MSGPACK_ENC_WINDOWS_31J},
      {""},
      {"macGreek", MSGPACK_ENC_MACGREEK},
      {"IBM855", MSGPACK_ENC_IBM855},
      {""},
      {"MacJapanese", MSGPACK_ENC_MACJAPANESE},
      {""},
      {"CP855", MSGPACK_ENC_CP855},
      {"macCentEuro", MSGPACK_ENC_MACCENTEURO},
      {""},
      {"Big5-UAO", MSGPACK_ENC_BIG5_UAO},
      {""},
      {"ISO-8859-6", MSGPACK_ENC_ISO_8859_6},
      {"ISO-8859-16", MSGPACK_ENC_ISO_8859_16},
      {""},
      {"macCroatian", MSGPACK_ENC_MACCROATIAN},
      {""},
      {"ISO-8859-5", MSGPACK_ENC_ISO_8859_5},
      {"ISO-8859-15", MSGPACK_ENC_ISO_8859_15},
      {"Windows-1252", MSGPACK_ENC_WINDOWS_1252},
      {"EUC-TW", MSGPACK_ENC_EUC_TW},
      {""},
      {"ISO-8859-8", MSGPACK_ENC_ISO_8859_8},
      {"IBM862", MSGPACK_ENC_IBM862},
      {""}, {""}, {""},
      {"macUkraine", MSGPACK_ENC_MACUKRAINE},
      {"IBM852", MSGPACK_ENC_IBM852},
      {"Windows-1250", MSGPACK_ENC_WINDOWS_1250},
      {""}, {""},
      {"eucJP-ms", MSGPACK_ENC_EUCJP_MS},
      {"IBM860", MSGPACK_ENC_IBM860},
      {""}, {""}, {""}, {""},
      {"stateless-ISO-2022-JP", MSGPACK_ENC_STATELESS_ISO_2022_JP},
      {"Windows-1251", MSGPACK_ENC_WINDOWS_1251},
      {""}, {""},
      {"ISO-8859-2", MSGPACK_ENC_ISO_8859_2},
      {"IBM861", MSGPACK_ENC_IBM861},
      {"Windows-1253", MSGPACK_ENC_WINDOWS_1253},
      {"EUC-JP", MSGPACK_ENC_EUC_JP},
      {""}, {""},
      {"IBM863", MSGPACK_ENC_IBM863},
      {"Windows-1257", MSGPACK_ENC_WINDOWS_1257},
      {""}, {""}, {""},
      {"ISO-8859-10", MSGPACK_ENC_ISO_8859_10},
      {""}, {""}, {""},
      {"CP852", MSGPACK_ENC_CP852},
      {"IBM857", MSGPACK_ENC_IBM857},
      {""}, {""}, {""},
      {"ISO-8859-1", MSGPACK_ENC_ISO_8859_1},
      {"ISO-8859-11", MSGPACK_ENC_ISO_8859_11},
      {""}, {""}, {""},
      {"ISO-8859-3", MSGPACK_ENC_ISO_8859_3},
      {"ISO-8859-13", MSGPACK_ENC_ISO_8859_13},
      {"TIS-620", MSGPACK_ENC_TIS_620},
      {""}, {""},
      {"ISO-8859-7", MSGPACK_ENC_ISO_8859_7},
      {"IBM869", MSGPACK_ENC_IBM869},
      {""}, {""}, {""},
      {"CP850", MSGPACK_ENC_CP850},
      {"Windows-874", MSGPACK_ENC_WINDOWS_874},
      {"Windows-1254", MSGPACK_ENC_WINDOWS_1254},
      {""}, {""}, {""},
      {"IBM864", MSGPACK_ENC_IBM864},
      {""}, {""}, {""}, {""},
      {"ISO-2022-JP", MSGPACK_ENC_ISO_2022_JP},
      {""},
      {"ISO-2022-JP-2", MSGPACK_ENC_ISO_2022_JP_2},
      {""},
      {"ISO-8859-9", MSGPACK_ENC_ISO_8859_9},
      {"UTF8-DoCoMo", MSGPACK_ENC_UTF8_DOCOMO},
      {""},
      {"UTF8-SoftBank", MSGPACK_ENC_UTF8_SOFTBANK},
      {""},
      {"UTF-8", MSGPACK_ENC_UTF_8},
      {"SJIS-DoCoMo", MSGPACK_ENC_SJIS_DOCOMO},
      {""},
      {"SJIS-SoftBank", MSGPACK_ENC_SJIS_SOFTBANK},
      {""},
      {"ISO-8859-4", MSGPACK_ENC_ISO_8859_4},
      {"ISO-8859-14", MSGPACK_ENC_ISO_8859_14},
      {""}, {""}, {""},
      {"Big5-HKSCS", MSGPACK_ENC_BIG5_HKSCS},
      {"IBM775", MSGPACK_ENC_IBM775},
      {""}, {""}, {""},
      {"macIceland", MSGPACK_ENC_MACICELAND},
      {"KOI8-R", MSGPACK_ENC_KOI8_R},
      {""},
      {"UTF8-MAC", MSGPACK_ENC_UTF8_MAC},
      {""}, {""},
      {"stateless-ISO-2022-JP-KDDI", MSGPACK_ENC_STATELESS_ISO_2022_JP_KDDI},
      {""}, {""}, {""}, {""},
      {"EUC-KR", MSGPACK_ENC_EUC_KR},
      {"macThai", MSGPACK_ENC_MACTHAI},
      {""}, {""}, {""},
      {"UTF-16", MSGPACK_ENC_UTF_16},
      {"GB12345", MSGPACK_ENC_GB12345},
      {"UTF-16LE", MSGPACK_ENC_UTF_16LE},
      {""}, {""},
      {"GB1988", MSGPACK_ENC_GB1988},
      {""},
      {"UTF-32LE", MSGPACK_ENC_UTF_32LE},
      {""}, {""},
      {"KOI8-U", MSGPACK_ENC_KOI8_U},
      {"CP50220", MSGPACK_ENC_CP50220},
      {"UTF-16BE", MSGPACK_ENC_UTF_16BE},
      {""}, {""},
      {"macCyrillic", MSGPACK_ENC_MACCYRILLIC},
      {""},
      {"UTF-32BE", MSGPACK_ENC_UTF_32BE},
      {""},
      {"CP950", MSGPACK_ENC_CP950},
      {"GB2312", MSGPACK_ENC_GB2312},
      {""}, {""}, {""}, {""},
      {"Shift_JIS", MSGPACK_ENC_SHIFT_JIS},
      {""}, {""}, {""}, {""},
      {"IBM737", MSGPACK_ENC_IBM737},
      {"CP50221", MSGPACK_ENC_CP50221},
      {""}, {""}, {""},
      {"UTF-32", MSGPACK_ENC_UTF_32},
      {"GB18030", MSGPACK_ENC_GB18030},
      {""}, {""},
      {"CP951", MSGPACK_ENC_CP951},
      {"ISO-2022-JP-KDDI", MSGPACK_ENC_ISO_2022_JP_KDDI},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
      {"CP51932", MSGPACK_ENC_CP51932},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {"IBM437", MSGPACK_ENC_IBM437},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {"UTF8-KDDI", MSGPACK_ENC_UTF8_KDDI},
      {"UTF-7", MSGPACK_ENC_UTF_7},
      {""}, {""},
      {"GBK", MSGPACK_ENC_GBK},
      {"SJIS-KDDI", MSGPACK_ENC_SJIS_KDDI},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {"US-ASCII", MSGPACK_ENC_US_ASCII},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
      {"CP949", MSGPACK_ENC_CP949}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = mochilo_encoding_hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
