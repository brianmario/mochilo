# encoding: utf-8
ROOT_DIR = File.dirname(File.dirname(__FILE__))
$LOAD_PATH.unshift "#{ROOT_DIR}/lib"

require 'test/unit'
require 'mochilo'
require 'msgpack'
require 'pp'
require 'benchmark'
require 'stringio'

class CrossPlatformTest < Test::Unit::TestCase
  CASES = [
    false, true, nil,
    0,0,0,0,0,0,0,0,0,
    -1,-1,-1,-1,-1,
    127,127,255,65535,4294967295,
    -32,-32,-128,-32768,-2147483648,
    0.0,-0.0,1.0,-1.0,
    "a","a","a",
    "","","",
    [0],[0],[0],
    [],[],[],
    {},{},{},
    {"a" => 97}, {"a" => 97}, {"a" => 97},
    [[]], [["a"]]
  ]

  def test_cross_plat
    stream = File.new("#{ROOT_DIR}/test/assets/cases.mpac")

    unpacker = Mochilo::Unpacker.new(stream)

    unpacker.each { |obj|
      pp obj
    }
  end

end


class MochiloPackTest < Test::Unit::TestCase

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
    OBJECTS.map{ |obj| Mochilo.pack(obj) }.join
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
    assert_equal all_objects, stream.read
  end

  def test_block_pack
    buffer = ""

    packer = Mochilo::Packer.new { |bytes| buffer << bytes }
    OBJECTS.each { |obj| packer << obj }
    packer.flush

    assert_equal all_objects, buffer
  end
end

class MochiloUnpackTest < Test::Unit::TestCase
  BUFFERS = [
    "\xCC\x80",
    "\xCD\x04\xD2",
    "\xCE\x00\x01\xE2@",
    "\xCF\x00\x00\x00\x01\x00\x00\x00\x00",
    "\xD0\x81",
    "\xD1\xFA\xF7",
    "\xD2\xFF\xFE\x1D\xC0",
    "\xD3\xFF\xFF\xFF\xFF\x00\x00\x00\x00",
    "\xC0",
    "\xC3",
    "\xC2",
    "\xCB@\x93J=p\xA3\xD7\n",
    "\xDA\x00\x01\x61",
    "\xDB\x00\x00\x00\x01\x61",
    "\xDC\x00\x01\241a",
    "\xDD\x00\x00\x00\x01\241a",
    "\xDE\x00\x01\241a\241a",
    "\241\x61",
    "\x91\x00",
  ]

  def test_simple_unpack
    BUFFERS.each do |buf|
      a = Mochilo.unpack(buf)
      b = Mochilo.pack(a)
      c = Mochilo.unpack(b)
      assert_equal a, c
    end
  end

  def stream_unpack

  end
end
