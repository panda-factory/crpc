AUTOMAKE_OPTIONS = foreign subdir-objects

ACLOCAL_AMFLAGS=-I m4

noinst_PROGRAMS = crpcd

crpcd_SOURCES = \
				 $(top_srcdir)/src/executive/daemon.c


crpcd_LDADD = \
			   $(top_srcdir)/src/crpc/libcrpc.la 
			   $(top_srcdir)/src/utils/libutils.la 

INCLUDES = \
		   -I$(top_srcdir)/include

SUBDIRS = \
		  src/crpc \
		  src/demo

#AM_CFLAGS = -fdiagnostics-color=auto

#可执行文件
#bin_PROGRAMS=xxx
#xxx_SOURCES=xxx.c
#xxx_LDADD=
#xxx_LDFLAGS=
#xxx_DEPENDENCIES=

#静态库
#lib_LIBRARIES=libxxx.a
#xxx_a_SOURCES=
#xxx_a_LDADD=
#xxx_a_LIBADD=
#xxx_a_LDFLAGS=

#动态库
#lib_LTLIBRARIES=xxx.la
#xxx_la_SOURCES=
#xxx_la_LDADD=
#xxx_la_LIBADD=
#xxx_la_LD_FLAGS=

#头文件
#include_HEADERS=xxx.h
#noinst_HEADERS=xxx.h

#data_DATA=xxx

#INCLUDES= 链接时所需要的头文件
#LDADD= 链接时所需要的库文件
#LDFLAGS= 链接时所需要的库文件选项标志
#AM_CXXFLAGS 编译.cpp时的选项
#AM_CFLAGS 编译.c时的选项
#EXTRA_DIST= 
#SUBDIRS 在处理本目录之前要递归处理哪些子目录

#$(top_srcdir)工程最顶层目录，用于引用源程序
#$(top_builddir)定义了生成目标文件的最上层目录，用于引用.o等编译出来的目标文件

#http://blog.csdn.net/sayigood/article/details/4954772
