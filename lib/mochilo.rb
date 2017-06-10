require 'mochilo/mochilo'

module Mochilo
  alias encode pack
  alias dump pack

  alias decode unpack
  alias load unpack

  extend self

  module Compat_1_2
    alias encode pack
    alias dump pack

    alias decode unpack
    alias load unpack

    extend self
  end
end
