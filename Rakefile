require 'date'
require 'rake/clean'
require 'rake/extensiontask'
require 'digest/md5'

task :default => :compile

# ==========================================================
# Ruby Extension
# ==========================================================
Rake::ExtensionTask.new('mochilo') do |ext|
  ext.cross_compile = true
  ext.cross_platform = ['x86-mingw32', 'x86-mswin32-60']

  ext.lib_dir = File.join 'lib', 'mochilo'
end

desc "Open an irb session preloaded with Mochilo"
task :console do
  sh "irb -rubygems -I lib -r ./lib/mochilo"
end

require 'rake/testtask'
Rake::TestTask.new('test') do |t|
  t.test_files = FileList['test/*_test.rb']
end
task 'test' => [:compile]


task :encodings do
  sh "ruby genperf.rb | gperf > ./ext/mochilo/encodings.h"
end
