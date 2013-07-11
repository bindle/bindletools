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
#   m4/bindle-postgresql.m4 - m4 macros for find PostgreSQL client library
#                             and CLI utility.
#


# AC_BINDLE_WITH_POSTGRESQL(action-if-true, action-if-false)
# ______________________________________________________________________________
AC_DEFUN([AC_BINDLE_WITH_POSTGRESQL],[dnl

   # prerequists
   AC_REQUIRE([AC_PROG_CC])

   # set initial values
   HAVE_POSTGRESQL="yes"
   POSTGRESQL_CFLAGS=""
   POSTGRESQL_CPPFLAGS=""
   POSTGRESQL_LDFLAGS=""
   POSTGRESQL_LIBS=""
   POSTGRESQL_PATH="${PATH}"

   # display options
   withval=""
   AC_ARG_WITH(
      pg-includedir,
      [AS_HELP_STRING([--with-pg-includedir=DIR], [Search for PostgreSQL headers in DIR.])],
      [ WITH_PG_INCLUDEDIR=$withval ],
      [ WITH_PG_INCLUDEDIR=$withval ]
   )
   withval=""
   AC_ARG_WITH(
      pg-libdir,
      [AS_HELP_STRING([--with-pg-libdir=DIR], [Search for PostgreSQL libraries in DIR.])],
      [ WITH_PG_LIBDIR=$withval ],
      [ WITH_PG_LIBDIR=$withval ]
   )
   withval=""
   AC_ARG_WITH(
      pg-path,
      [AS_HELP_STRING([--with-pg-path=PATH], [Search for PostgreSQL utilities in PATH.])],
      [ WITH_PG_PATH=$withval ],
      [ WITH_PG_PATH=$withval ]
   )

   # checks for pg-includedir
   if test "x${WITH_PG_INCLUDEDIR}" != "x" && \
      test "x${WITH_PG_INCLUDEDIR}" != "xno" && \
      test "x${WITH_PG_INCLUDEDIR}" != "xyes";then
      POSTGRESQL_CPPFLAGS="-I${WITH_PG_INCLUDEDIR}"
   fi

   # checks for pg-libdir
   if test "x${WITH_PG_LIBDIR}" != "x" && \
      test "x${WITH_PG_LIBDIR}" != "xno" && \
      test "x${WITH_PG_LIBDIR}" != "xyes";then
      POSTGRESQL_LDLAGS="-L${WITH_PG_LIBDIR}"
   fi

   # checks for pg-path
   if test "x${WITH_PG_PATH}" != "x" && \
      test "x${WITH_PG_PATH}" != "xno" && \
      test "x${WITH_PG_PATH}" != "xyes";then
      POSTGRESQL_PATH="${WITH_PG_PATH}"
   fi

   # checks for utilities
   AC_PATH_PROG([PG_CONFIG], [pg_config], [pg_config], [${POSTGRESQL_PATH}])
   AC_PATH_PROG([PSQL],      [psql],      [psql],      [${POSTGRESQL_PATH}])

   # saves old parameters
   OLD_CFLAGS=${CFLAGS}
   OLD_CPPFLAGS=${CPPFLAGS}
   OLD_LDFLAGS=${LDFLAGS}
   OLD_LIBS=${LIBS}

   # set new compile flags
   CFLAGS="${OLD_CFLAGS} ${POSTGRESQL_CFLAGS}"
   CPPFLAGS="${OLD_CPPFLAGS} ${POSTGRESQL_CPPFLAGS}"
   LDFLAGS="${OLD_LDFLAGS} ${POSTGRESQL_LDFLAGS}"
   LIBS="${OLD_LIBS} ${POSTGRESQL_LIBS}"

   # check for headers and library
   AC_CHECK_HEADERS([libpq-fe.h],         [], [HAVE_POSTGRESQL="no"])
   AC_SEARCH_LIBS([PQclear],        [pq], [], [HAVE_POSTGRESQL="no"])
   AC_SEARCH_LIBS([PQconnectdb],    [pq], [], [HAVE_POSTGRESQL="no"])
   AC_SEARCH_LIBS([PQerrorMessage], [pg], [], [HAVE_POSTGRESQL="no"])
   AC_SEARCH_LIBS([PQexec],         [pg], [], [HAVE_POSTGRESQL="no"])
   AC_SEARCH_LIBS([PQfinish],       [pg], [], [HAVE_POSTGRESQL="no"])
   AC_SEARCH_LIBS([PQgetvalue],     [pg], [], [HAVE_POSTGRESQL="no"])
   AC_SEARCH_LIBS([PQnfields],      [pg], [], [HAVE_POSTGRESQL="no"])
   AC_SEARCH_LIBS([PQntuples],      [pg], [], [HAVE_POSTGRESQL="no"])
   AC_SEARCH_LIBS([PQstatus],       [pg], [], [HAVE_POSTGRESQL="no"])
   AC_SEARCH_LIBS([PQresultStatus], [pg], [], [HAVE_POSTGRESQL="no"])

   # Apply substitution
   AC_SUBST([HAVE_POSTGRESQL],     [${HAVE_POSTGRESQL}])
   AC_SUBST([POSTGRESQL_CFLAGS],   [${POSTGRESQL_CFLAGS}])
   AC_SUBST([POSTGRESQL_CPPFLAGS], [${POSTGRESQL_CPPFLAGS}])
   AC_SUBST([POSTGRESQL_LDFLAGS],  [${POSTGRESQL_LDFLAGS}])
   AC_SUBST([POSTGRESQL_LIBS],     [${LIBS}])

   # replace old parameters
   CFLAGS=${OLD_CFLAGS}
   CPPFLAGS=${OLD_CPPFLAGS}
   LDFLAGS=${OLD_LDFLAGS}
   LIBS=${OLD_LIBS}

   # runs specified response
   if test "x${HAVE_POSTGRESQL}" == "xno" && test "x${2}" != "x";then
      :
      $2
   elif test "x${HAVE_POSTGRESQL}" != "xno" && test "x${1}" != "x";then
      :
      $1
   fi
])dnl


# end of m4 file
