## String

The String type is nearly identical to the Raw type from MessagePack but is specifically
aimed at marking a set of bytes as text.

### Format Specification

#### Symbol

For storing symbol names as ASCII text up to (2^8)-1 bytes.
Length is stored in unsigned 8-bit integer.

```
+--------+--------+----------
|  0xd4  |XXXXXXXX|...N bytes
+--------+--------+----------
=> XXXXXXXX (=N) bytes of raw bytes.
```

#### Regexp

Stores a regular expression, with options and encoding, up to (2^16)-1 bytes long.

```
+--------+--------+--------+--------+--------+--------+--------+--------+----------
|  0xd5  |XXXXXXXX|XXXXXXXX|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|ZZZZZZZZ|...N bytes
+--------+--------+--------+--------+--------+--------+--------+--------+----------
=> XXXXXXXX_XXXXXXXX (=N) bytes of raw bytes.
=> YYYYYYYY_YYYYYYYY_YYYYYYYY_YYYYYYYY 32-bit int options
=> ZZZZZZZZ encoding flag
```

#### Time

Stores a Ruby time, with usec resolution and UTC offset.

```
+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
|  0xd6  |XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|
+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
=> XXXXXXXX_XXXXXXXX_XXXXXXXX_XXXXXXXX_XXXXXXXX_XXXXXXXX_XXXXXXXX_XXXXXXXX 64-bit unsigned seconds since the epoch
=> YYYYYYYY_YYYYYYYY_YYYYYYYY_YYYYYYYY_YYYYYYYY_YYYYYYYY_YYYYYYYY_YYYYYYYY 64-bit unsigned microseconds
=> ZZZZZZZZ_ZZZZZZZZ_ZZZZZZZZ_ZZZZZZZZ 32-bit signed seconds offset from UTC
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
