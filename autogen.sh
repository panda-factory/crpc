#!/bin/sh
aclocal 
libtoolize -c -f
#autoheader 
autoconf
automake --add-missing
