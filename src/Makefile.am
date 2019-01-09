bin_PROGRAMS = platform
platform_SOURCES = main.c
platform_CFLAGS = -I$(top_srcdir)/glad/include @gllibs_CFLAGS@ $(AM_CFLAGS)
platform_LDADD = ../glad/src/libglglad.la @gllibs_LIBS@ $(AM_LDADD)
