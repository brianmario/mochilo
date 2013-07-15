# Why BananaPack?

It's actually quite simple. We need to be able to serialize data while being able to differentiate between text and binary.

There are many, many other serialization formats out there. Some require a schema be defined up-front, while others can be parsed  with no prior knowledge of the structure of the serialized data. Some are extremely verbose and harder to parse while others are compact and efficient to parse.

A couple of them that are comparable to BananaPack are MessagePack and JSON. To help explain why we created BananaPack, let's answer a few questions about those two first.

## Why not JSON?

JSON has the basic principles of a serialized format that we want. It's only capable of representing primitive data types in common with almost every language. That makes it great for cross-language communication.

The problem is that it's just text. Unicode text to be precise. That means you can't serialize arbitrary binary data without encoding it first. Common practice is to Base64 encode the data first. But then deserialization side *has* to know to unencode it after parsing. There's no need for hacks like this.

## Why not MessagePack?

MessagePack is nearly perfect for what we want. But text and binary share a single type in the spec, "Raw". That means when deserializing the data you could be reading a contents of a Markdown file, or the contents of a JPEG. There are countless hacks to get around this but there shouldn't be a need for them either.

## BananaPack

We wanted something compact and easy to parse. But it also needed to have native support for differentiating between text and binary data.

Knowing what encoding text is in is just as important as knowing the timezone of a timestamp. This has been true since the birth of the Internet.

So we used a couple of the "reserved" data type slots in the MessagePack spec to add a String16, String32 and Symbol type. String16 and String32 are exactly the same as the Raw16 and Raw32 types from the MessagePack spec except they have an encoding flag as well. This means that it's possible to serialize text data without hacks. This also means it's possible to serialize the contents of a JPEG without first needing to encode or escape it.