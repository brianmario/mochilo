# Mochilo

Mochilo is a Ruby library implementing the BananaPack protocol. BananaPack is a superset
of MessagePack. It adds three new types to the protocol: Symbol and String in 16 and 32 bit
lengths.

Check out docs/format-spec.md for detailed information on the differences between BananaPack
and MessagePack.

## Supported Ruby Types

The following Ruby types are supported. Meaning they will be deserialized into
the same Ruby type they were before serialization.

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
