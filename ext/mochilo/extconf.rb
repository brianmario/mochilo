require 'mkmf'

have_header("ruby/st.h")
have_header("st.h")
create_makefile('mochilo/mochilo')
