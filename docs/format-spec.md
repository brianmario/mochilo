## Timestamp

### Format Specification

Used for representing a timestamp.
The timestamp is stored as an unsigned 64-bit big-endian integer - the number of milliseconds since epoch.

```
+--------+--------+--------+--------+--------+--------+--------+--------+--------+
|  0xc4  |XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|
+--------+--------+--------+--------+--------+--------+--------+--------+--------+
=> unsigned 64-bit big-endian XXXXXXXX_XXXXXXXX_XXXXXXXX_XXXXXXXX_XXXXXXXX_XXXXXXXX_XXXXXXXX_XXXXXXXX
```

## String

The String type is nearly identical to the Raw type from MessagePack but is specifically
aimed at marking a set of bytes as text.

### Format Specification


#### String16

For storing text up to (2^16)-1 bytes.
Length is stored in unsigned 16-bit big-endian integer.
Encoding is stored in a signed

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
Encoding is stored in a signed

```
+--------+--------+--------+--------+--------+--------+----------
|  0xd9  |XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|YYYYYYYY|...N bytes
+--------+--------+--------+--------+--------+--------+----------
=> XXXXXXXX_XXXXXXXX_XXXXXXXX_XXXXXXXX (=N) bytes of raw bytes.
=> YYYYYYYY encoding flag
```

## Snappy

The Snappy type is sort of a superset of the Raw or String types in BananaPack, but the
contents are compressed with [Snappy](http://code.google.com/p/snappy). It's meant to be
used transparently when a Raw or String type is larger than a configured size.

*Needs to be discussed*

The contents of the compressed content will either be a Raw16, Raw32, String16 or String32
including it's leader byte in order for the parser to begin parsing that type as if it were
inline in the stream uncompressed at that position.

*Needs to be discussed*

### Format Specification

#### Snappy16

*TODO*

#### Snappy32

*TODO*
