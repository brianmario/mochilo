# Mochilo

Mochilo is a Ruby library implementing the BananaPack protocol. BananaPack is a superset of MessagePack. It adds three new types to the protocol: Symbol and String in 16 and 32 bit lengths.

The Symbol type is a String of text composed of ASCII characters with a maximum length of (2^8)-1 bytes.

The String16 and String32 types are exactly the same as the [Raw16](http://wiki.msgpack.org/display/MSGPACK/Format+specification#Formatspecification-raw16) and [Raw32](http://wiki.msgpack.org/display/MSGPACK/Format+specification#Formatspecification-raw32) types from MessagePack except there is an encoding flag stored along with the bytes. This allows the format to differentiate binary from text.

Check out docs/format-spec.md for more detailed information on the differences between BananaPack and MessagePack.

## Usage

``` ruby
require 'mochilo'
obj = {key: "value"}
bpack = Mochilo.pack(obj)
#=> "\x81\xD8\x00\x03\x01key\xD8\x00\x05\x00value"

hash = Mochilo.unpack(bpack)
#=> {"key"=>"value"}
```

Notice how `key` came back into Ruby as a String instead of a Symbol? This is because the `pack` method of Mochilo only generates "safe" bpack.

bpack without Symbols is considered "safe" for Ruby because in Ruby, Symbols aren't garbage collected. So if you unpack arbitrary bpack from an untrusted or malicious source, you could end up potentially exhausting the memory of your server.

To generate "unsafe" bpack, use `pack_unsafe` and `unpack_unsafe` methods instead:

``` ruby
require 'mochilo'
obj = {key: "value"}
bpack = Mochilo.pack_unsafe(obj)
#=> "\x81\xD4\x03key\xD8\x00\x05\x00value"

hash = Mochilo.unpack_unsafe(bpack)
#=> {:key=>"value"}
```

If you attempt to unpack "unsafe" bpack without using `unpack_unsafe`, an exception is raised.

## Supported Ruby Types

The following Ruby types are supported. Meaning they will be deserialized into the same Ruby type they were before serialization.

If any other object type is encountered during serialization, an exception is raised. This is to ensure you have explicit control over what is being serialized.

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
