# encoding: ascii-8bit
require File.expand_path('../setup', __FILE__)

require 'mochilo'
require 'stringio'

class MochiloPackTest < Minitest::Test

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

  def test_pack_nil
    assert_equal "\xC0", Mochilo.pack(nil)
  end

  def test_pack_false
    assert_equal "\xC2", Mochilo.pack(false)
  end

  def test_pack_true
    assert_equal "\xC3", Mochilo.pack(true)
  end

  def xtest_pack_float
    # ruby uses double's internally so we can't reliably
    # test for a float without some hacks
  end

  def test_pack_double
    assert_equal "\xCB@^\xDC\xCC\xCC\xCC\xCC\xCD", Mochilo.pack(123.45)
  end

  def test_pack_positive_fixed
    assert_equal "\x15", Mochilo.pack(21)
  end

  def test_pack_negative_fixed
    assert_equal "\xEB", Mochilo.pack(-21)
  end

  def test_pack_uint8
    assert_equal "\xCC\xD6", Mochilo.pack(214)
  end

  def test_pack_uint16
    assert_equal "\xCDS\xE2", Mochilo.pack(21474)
  end

  def test_pack_uint32
    assert_equal "\xCE\x7F\xFF\xFF\xFF", Mochilo.pack(2147483647)
  end

  def test_pack_uint64
    assert_equal "\xCF\x00\x00\x00\x04\xFF\xFF\xFF\xFF", Mochilo.pack(21474836479)
  end

  def test_pack_int8
    assert_equal "\xD0\xDE", Mochilo.pack(-34)
  end

  def test_pack_int16
    assert_equal "\xD1\xAC\x1E", Mochilo.pack(-21474)
  end

  def test_pack_int32
    assert_equal "\xD2\x80\x00\x00\x01", Mochilo.pack(-2147483647)
  end

  def test_pack_int64
    assert_equal "\xD3\xFF\xFF\xFF\xFB\x00\x00\x00\x01", Mochilo.pack(-21474836479)
  end

  def test_pack_str16
    str = "this is a test".force_encoding('UTF-8')
    assert_equal "\xD8\x00\x0E\x00#{str}", Mochilo.pack(str)
  end

  def xtest_pack_str32
    # TODO: not sure how to test this without making a massive 66k string
  end

  def test_pack_fixed_raw
    str = "this is a test"
    assert_equal "\xAE#{str}", Mochilo.pack(str)
  end

  def test_pack_raw16
    str = ("a"*255)
    assert_equal "\xDA\x00\xFF#{str}", Mochilo.pack(str)
  end

  def xtest_pack_raw32
    # TODO: not sure how to test this without making a massive 66k string
  end

  def test_pack_fixed_array
    assert_equal "\x90", Mochilo.pack([])
    assert_equal "\x91\x01", Mochilo.pack([1])
  end

  def test_pack_array16
    bytes = ("a"*34).bytes.to_a
    assert_equal "\xDC\x00\"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", Mochilo.pack(bytes)
  end

  def xtest_pack_array32
    # TODO: not sure how to test this without making a massive 66k item array
  end

  def test_pack_fixed_map
    assert_equal "\x80", Mochilo.pack({})
    assert_equal "\x81\x01\x02", Mochilo.pack({1 => 2})
  end

  def test_pack_symbol
    assert_equal "\xD8\x00\x04\x01test", Mochilo.pack(:test)
    assert_equal "\xD4\x04test", Mochilo.pack_unsafe(:test)
  end

  def test_pack_symbol_size
    too_big = ("a"*0x100).to_sym
    fine = ("a"*0xff).to_sym

    assert_raises Mochilo::PackError do
      Mochilo.pack_unsafe(too_big)
    end

    begin
      Mochilo.pack_unsafe(fine)
    rescue Mochilo::PackError => boom
      assert_nil boom, "exception raised, expected nothing"
    end
  end

  def test_pack_map16
    bytes = ("a"*34).bytes.to_a
    assert_equal "\xDC\x00\"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", Mochilo.pack(bytes)
  end

  def test_pack_map32
    # TODO: not sure how to test this without making a massive 66k item hash
  end

  def test_pack_unsupported_type
    assert_raises Mochilo::PackError do
      Mochilo.pack(Object.new)
    end
  end
end
