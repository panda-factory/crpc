AUTOMAKE_OPTIONS = foreign subdir-objects

noinst_PROGRAMS = cli-demo

cli_demo_SOURCES = \
				   demo_cli0.c

cli_demo_LDADD = \
				  $(top_srcdir)/src/crpc/libcrpc.la

cli_demo_LDFLAGS = \
                   -lprotobuf-c -L/usr/lib

INCLUDES = \
		   -I$(top_srcdir)/include

SUBDIRS = \
		  ../crpc
