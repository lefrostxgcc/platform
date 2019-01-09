#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([platform], [1.0], [chipz68q@gmail.com])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AM_PROG_AR
LT_INIT

# Checks for libraries.
PKG_CHECK_MODULES([gllibs], [gl, glew, glfw3 >= 3.0])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
				glad/Makefile
				glad/src/Makefile
				cglm/Makefile
				cglm/include/Makefile
				cglm/include/cglm/Makefile
				cglm/include/cglm/call/Makefile
				cglm/include/cglm/simd/Makefile
				cglm/include/cglm/simd/avx/Makefile
				cglm/include/cglm/simd/neon/Makefile
				cglm/include/cglm/simd/sse2/Makefile
				data/Makefile
				data/images/Makefile
				src/Makefile])
AC_OUTPUT
