# encoding: utf-8
ROOT_DIR = File.dirname(File.dirname(__FILE__))
$LOAD_PATH.unshift "#{ROOT_DIR}/lib"

require 'test/unit'
require 'mochilo'
require 'pp'
require 'benchmark'

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

  def test_benchmark
    binary = IO.read("#{ROOT_DIR}/test/assets/1mb.bin")
    puts Benchmark.measure {
      1000.times do
        Mochilo.unpack(binary)
      end
    }
  end
end
