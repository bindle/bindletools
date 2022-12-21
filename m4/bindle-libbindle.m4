#
#   Bindle Binaries Tools
#   Copyright (C) 2022 David M. Syzdek <david@syzdek.net>.
#
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions are
#   met:
#
#      * Redistributions of source code must retain the above copyright
#        notice, this list of conditions and the following disclaimer.
#      * Redistributions in binary form must reproduce the above copyright
#        notice, this list of conditions and the following disclaimer in the
#        documentation and/or other materials provided with the distribution.
#      * Neither the name of Bindle Binaries nor the
#        names of its contributors may be used to endorse or promote products
#        derived from this software without specific prior written permission.
#
#   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
#   IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
#   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#   PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL BINDLE BINARIES BE LIABLE FOR
#   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
#   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
#   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
#   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
#   OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
#   SUCH DAMAGE.
#


# AC_BINDLE_LIBBINDLE()
# ______________________________________________________________________________
AC_DEFUN([AC_BINDLE_LIBBINDLE],[dnl

   AC_REQUIRE([AC_PROG_CC])
   AC_REQUIRE([AC_BINDLE])

   LIBBINDLE_PREFIX="$1"
   LIBBINDLE_OPTION="$2"

   ENABLE_LIBBINDLE_BUILTIN="no"
   ENABLE_LIBBINDLE_SOURCES="no"
   ENABLE_LIBBINDLE_TESTS="no"
   ENABLE_LIBBINDLE="no"

   enableval=""
   AC_ARG_ENABLE(
      builtin-bindle,
      [AS_HELP_STRING([--enable-builtin-bindle], [enable builtin bindletools library])],
      [ EBUILTINBINDLE=$enableval ],
      [ EBUILTINBINDLE=$enableval ]
   )
   if test "x${LIBBINDLE_OPTION}" = "xyes";then
      ENABLE_LIBBINDLE_BUILTIN=yes
      ENABLE_LIBBINDLE_SOURCES=yes
   elif test "x${LIBBINDLE_OPTION}" = "xno";then
      ENABLE_LIBBINDLE_BUILTIN=no
      ENABLE_LIBBINDLE_SOURCES=no
   elif test "x${LIBBINDLE_OPTION}" = "xinstall";then
      ENABLE_LIBBINDLE_BUILTIN=no
      ENABLE_LIBBINDLE_SOURCES=no
      ENABLE_LIBBINDLE=yes
      ENABLE_LIBBINDLE_TESTS=yes
   elif test "x${LIBBINDLE_OPTION}" = "xsources";then
      ENABLE_LIBBINDLE_BUILTIN=no
      ENABLE_LIBBINDLE_SOURCES=yes
   elif test "x${EBUILTINBINDLE}" = "xyes";then
      ENABLE_LIBBINDLE_BUILTIN=yes
      ENABLE_LIBBINDLE_SOURCES=yes
   else
      LIBBINDLE_OPTION=auto
   fi

   # look for installed libbindle
   AC_CHECK_HEADERS([termios.h],     [], [])
   AC_CHECK_HEADERS([sgtty.h],       [], [])
   if test "x${LIBBINDLE_OPTION}" = "xauto";then
      FOUND_LIBBINDLE=yes
      AC_CHECK_HEADERS([bindle.h],                    [], [FOUND_LIBBINDLE=no])
      AC_CHECK_LIB([bindle], [bindle_free],           [], [FOUND_LIBBINDLE=no])
      AC_CHECK_LIB([bindle], [bindle_obj_release],    [], [FOUND_LIBBINDLE=no])
      AC_CHECK_LIB([bindle], [bindle_obj_retain],     [], [FOUND_LIBBINDLE=no])
      if test "x${FOUND_LIBBINDLE}" = "xyes";then
         ENABLE_LIBBINDLE_BUILTIN=no
         ENABLE_LIBBINDLE_SOURCES=no
      elif test "x${EBUILTINBINDLE}" = "xno";then
         AC_MSG_ERROR([libbindle not found])
      else
         ENABLE_LIBBINDLE_BUILTIN=yes
         ENABLE_LIBBINDLE_SOURCES=yes
      fi
   fi

   if test "x${ENABLE_LIBBINDLE_BUILTIN}" = "xyes";then
      ENABLE_LIBBINDLE_TESTS=yes
   fi

   # determine prefix
   if test "x${LIBBINDLE_PREFIX}" = "x";then
      LIBBINDLE_PREFIX="bindle_"
   elif test "x${ENABLE_LIBBINDLE_SOURCES}" = "xno";then
      LIBBINDLE_PREFIX="bindle_"
   fi

   AC_SUBST([LIBBINDLE_PREFIX],                 [${LIBBINDLE_PREFIX}])
   if test "x${LIBBINDLE_PREFIX}" != "xbindle_";then
      AC_DEFINE_UNQUOTED([LIBBINDLE_PREFIX],       ["$LIBBINDLE_PREFIX"],  [libbindle.la function prefix])
      AC_DEFINE_UNQUOTED([HAVE_BINDLE_PREFIX_H],   1,  [use bindle_prefix.h])
   fi
   #
   AM_CONDITIONAL([WITH_BINDLE_PREFIX_H],       [test "x${LIBBINDLE_PREFIX}"      != "xbindle_"])
   AM_CONDITIONAL([WITHOUT_BINDLE_PREFIX_H],    [test "x${LIBBINDLE_PREFIX}"       = "xbindle_"])
   #
   AM_CONDITIONAL([ENABLE_BUILTIN_BINDLE],      [test "x${ENABLE_LIBBINDLE_BUILTIN}"  = "xyes"])
   AM_CONDITIONAL([DISABLE_BUILTIN_BINDLE],     [test "x${ENABLE_LIBBINDLE_BUILTIN}" != "xyes"])
   #
   AM_CONDITIONAL([ENABLE_LIBBINDLE_SOURCES],   [test "x${ENABLE_LIBBINDLE_SOURCES}"  = "xyes"])
   AM_CONDITIONAL([DISABLE_LIBBINDLE_SOURCES],  [test "x${ENABLE_LIBBINDLE_SOURCES}" != "xyes"])
   #
   AM_CONDITIONAL([ENABLE_LIBBINDLE],           [test "x${ENABLE_LIBBINDLE}"       = "xyes"])
   AM_CONDITIONAL([DISABLE_LIBBINDLE],          [test "x${ENABLE_LIBBINDLE}"      != "xyes"])
   #
   AM_CONDITIONAL([ENABLE_LIBBINDLE_TESTS],     [test "x${ENABLE_LIBBINDLE_TESTS}"    = "xyes"])
   AM_CONDITIONAL([DISABLE_LIBBINDLE_TESTS],    [test "x${ENABLE_LIBBINDLE_TESTS}"   != "xyes"])
])dnl


# end of m4 file
