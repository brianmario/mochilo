# Why BananaPack?

It's actually quite simple. We need to be able to serialize data while being able to differentiate between text and binary.

There are many, many other serialization formats out there. Some require a schema be defined up-front, while others can be parsed  with no prior knowledge of the structure of the serialized data. Some are extremely verbose and harder to parse while others are compact and efficient to parse.

A couple of them that are comparable to BananaPack are MessagePack and JSON. To help explain why we created BananaPack, let's answer a few questions about those two first.

## Why not JSON?

JSON has the basic principles of a serialized format that we want. It's only capable of representing primitive data types in common with almost every language. That makes it great for cross-language communication.

The problem is that it's just text. Unicode text to be precise. That means you can't serialize arbitrary binary data without encoding it first. Common practice is to Base64 encode the data first. But then deserialization side *has* to know to decode it after parsing. There's no need for hacks like this.

## Why not MessagePack?

MessagePack is nearly perfect for what we want. But it can only differentiate between binary data
and text that is encoded as UTF-8. We need to support more encodings than that so we can serialize
it over the wire without the need to put it through potentially lossy transcoding.

## BananaPack

We wanted something compact and easy to parse. But it also needed to have native support for differentiating between text and binary data.

Knowing what encoding text is in is just as important as knowing the timezone of a timestamp. This has been true since the birth of the Internet.

So we used the new MessagePack spec to add Enc8, Enc16 and Enc32 types. The encoding of the string is stored as a flag in the `type` field of the `ext` type.
