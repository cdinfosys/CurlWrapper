#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([CurlWrapper], [0.1], [cdisgithub@cdinfosys.co.za])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_SUBST([CC])

AC_CHECK_LIB([curl], [curl_easy_setopt], [], [AC_MSG_ERROR([libcurl library is not present])])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES(Makefile demo_fetch/Makefile demo_upload/Makefile)
AC_OUTPUT
