#
#   Bindle Binaries Tools
#   Copyright (C) 2013 Bindle Binaries <syzdek@bindlebinaries.com>.
#
#   @BINDLE_BINARIES_BSD_LICENSE_START@
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
#   @BINDLE_BINARIES_BSD_LICENSE_END@
#
#   m4/bindle-sqlite3.m4 - m4 macros for finding SQLite3 client library.
#


# AC_BINDLE_WITH_SQLITE3(action-if-true, action-if-false)
# ______________________________________________________________________________
AC_DEFUN([AC_BINDLE_WITH_SQLITE3],[dnl

   # prerequists
   AC_REQUIRE([AC_PROG_CC])

   # saves old parameters
   OLD_CFLAGS=${CFLAGS}
   OLD_CPPFLAGS=${CPPFLAGS}
   OLD_LDFLAGS=${LDFLAGS}
   OLD_LIBS=${LIBS}

   # set initial values
   HAVE_SQLITE3="yes"
   SQLITE3_CFLAGS=""
   SQLITE3_CPPFLAGS=""
   SQLITE3_LDFLAGS=""
   SQLITE3_LIBS=""

   # display options
   withval=""
   AC_ARG_WITH(
      sqlite3-include,
      [AS_HELP_STRING([--with-sqlite3-include=DIR], [Search for SQLite3 headers in DIR.])],
      [ SQLITE_INCLUDEDIR=$withval ],
      [ SQLITE_INCLUDEDIR=$withval ]
   )
   withval=""
   AC_ARG_WITH(
      sqlite3-lib,
      [AS_HELP_STRING([--with-sqlite3-lib=DIR], [Search for SQLite3 library in DIR.])],
      [ SQLITE_LIBDIR=$withval ],
      [ SQLITE_LIBDIR=$withval ]
   )

   # applies options
   if test "x${SQLITE_INCLUDEDIR}" != "x" || \
      test "x${SQLITE_INCLUDEDIR}" != "xno" || \
      test "x${SQLITE_INCLUDEDIR}" != "xyes";then
      SQLITE3_CPPFLAGS="-I${SQLITE_INCLUDEDIR}"
   fi
   if test "x${SQLITE_LIBDIR}" != "x" || \
      test "x${SQLITE_LIBDIR}" != "xno" || \
      test "x${SQLITE_LIBDIR}" != "xyes";then
      SQLITE3_LDFLAGS="-L${SQLITE_LIBDIR}"
   fi

   # apply sqlite parameters
   CFLAGS="${SQLITE3_CFLAGS}"
   CPPFLAGS="${SQLITE3_CPPFLAGS}"
   LDFLAGS="${SQLITE3_LDFLAGS}"
   LIBS="${SQLITE3_LIBS}"

   # check for SQLite3
   AC_CHECK_HEADERS([sqlite3.h],                     [], [HAVE_SQLITE3="no"])
   AC_SEARCH_LIBS([sqlite3_close],        [sqlite3], [], [HAVE_SQLITE3="no"])
   AC_SEARCH_LIBS([sqlite3_column_count], [sqlite3], [], [HAVE_SQLITE3="no"])
   AC_SEARCH_LIBS([sqlite3_column_type],  [sqlite3], [], [HAVE_SQLITE3="no"])
   AC_SEARCH_LIBS([sqlite3_column_value], [sqlite3], [], [HAVE_SQLITE3="no"])
   AC_SEARCH_LIBS([sqlite3_exec],         [sqlite3], [], [HAVE_SQLITE3="no"])
   AC_SEARCH_LIBS([sqlite3_finalize],     [sqlite3], [], [HAVE_SQLITE3="no"])
   AC_SEARCH_LIBS([sqlite3_free],         [sqlite3], [], [HAVE_SQLITE3="no"])
   AC_SEARCH_LIBS([sqlite3_open],         [sqlite3], [], [HAVE_SQLITE3="no"])
   AC_SEARCH_LIBS([sqlite3_prepare],      [sqlite3], [], [HAVE_SQLITE3="no"])
   AC_SEARCH_LIBS([sqlite3_step],         [sqlite3], [], [HAVE_SQLITE3="no"])

   # Apply substitution
   AC_SUBST([HAVE_SQLITE3],     [${HAVE_SQLITE3}])
   AC_SUBST([SQLITE3_CFLAGS],   [${SQLITE3_CFLAGS}])
   AC_SUBST([SQLITE3_CPPFLAGS], [${SQLITE3_CPPFLAGS}])
   AC_SUBST([SQLITE3_LDFLAGS],  [${SQLITE3_LDFLAGS}])
   AC_SUBST([SQLITE3_LIBS],     [${LIBS}])

   # replace old parameters
   CFLAGS=${OLD_CFLAGS}
   CPPFLAGS=${OLD_CPPFLAGS}
   LDFLAGS=${OLD_LDFLAGS}
   LIBS=${OLD_LIBS}

   # runs specified response
   if test "x${HAVE_SQLITE3}" == "xno" && test "x${2}" != "x";then
      :
      $2
   elif test "x${HAVE_SQLITE3}" != "xno" && test "x${1}" != "x";then
      :
      $1
   fi
])dnl


# end of m4 file
