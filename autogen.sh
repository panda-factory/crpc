#!/bin/bash

DIE=true
PROJECT="CRPC"
SRCPATH=`pwd`

ACLOCAL=aclocal
AUTOHEADER=autoheader
AUTOMAKE=automake
AUTOCONF=autoconf

AUTOCCONF_VERSION=`$AUTOCONF --version | grep '^autoconf' | sed 's/.*) *//'`

case $AUTOCONF_VERSION in 0.* | 1.* | 2.[0-5]*)
    cat << EOF 
        You must have autoconf 2.60 or later installed to compile $PROJECT. 
        Download the appropriate package for your distribution/OS, 
        or get the source tarball at ......
EOF
DIE="exit 1"
    ;;
esac

AUTOMAKE_VERSION=`$AUTOMAKE --version | grep '^automake' | sed 's/.*) *//'`

case $AUTOMAKE_VERSION in 0.* | 1.[1][0-4]*)
    cat << EOF 
        You must have automake 1.90 or later installed to compile $PROJECT. 
        Download the appropriate package for your distribution/OS, 
        or get the source tarball at ......
EOF
DIE="exit 1"
    ;;
esac

$DIE

echo $ACLOCAL
$ACLOCAL || exit 1

echo $AUTOMAKE --add-missing
$AUTOMAKE --add-missing

echo $AUTOCONF
$AUTOCONF || exit 1

echo
echo "Now type \"./configure [options]\" and \"make\" to compile $PROJECT."
