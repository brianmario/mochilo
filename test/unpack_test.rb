# encoding: utf-8
ROOT_DIR = File.dirname(File.dirname(__FILE__))
$LOAD_PATH.unshift "#{ROOT_DIR}/lib"

require 'test/unit'
require 'mochilo'
require 'msgpack'
require 'pp'
require 'benchmark'

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

  def test_simple_pack
    OBJECTS.each do |obj|
      a = Mochilo.pack(obj)
      b = Mochilo.unpack(a)
      assert_equal obj, b
    end
  end

  def test_block_pack
    buffer1 = ""
    packer = Mochilo::Packer.new { |bytes| buffer1 << bytes }
    OBJECTS.each { |obj| packer << obj }
    packer.flush

    buffer2 = OBJECTS.map{ |obj| Mochilo.pack(obj) }.join
    assert_equal buffer1, buffer2
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
    "\xCA@\x93J=",
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
      assert_equal buf, b
    end
  end

  def xtest_benchmark
    binary = IO.read("#{ROOT_DIR}/test/assets/255k.bin")
    Benchmark.bmbm do |x|
      x.report 'MessagePack' do
        100.times do
          MessagePack.unpack(binary)
        end
      end
      x.report 'Mochilo' do
        100.times do
          Mochilo.unpack(binary)
        end
      end
    end
  end
end
