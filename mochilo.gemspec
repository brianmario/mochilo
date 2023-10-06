require './lib/mochilo/version'

Gem::Specification.new do |s|
  s.name = %q{mochilo}
  s.version = Mochilo::VERSION
  s.authors = ["Vicent Martí", "Brian Lopez"]
  s.date = Time.now.utc.strftime("%Y-%m-%d")
  s.email = %q{vicent@github.com seniorlopez@gmail.com}
  s.extensions = ["ext/mochilo/extconf.rb"]
  s.files = `git ls-files`.split("\n")
  s.homepage = %q{http://github.com/brianmario/mochilo}
  s.rdoc_options = ["--charset=UTF-8"]
  s.require_paths = ["lib"]
  s.rubygems_version = %q{1.4.2}
  s.summary = %q{A ruby library for BananaPack}
  s.test_files = `git ls-files test`.split("\n")
  s.required_ruby_version = ">= 2.4.0"

  # tests
  s.add_development_dependency 'rake-compiler', ">= 0.8.1"
  s.add_development_dependency 'minitest', ">= 4.1.0"
end
