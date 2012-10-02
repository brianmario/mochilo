require 'mkmf'

$CFLAGS += ' -fvisibility=hidden'

dir_config('mochilo')
create_makefile('mochilo')
