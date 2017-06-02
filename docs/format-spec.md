## Enc

The Enc type is nearly identical to the Str type from the MessagePack spec but has a flag for specifying an encoding other than UTF-8.

### Format Specification

#### Enc8

For serializing text up to (2^8)-1 bytes.

```
+--------+--------+--------+========+
|  0xc7  |XXXXXXXX|  type  |  data  |
+--------+--------+--------+========+
=> XXXXXXXX - a 8-bit unsigned integer which represents the legth of data.
=> type - encoding flag, a signed 8-bit signed integer
```

#### Enc16

For serializing text up to (2^16)-1 bytes.

```
+--------+--------+--------+--------+========+
|  0xc8  |YYYYYYYY|YYYYYYYY|  type  |  data  |
+--------+--------+--------+--------+========+
=> YYYYYYYY_YYYYYYYY - a 16-bit big-endian unsigned integer which represents the legth of data.
=> type - encoding flag, a signed 8-bit signed integer
```

#### Enc32

For serializing text up to (2^32)-1 bytes.

```
+--------+--------+--------+--------+--------+--------+========+
|  0xc9  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|  type  |  data  |
+--------+--------+--------+--------+--------+--------+========+
=> ZZZZZZZZ_ZZZZZZZZ_ZZZZZZZZ_ZZZZZZZZ - a big-endian 32-bit unsigned integer which represents the legth of data.
=> type - encoding flag, a signed 8-bit signed integer
```

#### Extensions

If the encoding flag is `0xff`, the data is not a string. The first byte of the data field defines the type, and the rest of the data field is defined per type.
The length of the enclosing ENCx type is the combined length of ext type and ext data.

```
----+-----------+----------+==========+
... | type=0xff | ext type | ext data |
----+-----------+----------+==========+
```

* Symbol, ext type = 0
* Regexp, ext type = 1
* Time, ext type = 2

##### Symbol

Symbols are encoded as the symbol's name, without a NUL terminator.

```
----+-----------+----------+=============+
... | type=0xff |   0x00   | symbol name |
----+-----------+----------+=============+
```

##### Regexp

Regexps are encoded as 32-bit integer from ruby's `Regexp#options`, an 8-bit encoding type, and the text of the regexp.

```
----+-----------+----------+--------+--------+--------+--------+--------+======+
... | type=0xff |   0x01   |XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|YYYYYYYY| data |
----+-----------+----------+--------+--------+--------+--------+--------+======+
XXXXXXXX_XXXXXXXX_XXXXXXXX_XXXXXXXX 32-bit unsigned options
YYYYYYYY encoding flag
```

##### Time

Time values are encoded as a 64-bit unsigned number of seconds since the epoch, a 64-bit unsigned number of usec, and a 32-bit signed seconds of offset from UTC.

```
----+-----------+----------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
... | type=0xff |   0x02   |XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|
----+-----------+----------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
XXXXXXXX... whole seconds since epoch
YYYYYYYY... usec fraction to add to X
ZZZZZZZZ... (signed) seconds offset from UTC
```
