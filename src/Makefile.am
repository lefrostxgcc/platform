bin_PROGRAMS = platform
platform_SOURCES = main.c stb_image.h
platform_CPPFLAGS= -DDATA_PATH=\"$(pkgdatadir)\" -I$(top_srcdir)/src
platform_CFLAGS = -I$(top_srcdir)/glad/include @gllibs_CFLAGS@ $(AM_CFLAGS)
platform_LDADD = ../glad/src/libglglad.la @gllibs_LIBS@ -lm $(AM_LDADD)
