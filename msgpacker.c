#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
TODO: make these noop if __BIG_ENDIAN__

#ifdef __BIG_ENDIAN__
#define msgpacker_swap16(b, l, o)
#define msgpacker_swap32(b, l, o)
#define msgpacker_swap64(b, l, o)
#else
*/
static inline void msgpacker_swap16(unsigned char *buffer, size_t len, void *out) {
  if (len > 1) {
    unsigned char *risky = (unsigned char *)out;
    risky[0] = buffer[1];
    risky[1] = buffer[0];
  }
}

static inline void msgpacker_swap32(unsigned char *buffer, size_t len, void *out) {
  if (len > 3) {
    unsigned char *risky = (unsigned char *)out;
    risky[0] = buffer[3];
    risky[1] = buffer[2];
    risky[2] = buffer[1];
    risky[3] = buffer[0];
  }
}

static inline void msgpacker_swap64(unsigned char *buffer, size_t len, void *out) {
  if (len > 7) {
    unsigned char *risky = (unsigned char *)out;
    risky[0] = buffer[7];
    risky[1] = buffer[6];
    risky[2] = buffer[5];
    risky[3] = buffer[4];
    risky[4] = buffer[3];
    risky[5] = buffer[2];
    risky[6] = buffer[1];
    risky[7] = buffer[0];
  }
}
/* #endif */

#define msgpacker__decode_uint16 msgpacker_swap16
#define msgpacker__decode_int16  msgpacker_swap16
#define msgpacker__decode_uint32 msgpacker_swap32
#define msgpacker__decode_int32  msgpacker_swap32
#define msgpacker__decode_uint64 msgpacker_swap64
#define msgpacker__decode_int64  msgpacker_swap64
#define msgpacker__decode_float  msgpacker_swap32
#define msgpacker__decode_double msgpacker_swap64

static inline void msgpacker__decode_uint8(unsigned char *buffer, size_t len, uint8_t *out) {
  if (len > 0) {
    unsigned char *risky = (unsigned char *)out;
    risky[0] = buffer[0];
  }
}

static inline void msgpacker__decode_int8(unsigned char *buffer, size_t len, int8_t *out) {
  if (len > 0) {
    unsigned char *risky = (unsigned char *)out;
    risky[0] = buffer[0];
  }
}

static inline void msgpacker__decode_raw16(unsigned char *buffer, size_t len, unsigned char *out, uint16_t out_len) {
  if (len >= (size_t)out_len) memcpy(out, buffer, out_len);
}

static inline void msgpacker__decode_raw32(unsigned char *buffer, size_t len, unsigned char *out, uint32_t out_len) {
  if (len >= (size_t)out_len) memcpy(out, buffer, out_len);
}

void msgpacker_decode(unsigned char *buffer, size_t len) {
  uint8_t leader;
  size_t len_left;

  uint8_t uint8_out;
  uint16_t uint16_out;
  uint32_t uint32_out;
  uint64_t uint64_out;
  int8_t int8_out;
  int16_t int16_out;
  int32_t int32_out;
  int64_t int64_out;
  float float_out;
  double double_out;
  unsigned char *buffer_out;

  len_left = len;

  leader = buffer[0];

  while (len_left > 0) {
    len_left--;
    buffer++;

    switch(leader) {
      case 0xcc:
        msgpacker__decode_uint8(buffer, len_left, &uint8_out);
        len_left--;
        printf("Decoded an uint8: %u\n", uint8_out);
        break;
      case 0xcd:
        msgpacker__decode_uint16(buffer, len_left, &uint16_out);
        len_left-=2;
        printf("Decoded an uint16_out: %u\n", uint16_out);
        break;
      case 0xce:
        msgpacker__decode_uint32(buffer, len_left, &uint32_out);
        len_left-=4;
        printf("Decoded an uint32_out: %u\n", uint32_out);
        break;
      case 0xcf:
        msgpacker__decode_uint64(buffer, len_left, &uint64_out);
        len_left-=8;
        printf("Decoded an uint64_out: %llu\n", uint64_out);
      case 0xd0:
        msgpacker__decode_int8(buffer, len_left, &int8_out);
        len_left--;
        printf("Decoded an int8: %d\n", int8_out);
        break;
      case 0xd1:
        msgpacker__decode_int16(buffer, len_left, &int16_out);
        len_left-=2;
        printf("Decoded an int16: %d\n", int16_out);
        break;
      case 0xd2:
        msgpacker__decode_int32(buffer, len_left, &int32_out);
        len_left-=4;
        printf("Decoded an int32_out: %d\n", int32_out);
        break;
      case 0xd3:
        msgpacker__decode_int64(buffer, len_left, &int64_out);
        len_left-=8;
        printf("Decoded an int64_out: %lld\n", int64_out);
        break;
      case 0xc0:
        buffer++;
        printf("Decoded a null\n");
        break;
      case 0xc3:
        buffer++;
        printf("Decoded a true\n");
        break;
      case 0xc2:
        buffer++;
        printf("Decoded a false\n");
        break;
      case 0xca:
        msgpacker__decode_float(buffer, len_left, &float_out);
        len_left-=4;
        printf("Decoded a float: %f\n", float_out);
        break;
      case 0xcb:
        msgpacker__decode_double(buffer, len_left, &double_out);
        len_left-=8;
        printf("Decoded a double: %f\n", double_out);
        break;
      case 0xda:
        msgpacker__decode_uint16(buffer, len_left, &uint16_out);
        len_left-=2;
        buffer+=2;

        buffer_out = malloc(uint16_out);
        msgpacker__decode_raw16(buffer, len_left, buffer_out, uint16_out);
        len_left-=uint16_out;

        printf("Decoded a raw16 %d byte(s) long: %.*s\n", uint16_out, uint16_out, buffer_out);
        free(buffer_out);
        break;
      case 0xdb:
        msgpacker__decode_uint32(buffer, len_left, &uint32_out);
        len_left-=4;
        buffer+=4;

        buffer_out = malloc(uint32_out);
        msgpacker__decode_raw32(buffer, len_left, buffer_out, uint32_out);
        len_left-=uint32_out;

        printf("Decoded a raw32 %d byte(s) long: %.*s\n", uint32_out, uint32_out, buffer_out);
        free(buffer_out);
        break;
      case 0xdc:
        msgpacker__decode_uint16(buffer, len_left, &uint16_out);
        len_left-=2;
        buffer+=2;

        /* TODO: loop over `uint16_out` times */
        len_left = 0;

        printf("Decoded an array16 %d item(s) long\n", uint16_out);
        break;
      case 0xdd:
        msgpacker__decode_uint32(buffer, len_left, &uint32_out);
        len_left-=2;
        buffer+=2;

        /* TODO: loop over `uint32_out` times */
        len_left = 0;

        printf("Decoded an array32 %d item(s) long\n", uint32_out);
        break;
      case 0xde:
        msgpacker__decode_uint16(buffer, len_left, &uint16_out);
        len_left-=2;
        buffer+=2;

        /* TODO: loop over `uint16_out` times */
        len_left = 0;

        printf("Decoded an map16 %d item(s) long\n", uint16_out);
        break;
      case 0xdf:
        msgpacker__decode_uint32(buffer, len_left, &uint32_out);
        len_left-=2;
        buffer+=2;

        /* TODO: loop over `uint32_out` times */
        len_left = 0;

        printf("Decoded an map32 %d item(s) long\n", uint32_out);
        break;
      default:
        if ((leader & 0x80) == 0x0) {
          printf("Decoded a positive fixnum: %d\n", leader);
        } else if ((leader & 0xe0) == 0xe0) {
          printf("Decoded a negative fixnum: %d\n", (int8_t)leader);
        } else if ((leader & 0xa0) == 0xa0) {
          int8_out = leader & (~0xa0);

          if (len_left >= (size_t)int8_out) {
            buffer_out = malloc(int8_out);
            memcpy(buffer_out, buffer, int8_out);
            printf("Decoded a fixed raw %d bytes long: %.*s\n", int8_out, int8_out, buffer_out);
            len_left-=int8_out;
            buffer+=int8_out;
            free(buffer_out);
          }
        } else if ((leader & 0x90) == 0x90) {
          int8_out = leader & (~0x90);

          printf("Decoded a fixed array %d items long\n", int8_out);
          /* TODO: loop over `int8_out` times */
          len_left = 0;
        } else if ((leader & 0x80) == 0x80) {
          int8_out = leader & (~0x80);

          printf("Decoded a fixed map %d items long\n", int8_out);
          /* TODO: loop over `int8_out` times */
          len_left = 0;
        } else {
          printf("Unknown leader type of 0x%x\n", leader);
        }
    }
  }
}

int main() {
  // unsigned char buffer[2] = "\xCC\x80";
  // unsigned char buffer[3] = "\xCD\x04\xD2";
  // unsigned char buffer[5] = "\xCE\x00\x01\xE2@";
  // unsigned char buffer[9] = "\xCF\x00\x00\x00\x01\x00\x00\x00\x00";
  // unsigned char buffer[2] = "\xD0\x81";
  // unsigned char buffer[3] = "\xD1\xFA\xF7";
  // unsigned char buffer[5] = "\xD2\xFF\xFE\x1D\xC0";
  // unsigned char buffer[9] = "\xD3\xFF\xFF\xFF\xFF\x00\x00\x00\x00";
  // unsigned char buffer[1] = "\xC0";
  // unsigned char buffer[1] = "\xC3";
  // unsigned char buffer[1] = "\xC2";
  // unsigned char buffer[5] = "\xCA@\x93J=";
  // unsigned char buffer[9] = "\xCB@\x93J=p\xA3\xD7\n";
  // unsigned char buffer[4] = "\xDA\x00\x01\x61";
  // unsigned char buffer[6] = "\xDB\x00\x00\x00\x01\x61";
  // unsigned char buffer[5] = "\xDC\x00\x01\241a";
  // unsigned char buffer[7] = "\xDD\x00\x00\x00\x01\241a";
  // unsigned char buffer[7] = "\xDE\x00\x01\241a\241a";
  // unsigned char buffer[2] = "\241\x61";
  // unsigned char buffer[2] = "\x91\x00";
  unsigned char buffer[4] = "\x81\224\x61\x61";

  size_t len;

  len = sizeof(buffer);

  msgpacker_decode((unsigned char *)&buffer[0], len);

  return 0;
}
