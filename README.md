# Mochilo

Mochilo is a Ruby library implementing the BananaPack protocol. BananaPack is a superset of MessagePack. It adds three new types to the protocol: Symbol and String in 16 and 32 bit lengths.

The Symbol type is a String of text composed of ASCII characters with a maximum length of (2^8)-1 bytes.

The String16 and String32 types are exactly the same as the [Raw16](http://wiki.msgpack.org/display/MSGPACK/Format+specification#Formatspecification-raw16) and [Raw32](http://wiki.msgpack.org/display/MSGPACK/Format+specification#Formatspecification-raw32) types from MessagePack except there is an encoding flag stored along with the bytes. This allows the format to differentiate binary from text.

Check out docs/format-spec.md for more detailed information on the differences between BananaPack and MessagePack.

## Supported Ruby Types

The following Ruby types are supported. Meaning they will be deserialized into the same Ruby type they were before serialization.

* Fixnum
* Bignum
* Float
* Symbol
* String (with encoding)
* nil
* true
* false
* Array
* Hash
