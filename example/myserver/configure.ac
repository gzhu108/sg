#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([myserver], [1.0], [BUG-REPORT-ADDRESS])
AC_CONFIG_AUX_DIR([.])
AM_INIT_AUTOMAKE
AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

PKG_CHECK_MODULES([UUID], [uuid >= 2.23.0])
AC_SUBST([UUID_CFLAGS])
AC_SUBST([UUID_LIBS])

AC_OUTPUT
