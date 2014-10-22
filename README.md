# Mochilo

Mochilo is a Ruby library implementing the BananaPack protocol. BananaPack is a superset of MessagePack. It takes advantage of the new `ext` types to extend the protocol, adding 3 new
types which are used for serializing strings with an encoding other than UTF-8.

The mapping of the `ext` types are:

* `ext8`  - A String in an encoding other than UTF-8 who's length is between 0 and (2^8)-1 bytes.
* `ext16` - A String in an encoding other than UTF-8 who's length is between 0 and (2^16)-1 bytes.
* `ext32` - A String in an encoding other than UTF-8 who's length is between 0 and (2^32)-1 bytes.

Also check out [docs/format-spec.md](docs/format-spec.md) for more detailed information on these types.

Strings tagged as `ASCII-8BIT` are encoded as the `bin` types in the MessagePack spec. The type that's used depends on the length of the string. Check out the spec [here](https://github.com/msgpack/msgpack/blob/master/spec.md) for more information on those types.

## Usage

``` ruby
require 'mochilo'
obj = {"key" => "value"}
packed = Mochilo.pack(obj)
#=> "\x81\xD8\x00\x03\x01key\xD8\x00\x05\x00value"

hash = Mochilo.unpack(packed)
#=> {"key"=>"value"}
```

## Supported Ruby Types

The following Ruby types are supported. Meaning they will be deserialized into the same Ruby type they were before serialization.

If any other object type is encountered during serialization, an exception is raised. This is to ensure you have explicit control over what is being serialized.

* Fixnum
* Bignum
* Float
* String (with encoding)
* nil
* true
* false
* Array
* Hash
