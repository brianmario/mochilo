# Mochilo

Mochilo is a Ruby library implementing the BananaPack protocol. BananaPack is a superset
of MessagePack. It adds four new types to the protocol: String in 16 and 32 bit lengths
and Snappy in 16 and 32 bit lengths.

Check out docs/format-spec.md for detailed information on the differences between BananaPack
and MessagePack.

## Supported Ruby Types

The following Ruby types are supported

* Fixnum
* Bignum
* Float
* String (with encoding)
* nil
* true
* false
* Array
* Hash

NOTE: `Symbol` objects will be treated as strings while packing. They'll come back into Ruby as String objects while unpacking.
