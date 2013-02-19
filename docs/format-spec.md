## String

The String type is nearly identical to the Raw type from MessagePack but is specifically
aimed at marking a set of bytes as text.

### Format Specification

#### Symbol

For storing symbol names as ASCII text up to (2^16)-1 bytes.
Length is stored in unsigned 16-bit big-endian integer.
Encoding is stored as an int8

```
+--------+--------+--------+----------
|  0xd4  |XXXXXXXX|XXXXXXXX|...N bytes
+--------+--------+--------+----------
=> XXXXXXXX_XXXXXXXX (=N) bytes of raw bytes.
```

#### String16

For storing text up to (2^16)-1 bytes.
Length is stored in unsigned 16-bit big-endian integer.
Encoding is stored as an int8

```
+--------+--------+--------+--------+----------
|  0xd8  |XXXXXXXX|XXXXXXXX|YYYYYYYY|...N bytes
+--------+--------+--------+--------+----------
=> XXXXXXXX_XXXXXXXX (=N) bytes of raw bytes.
=> YYYYYYYY encoding flag
```

#### String32

For storing text up to (2^32)-1 bytes.
Length is stored in unsigned 32-bit big-endian integer.
Encoding is stored as an int8

```
+--------+--------+--------+--------+--------+--------+----------
|  0xd9  |XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|YYYYYYYY|...N bytes
+--------+--------+--------+--------+--------+--------+----------
=> XXXXXXXX_XXXXXXXX_XXXXXXXX_XXXXXXXX (=N) bytes of raw bytes.
=> YYYYYYYY encoding flag
```
