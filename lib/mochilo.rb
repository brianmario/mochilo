require 'mochilo/mochilo'

module Mochilo
  alias encode pack
  alias dump pack

  alias decode unpack
  alias load unpack

  extend self
end
