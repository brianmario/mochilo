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
    [0,127].each do |int|
      assert_equal int.chr, Mochilo.pack(int)
    end
  end

  def test_pack_negative_fixed
    assert_equal "\xFF", Mochilo.pack(-1)
    assert_equal "\xE0", Mochilo.pack(-32)
  end

  def test_pack_uint8
    [128,255].each do |int|
      assert_equal "\xCC#{int.chr}", Mochilo.pack(int)
    end
  end

  def test_pack_uint16
    [256,((2**16)-1)].each do |int|
      packed = [int].pack("n")
      assert_equal "\xCD#{packed}", Mochilo.pack(int)
    end
  end

  def test_pack_uint32
    [(2**16),((2**32)-1)].each do |int|
      packed = [int].pack("N")
      assert_equal "\xCE#{packed}", Mochilo.pack(int)
    end
  end

  def test_pack_uint64
    [(2**32),((2**64)-1)].each do |int|
      packed = [int].pack("Q>")
      assert_equal "\xCF#{packed}", Mochilo.pack(int)
    end
  end

  def test_pack_int8
    assert_equal "\xD0\xDF", Mochilo.pack(-33)
    assert_equal "\xD0\x80", Mochilo.pack(-128)
  end

  def test_pack_int16
    assert_equal "\xD1\xFF\x7F", Mochilo.pack(-129)
    assert_equal "\xD1\x80\x00", Mochilo.pack(-32768)
  end

  def test_pack_int32
    assert_equal "\xD2\xFF\xFF\x7F\xFF", Mochilo.pack(-32769)
    assert_equal "\xD2\x80\x00\x00\x00", Mochilo.pack(-2147483648)
  end

  def test_pack_int64
    assert_equal "\xD3\xFF\xFF\xFF\xFF\x7F\xFF\xFF\xFF", Mochilo.pack(-2147483649)
    assert_equal "\xD3\x80\x00\x00\x00\x00\x00\x00\x00", Mochilo.pack(-9223372036854775808)
  end

  def test_pack_fixed_str
    str = "a".force_encoding('UTF-8')
    assert_equal "\xA1#{str}", Mochilo.pack(str)

    str = ("a"*31).force_encoding('UTF-8')
    assert_equal "\xBF#{str}", Mochilo.pack(str)
  end

  def test_pack_str8
    str = ("a"*32).force_encoding('UTF-8')
    assert_equal "\xD9#{str.bytesize.chr}#{str}", Mochilo.pack(str)

    str = ("a"*255).force_encoding('UTF-8')
    assert_equal "\xD9#{str.bytesize.chr}#{str}", Mochilo.pack(str)
  end

  def test_pack_str16
    str = ("a"*256).force_encoding('UTF-8')
    assert_equal "\xDA\x01\x00#{str}", Mochilo.pack(str)

    str = ("a"*(2**16-1)).force_encoding('UTF-8')
    assert_equal "\xDA\xFF\xFF#{str}", Mochilo.pack(str)
  end

  def test_pack_str32
    str = ("a"*(2**16)).force_encoding('UTF-8')
    assert_equal "\xDB\x00\x01\x00\x00#{str}", Mochilo.pack(str)

    # This will create a 4GB string, so let's just skip that ;)
    # str = ("a"*(2**32-1)).force_encoding('UTF-8')
    # assert_equal "\xDB\x00\x01\x00\x00#{str}", Mochilo.pack(str)
  end

  def test_pack_enc8
    str = ("a"*32).force_encoding('ISO-8859-1')
    assert_equal "\xC7#{str.bytesize.chr}\x0C#{str}", Mochilo.pack(str)

    str = ("a"*255).force_encoding('ISO-8859-1')
    assert_equal "\xC7#{str.bytesize.chr}\x0C#{str}", Mochilo.pack(str)
  end

  def test_pack_enc16
    str = ("a"*256).force_encoding('ISO-8859-1')
    assert_equal "\xC8\x01\x00\x0C#{str}", Mochilo.pack(str)

    str = ("a"*(2**16-1)).force_encoding('ISO-8859-1')
    assert_equal "\xC8\xFF\xFF\x0C#{str}", Mochilo.pack(str)
  end

  def test_pack_enc32
    str = ("a"*(2**16)).force_encoding('ISO-8859-1')
    assert_equal "\xC9\x00\x01\x00\x00\x0C#{str}", Mochilo.pack(str)

    # This would create a 4GB string, so let's just skip that ;)
    # str = ("a"*(2**32-1)).force_encoding('ISO-8859-1')
    # assert_equal "\xC9\x00\x01\x00\x00\x0C#{str}", Mochilo.pack(str)
  end

  def test_pack_bin8
    str = ("a"*32).force_encoding('binary')
    assert_equal "\xC4#{str.bytesize.chr}#{str}", Mochilo.pack(str)

    str = ("a"*255).force_encoding('binary')
    assert_equal "\xC4#{str.bytesize.chr}#{str}", Mochilo.pack(str)
  end

  def test_pack_bin16
    str = ("a"*256).force_encoding('binary')
    assert_equal "\xC5\x01\x00#{str}", Mochilo.pack(str)

    str = ("a"*(2**16-1)).force_encoding('binary')
    assert_equal "\xC5\xFF\xFF#{str}", Mochilo.pack(str)
  end

  def test_pack_bin32
    str = ("a"*(2**16)).force_encoding('binary')
    assert_equal "\xC6\x00\x01\x00\x00#{str}", Mochilo.pack(str)

    # This would create a 4GB string, so let's just skip that ;)
    # str = ("a"*(2**32-1)).force_encoding('UTF-8')
    # assert_equal "\xC6\x00\x01\x00\x00#{str}", Mochilo.pack(str)
  end

  def test_pack_fixed_array
    assert_equal "\x90", Mochilo.pack([])

    arr = (1..15).to_a
    expected = "\x9F"
    arr.each {|i| expected << Mochilo.pack(i)}
    assert_equal expected, Mochilo.pack(arr)
  end

  def test_pack_array16
    arr = (1..16).to_a
    expected = "\xDC\x00\x10"
    arr.each {|i| expected << Mochilo.pack(i)}
    assert_equal expected, Mochilo.pack(arr)

    arr = (1..2**16-1).to_a
    expected = "\xDC\xFF\xFF"
    arr.each {|i| expected << Mochilo.pack(i)}
    assert_equal expected, Mochilo.pack(arr)
  end

  def test_pack_array32
    arr = (1..2**16).to_a
    expected = "\xDD\x00\x01\x00\x00"
    arr.each {|i| expected << Mochilo.pack(i)}
    assert_equal expected, Mochilo.pack(arr)

    # This would create an array with 4294967295 entries in it, so let's not
    # arr = (1..2**32-1).to_a
    # expected = "\xDD\xFF\xFF\xFF\xFF"
    # arr.each {|i| expected << Mochilo.pack(i)}
    # assert_equal expected, Mochilo.pack(arr)
  end

  def test_pack_fixed_map
    assert_equal "\x80", Mochilo.pack({})

    arr = (1..15).to_a
    hash = {}
    arr.each {|i| hash[i] = i }
    expected = "\x8F"
    arr.each {|i| expected << (Mochilo.pack(i) + Mochilo.pack(i))}
    assert_equal expected, Mochilo.pack(hash)
  end

  def test_pack_map16
    arr = (1..16).to_a
    hash = {}
    arr.each {|i| hash[i] = i }
    expected = "\xDE\x00\x10"
    arr.each {|i| expected << (Mochilo.pack(i) + Mochilo.pack(i))}
    assert_equal expected, Mochilo.pack(hash)

    arr = (1..2**16-1).to_a
    hash = {}
    arr.each {|i| hash[i] = i }
    expected = "\xDE\xFF\xFF"
    arr.each {|i| expected << (Mochilo.pack(i) + Mochilo.pack(i))}
    assert_equal expected, Mochilo.pack(hash)
  end

  def test_pack_map32
    arr = (1..2**16).to_a
    hash = {}
    arr.each {|i| hash[i] = i }
    expected = "\xDF\x00\x01\x00\x00"
    arr.each {|i| expected << (Mochilo.pack(i) + Mochilo.pack(i))}
    assert_equal expected, Mochilo.pack(hash)

    # This would create a hash with 4294967295 entries in it, so let's not
    # arr = (1..2**32-1).to_a
    # hash = {}
    # arr.each {|i| hash[i] = i }
    # expected = "\xDF\xFF\xFF\xFF\xFF"
    # arr.each {|i| expected << (Mochilo.pack(i) + Mochilo.pack(i))}
    # assert_equal expected, Mochilo.pack(hash)
  end

  def test_pack_unsupported_type
    assert_raises Mochilo::PackError do
      Mochilo.pack(Object.new)
    end
  end

  def test_pack_symbol
    expected = "\xC7\x08\xFF\x00symbol\x00"
    assert_equal expected, Mochilo.pack(:symbol)
    assert_equal :symbol, Mochilo.unpack(expected)
  end

  def test_pack_regexp
    expected = "\xC7\x0D\xFF\x01\x00\x00\x00\x00\x01pa.tern"
    assert_equal expected, Mochilo.pack(/pa.tern/)
    [
      /pa.tern/,
      /thing/im,
    ].each do |re|
      assert_equal re, Mochilo.unpack(Mochilo.pack(re))
    end
  end

  def test_time
    t = Time.gm(2042, 7, 21, 3, 32, 37, 974010)
    expected = "\xC7\x11\xFF\x02" +
      "\x00\x00\x00\x00\x88\x77\x66\x55" + # sec
      "\x00\x00\x00\x00\x00\x0E\xDC\xBA"   # usec
    assert_equal expected, Mochilo.pack(t)
    assert_equal t, Mochilo.unpack(expected)
  end
end
