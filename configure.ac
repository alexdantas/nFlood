#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([nflood], [0.1], [alex.dantas92@gmail.com])
# AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Checks for libraries.
AC_CHECK_LIB([ncurses], [initscr])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile src/Makefile])

AC_OUTPUT

# My custom banner after ./configure
echo \
"
Thank you for using ${PACKAGE_NAME} version ${PACKAGE_VERSION}!
Now type [make] and [sudo make install] to install the program.
To execute before installing, type [make run]."

