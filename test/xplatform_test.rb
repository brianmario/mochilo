# encoding: utf-8
require File.expand_path('../setup', __FILE__)

require 'mochilo'

class CrossPlatformTest < MiniTest::Unit::TestCase
  ROOT_DIR = File.dirname(File.dirname(__FILE__))

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
  end
end
