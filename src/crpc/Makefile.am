AUTOMAKE_OPTIONS = foreign subdir-objects

lib_LTLIBRARIES = libcrpc.la

SUBDIRS = ../utils \
		  ../protobuf

libcrpc_la_SOURCES = \
					 crpc_cli.c \
					 crpc_srv.c 

libcrpc_la_LIBADD = \
				   $(top_srcdir)/src/utils/libutils.la \
				   $(top_srcdir)/src/protobuf/libprotobuf.la

INCLUDES = \
		   -I$(top_srcdir)/include \
		   -I$(top_srcdir)/src/protobuf
		   

#静态库
#lib_LIBRARIES=libxxx.a
#xxx_a_SOURCES=
#xxx_a_LDADD=
#xxx_a_LIBADD=
#xxx_a_LDFLAGS=
