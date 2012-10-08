require 'date'
require 'rake/clean'
require 'rake/extensiontask'
require 'digest/md5'

task :default => :compile

# ==========================================================
# Ruby Extension
# ==========================================================
Rake::ExtensionTask.new('mochilo')

desc "Open an irb session preloaded with Mochilo"
task :console do
  sh "irb -rubygems -I lib -r ./lib/mochilo"
end

require 'rake/testtask'
Rake::TestTask.new('test') do |t|
  t.test_files = FileList['test/*_test.rb']
  t.ruby_opts += ['-rubygems'] if defined? Gem
end
task 'test' => [:compile]


task :encodings do
  sh "ruby genperf.rb | gperf > ./ext/mochilo/encodings.h"
end
