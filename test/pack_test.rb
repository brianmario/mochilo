# encoding: ascii-8bit
require File.expand_path('../setup', __FILE__)

require 'mochilo'
require 'stringio'

class MochiloPackTest < MiniTest::Unit::TestCase

  OBJECTS = [
    {"hello" => "world"},
    12345,
    -12345,
    "hey this is a test",
    0.231
  ]

  def bytes(a)
    a.bytes.to_a.map { |b| "0x" + b.to_s(16) }.join(' ')
  end

  def all_objects
    objects = OBJECTS.map{ |obj| Mochilo.pack(obj) }.join
    if objects.respond_to?(:encoding)
      objects.force_encoding('binary')
    end
    objects
  end

  def test_simple_pack
    OBJECTS.each do |obj|
      a = Mochilo.pack(obj)
      b = Mochilo.unpack(a)
      assert_equal obj, b
    end
  end

  def test_stream_pack
    stream = StringIO.new

    packer = Mochilo::Packer.new(stream)
    OBJECTS.each { |obj| packer << obj }
    packer.flush

    stream.rewind
    serialized = stream.read
    if serialized.respond_to?(:encoding)
      serialized.force_encoding('binary')
    end
    assert_equal all_objects, serialized
  end

  def test_block_pack
    buffer = ""

    packer = Mochilo::Packer.new { |bytes| buffer << bytes }
    OBJECTS.each { |obj| packer << obj }
    packer.flush

    assert_equal all_objects, buffer
  end
end
