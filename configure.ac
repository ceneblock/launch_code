#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Launch Code Webserver], [0.1], [ceneblock@ceneblock.com])
AC_CONFIG_SRCDIR([src/main.cpp])
AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_HEADERS([config.h])
AC_OUTPUT(Makefile src/Makefile)

#Muk up the CFLAGS variable
: ${CFLAGS="-g"}

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL
LT_INIT
LT_LANG([C++])

AC_CONFIG_MACRO_DIRS([m4])

# Checks for libraries.
AX_CXX_COMPILE_STDCXX_11([ext],[mandatory])

# Checks for header files.
AC_CHECK_HEADERS([unistd.h])
AC_CHECK_HEADERS([sys/types.h sys/socket.h netinet/in.h \
                  string.h arpa/inet.h sys/stat.h fcntl.h \
                  magic.h poll.h sys/ioctl.h magic.h])

AC_CHECK_HEADERS([magic.h], [], [have_magic=no])

if test "x${have_magic}" = xno; then
  AC_MSG_ERROR([
      -----------------------------------------------
       Unable to find BSD file dev on this system.
       Check with your distribution on how to install
       BSD file.

       Source code for BSD file can be found at:
            http://ftp.astron.com/pub/file/
            https://github.com/file/file
      -----------------------------------------------
      ])
fi


# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_CHECK_LIB([pthread], [pthread_create])
AC_CHECK_LIB([magic], [magic_open])

AC_OUTPUT
