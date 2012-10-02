require 'mkmf'

$CFLAGS += ' -fvisibility=hidden -O0'

dir_config('mochilo')
create_makefile('mochilo')
