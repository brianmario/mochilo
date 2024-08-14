# encoding: ascii-8bit
require File.expand_path('../setup', __FILE__)

require 'mochilo'

class MochiloUnpackTest < Minitest::Test
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

  def test_unpack_nil
    assert_nil Mochilo.unpack("\xC0")
  end

  def test_unpack_false
    assert_equal false, Mochilo.unpack("\xC2")
  end

  def test_unpack_true
    assert_equal true, Mochilo.unpack("\xC3")
  end

  def xtest_unpack_float
    # ruby uses double's internally so we can't reliably
    # test for a float without some hacks
  end

  def test_unpack_double
    assert_equal 123.45, Mochilo.unpack("\xCB@^\xDC\xCC\xCC\xCC\xCC\xCD")
  end

  def test_unpack_positive_fixed
    (0..127).each do |int|
      assert_equal int, Mochilo.unpack(int.chr)
    end
  end

  def test_unpack_negative_fixed
    assert_equal(-1, Mochilo.unpack("\xFF"))
    assert_equal(-32, Mochilo.unpack("\xE0"))
  end

  def test_unpack_uint8
    [128,255].each do |int|
      assert_equal int, Mochilo.unpack("\xCC#{int.chr}")
    end
  end

  def test_unpack_uint16
    [256,((2**16)-1)].each do |int|
      packed = [int].pack("n")
      assert_equal int, Mochilo.unpack("\xCD#{packed}")
    end
  end

  def test_unpack_uint32
    [(2**16),((2**32)-1)].each do |int|
      packed = [int].pack("N")
      assert_equal int, Mochilo.unpack("\xCE#{packed}")
    end
  end

  def test_unpack_uint64
    [(2**32),((2**64)-1)].each do |int|
      packed = [int].pack("Q>")
      assert_equal int, Mochilo.unpack("\xCF#{packed}")
    end
  end

  def test_unpack_int8
    assert_equal(-33, Mochilo.unpack("\xD0\xDF"))
    assert_equal(-128, Mochilo.unpack("\xD0\x80"))
  end

  def test_unpack_int16
    assert_equal(-129, Mochilo.unpack("\xD1\xFF\x7F"))
    assert_equal(-32768, Mochilo.unpack("\xD1\x80\x00"))
  end

  def test_unpack_int32
    assert_equal(-32769, Mochilo.unpack("\xD2\xFF\xFF\x7F\xFF"))
    assert_equal(-2147483648, Mochilo.unpack("\xD2\x80\x00\x00\x00"))
  end

  def test_unpack_int64
    assert_equal(-2147483649, Mochilo.unpack("\xD3\xFF\xFF\xFF\xFF\x7F\xFF\xFF\xFF"))
    assert_equal(-9223372036854775808, Mochilo.unpack("\xD3\x80\x00\x00\x00\x00\x00\x00\x00"))
  end

  def test_unpack_fixed_str
    str = "a".force_encoding('UTF-8')
    val = Mochilo.unpack("\xA1#{str}")
    assert_equal str, val
    assert_equal Encoding::UTF_8, val.encoding

    str = ("a"*31).force_encoding('UTF-8')
    val = Mochilo.unpack("\xBF#{str}")
    assert_equal str, val
    assert_equal Encoding::UTF_8, val.encoding
  end

  def test_unpack_str8
    str = ("a"*32).force_encoding('UTF-8')
    val = Mochilo.unpack("\xD9#{str.bytesize.chr}#{str}")
    assert_equal str, val
    assert_equal Encoding::UTF_8, val.encoding

    str = ("a"*255).force_encoding('UTF-8')
    val = Mochilo.unpack("\xD9#{str.bytesize.chr}#{str}")
    assert_equal str, val
    assert_equal Encoding::UTF_8, val.encoding
  end

  def test_unpack_str16
    str = ("a"*256).force_encoding('UTF-8')
    val = Mochilo.unpack("\xDA\x01\x00#{str}")
    assert_equal str, val
    assert_equal Encoding::UTF_8, val.encoding

    str = ("a"*(2**16-1)).force_encoding('UTF-8')
    val = Mochilo.unpack("\xDA\xFF\xFF#{str}")
    assert_equal str, val
    assert_equal Encoding::UTF_8, val.encoding
  end

  def test_unpack_str32
    str = ("a"*(2**16)).force_encoding('UTF-8')
    val = Mochilo.unpack("\xDB\x00\x01\x00\x00#{str}")
    assert_equal str, val
    assert_equal Encoding::UTF_8, val.encoding

    # This will create a 4GB string, so let's just skip that ;)
    # str = ("a"*(2**32-1)).force_encoding('UTF-8')
    # val = Mochilo.unpack("\xDB\xFF\xFF\xFF\xFF#{str}")
    # assert_equal str, val
    # assert_equal Encoding::UTF_8, val.encoding
  end

  def test_unpack_enc8
    str = ("a"*32).force_encoding('ISO-8859-1')
    val = Mochilo.unpack("\xC7#{str.bytesize.chr}\x0C#{str}")
    assert_equal str, val
    assert_equal Encoding::ISO_8859_1, val.encoding

    str = ("a"*255).force_encoding('ISO-8859-1')
    val = Mochilo.unpack("\xC7#{str.bytesize.chr}\x0C#{str}")
    assert_equal str, val
    assert_equal Encoding::ISO_8859_1, val.encoding
  end

  def test_unpack_enc16
    str = ("a"*256).force_encoding('ISO-8859-1')
    val = Mochilo.unpack("\xC8\x01\x00\x0C#{str}")
    assert_equal str, val
    assert_equal Encoding::ISO_8859_1, val.encoding

    str = ("a"*(2**16-1)).force_encoding('ISO-8859-1')
    val = Mochilo.unpack("\xC8\xFF\xFF\x0C#{str}")
    assert_equal str, val
    assert_equal Encoding::ISO_8859_1, val.encoding
  end

  def test_unpack_enc32
    str = ("a"*(2**16)).force_encoding('ISO-8859-1')
    val = Mochilo.unpack("\xC9\x00\x01\x00\x00\x0C#{str}")
    assert_equal str, val
    assert_equal Encoding::ISO_8859_1, val.encoding

    # This would create a 4GB string, so let's just skip that ;)
    # str = ("a"*(2**32-1)).force_encoding('ISO-8859-1')
    # val = Mochilo.unpack("\xC9\x00\x01\x00\x00\x0C#{str}")
    # assert_equal str, val
    # assert_equal Encoding::ISO_8859_1, val.encoding
  end

  def test_unpack_bin8
    str = ("a"*32).force_encoding('binary')
    val = Mochilo.unpack("\xC4#{str.bytesize.chr}#{str}")
    assert_equal str, val
    assert_equal Encoding::ASCII_8BIT, val.encoding

    str = ("a"*255).force_encoding('binary')
    val = Mochilo.unpack("\xC4#{str.bytesize.chr}#{str}")
    assert_equal str, val
    assert_equal Encoding::ASCII_8BIT, val.encoding
  end

  def test_unpack_bin16
    str = ("a"*256).force_encoding('binary')
    val = Mochilo.unpack("\xC5\x01\x00#{str}")
    assert_equal str, val
    assert_equal Encoding::ASCII_8BIT, val.encoding

    str = ("a"*(2**16-1)).force_encoding('binary')
    val = Mochilo.unpack("\xC5\xFF\xFF#{str}")
    assert_equal str, val
    assert_equal Encoding::ASCII_8BIT, val.encoding
  end

  def test_unpack_bin32
    str = ("a"*(2**16)).force_encoding('binary')
    val = Mochilo.unpack("\xC6\x00\x01\x00\x00#{str}")
    assert_equal str, val
    assert_equal Encoding::ASCII_8BIT, val.encoding

    # This would create a 4GB string, so let's just skip that ;)
    # str = ("a"*(2**32-1)).force_encoding('UTF-8')
    # val = Mochilo.unpack("\xC6\x00\x01\x00\x00#{str}")
    # assert_equal str, val
    # assert_equal Encoding::ASCII_8BIT, val.encoding
  end

  def test_unpack_fixed_array
    assert_equal [], Mochilo.unpack("\x90")

    arr = (1..15).to_a
    data = "\x9F"
    arr.each {|i| data << Mochilo.pack(i)}
    assert_equal arr, Mochilo.unpack(data)
  end

  def test_unpack_array16
    arr = (1..16).to_a
    data = "\xDC\x00\x10"
    arr.each {|i| data << Mochilo.pack(i)}
    assert_equal arr, Mochilo.unpack(data)

    arr = (1..2**16-1).to_a
    data = "\xDC\xFF\xFF"
    arr.each {|i| data << Mochilo.pack(i)}
    assert_equal arr, Mochilo.unpack(data)
  end

  def test_unpack_array32
    arr = (1..2**16).to_a
    data = "\xDD\x00\x01\x00\x00"
    arr.each {|i| data << Mochilo.pack(i)}
    assert_equal arr, Mochilo.unpack(data)

    # This would create an array with 4294967295 entries in it, so let's not
    # arr = (1..2**32-1).to_a
    # data = "\xDD\xFF\xFF\xFF\xFF"
    # arr.each {|i| data << Mochilo.pack(i)}
    # assert_equal arr, Mochilo.unpack(data)
  end

  def test_pack_fixed_map
    assert_equal({}, Mochilo.unpack("\x80"))

    arr = (1..15).to_a
    hash = {}
    arr.each {|i| hash[i] = i }
    data = "\x8F"
    arr.each {|i| data << (Mochilo.pack(i) + Mochilo.pack(i))}
    assert_equal hash, Mochilo.unpack(data)
  end

  def test_pack_map16
    arr = (1..16).to_a
    hash = {}
    arr.each {|i| hash[i] = i }
    data = "\xDE\x00\x10"
    arr.each {|i| data << (Mochilo.pack(i) + Mochilo.pack(i))}
    assert_equal hash, Mochilo.unpack(data)

    arr = (1..2**16-1).to_a
    hash = {}
    arr.each {|i| hash[i] = i }
    data = "\xDE\xFF\xFF"
    arr.each {|i| data << (Mochilo.pack(i) + Mochilo.pack(i))}
    assert_equal hash, Mochilo.unpack(data)
  end

  def test_pack_map32
    arr = (1..2**16).to_a
    hash = {}
    arr.each {|i| hash[i] = i }
    data = "\xDF\x00\x01\x00\x00"
    arr.each {|i| data << (Mochilo.pack(i) + Mochilo.pack(i))}
    assert_equal hash, Mochilo.unpack(data)

    # This would create a hash with 4294967295 entries in it, so let's not
    # arr = (1..2**32-1).to_a
    # hash = {}
    # arr.each {|i| hash[i] = i }
    # data = "\xDF\xFF\xFF\xFF\xFF"
    # arr.each {|i| data << (Mochilo.pack(i) + Mochilo.pack(i))}
    # assert_equal hash, Mochilo.unpack(data)
  end

  def test_unpack_custom_types_with_errors
    # regexp has all the options bits
    re = Mochilo.unpack("\xC7\x0D\xFF\x01\xFF\xFF\xFF\xFF\x01pa.tern")
    refute_equal 0, re.options, "unpacked regexp options"

    # time is missing the last char
    assert_raises Mochilo::UnpackError do
      Mochilo.unpack("\xC7\x11\xFF\x02\x00\x00\x00\x00\x88\x77\x66\x55\x00\x00\x00\x00\x00\x0E\xDC")
    end

    # time overflows
    assert_raises RangeError do
      # set the top bit of the time.....v ...and of usec................v
      Mochilo.unpack("\xC7\x15\xFF\x02\x80\x00\x00\x00\x88\x77\x66\x55\x80\x00\x00\x00\x00\x0E\xDC\xBA\xFF\xFF\xFF\xFF")
    end

    # unknown field
    assert_raises Mochilo::UnpackError do
      Mochilo.unpack("\xC7\x07\xFF\x10unknowncustomtype")
    end
  end

  # Make sure that the custom parsers never read past the end
  # of their buffers.
  def test_unpack_custom_types_with_missing_char
    assert_error_on_truncated_unpack "\xC7\x07\xFF\x00symbol"
    assert_error_on_truncated_unpack "\xC7\x0D\xFF\x01\x00\x00\x00\x00\x01pa.tern"
    assert_error_on_truncated_unpack "\xC7\x15\xFF\x02\x00\x00\x00\x00\x88\x77\x66\x55\x00\x00\x00\x00\x00\x0E\xDC\xBA\x00\x00\x00\x00"
  end

  def assert_error_on_truncated_unpack(packed)
    refute_nil Mochilo.unpack(packed)
    assert_raises Mochilo::UnpackError do
      Mochilo.unpack(packed.chop)
    end
  end
end
