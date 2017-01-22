#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([zlib-example], [1.0], [/dev/null])
# AC_CONFIG_SRCDIR([decompress.c])
AC_CONFIG_HEADERS([config.h])

AC_CHECK_HEADER([zlib.h], [], [
  AC_MSG_ERROR([Unable to find zlib.h])
])

AC_CHECK_LIB([z], [inflateEnd], [], [
  AC_MSG_ERROR([Unable to find zlib])
])

# Checks for programs.
AC_PROG_CC
AM_INIT_AUTOMAKE
# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
