# encoding: utf-8
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
      if a.nil?
        assert_nil c
      else
        assert_equal a, c
      end
    end
  end

  def test_unpack_nil
    assert_nil Mochilo.unpack("\xC0")
  end

  def test_unpack_true
    assert_equal true, Mochilo.unpack("\xC3")
  end

  def test_unpack_false
    assert_equal false, Mochilo.unpack("\xC2")
  end

  def xtest_unpack_float
    # ruby uses double's internally so we can't reliably
    # test for a float without some hacks
  end

  def test_unpack_double
    assert_equal 123.45, Mochilo.unpack("\xCB@^\xDC\xCC\xCC\xCC\xCC\xCD")
  end

  def test_unpack_positive_fixed
    assert_equal 21, Mochilo.unpack("\x15")
  end

  def test_unpack_negative_fixed
    assert_equal(-21, Mochilo.unpack("\xEB"))
  end

  def test_unpack_uint8
    assert_equal 214, Mochilo.unpack("\xCC\xD6")
  end

  def test_unpack_uint16
    assert_equal 21474, Mochilo.unpack("\xCDS\xE2")
  end

  def test_unpack_uint32
    assert_equal 2147483647, Mochilo.unpack("\xCE\x7F\xFF\xFF\xFF")
  end

  def test_unpack_uint64
    assert_equal 21474836479, Mochilo.unpack("\xCF\x00\x00\x00\x04\xFF\xFF\xFF\xFF")
  end

  def test_unpack_int8
    assert_equal(-34, Mochilo.unpack("\xD0\xDE"))
  end

  def test_unpack_int16
    assert_equal(-21474, Mochilo.unpack("\xD1\xAC\x1E"))
  end

  def test_unpack_int32
    assert_equal(-2147483647, Mochilo.unpack("\xD2\x80\x00\x00\x01"))
  end

  def test_unpack_int64
    assert_equal(-21474836479, Mochilo.unpack("\xD3\xFF\xFF\xFF\xFB\x00\x00\x00\x01"))
  end

  if defined?(Encoding)
    def test_unpack_str16
      str = "this is a test".force_encoding('UTF-8')
      assert_equal str, Mochilo.unpack("\xD8\x00\x0E\x00#{str}")
    end
  end

  def xtest_unpack_str32
    # TODO: not sure how to test this without making a massive 66k string
  end

  def test_unpack_fixed_raw
    str = "this is a test"
    assert_equal str, Mochilo.unpack("\xAE#{str}")
  end

  def test_unpack_symbol
    assert_raises Mochilo::UnpackError do
      Mochilo.unpack("\xD4\x04test")
    end

    assert_equal :test, Mochilo.unpack_unsafe("\xD4\x04test")
  end

  def test_unpack_raw16
    str = ("a"*255)
    assert_equal str, Mochilo.unpack("\xDA\x00\xFF#{str}")
  end

  def xtest_unpack_raw32
    # TODO: not sure how to test this without making a massive 66k string
  end

  def test_unpack_fixed_array
    assert_equal [], Mochilo.unpack("\x90")
    assert_equal [1], Mochilo.unpack("\x91\x01")
  end

  def test_unpack_array16
    bytes = ("a"*34).bytes.to_a
    assert_equal bytes, Mochilo.unpack("\xDC\x00\"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
  end

  def xtest_unpack_array32
    # TODO: not sure how to test this without making a massive 66k item array
  end

  def test_unpack_fixed_map
    assert_equal({}, Mochilo.unpack("\x80"))
    assert_equal({1 => 2}, Mochilo.unpack("\x81\x01\x02"))
  end

  def test_unpack_map16
    bytes = ("a"*34).bytes.to_a
    assert_equal bytes, Mochilo.unpack("\xDC\x00\"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
  end

  def test_unpack_map32
    # TODO: not sure how to test this without making a massive 66k item hash
  end

  def test_unpack_time_compat
    t = Time.at 1234567890
    packed = Mochilo.pack(t)
    assert_equal t, Mochilo.unpack(packed)
    assert_equal t, Mochilo::Compat_1_2.unpack(packed)
  end

  def test_unpack_regex_compat
    re = /hi/i
    packed = Mochilo.pack(re)
    assert_equal re, Mochilo.unpack(packed)
    assert_equal re, Mochilo::Compat_1_2.unpack(packed)
  end

  def test_unpack_multibyte_strings_of_various_sizes
    # Check a variety of sizes
    0.upto(2000) do |size|
      contents = "a"*size
      contents.force_encoding("UTF-16BE")
      packed = Mochilo.pack(contents)
      unpacked = Mochilo.unpack(packed)

      # Regression test: previously crashed on Ruby 3.2
      unpacked.b

      assert_equal contents, unpacked
    end
  end
end
