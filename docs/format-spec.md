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
