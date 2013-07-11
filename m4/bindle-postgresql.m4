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
#   m4/bindle.m4 - m4 macros used by configure.ac
#


# AC_BINDLE_WITH_POSTGRESQL(action-if-true, action-if-false)
# ______________________________________________________________________________
AC_DEFUN([AC_BINDLE_WITH_POSTGRESQL],[dnl

   # prerequists
   AC_REQUIRE([AC_PROG_CC])

   # saves old parameters
   OLD_CFLAGS=${CFLAGS}
   OLD_CPPFLAGS=${CPPFLAGS}
   OLD_LDFLAGS=${LDFLAGS}
   OLD_LIBS=${LIBS}

   # set initial values
   HAVE_POSTGRESQL="yes"
   POSTGRESQL_CFLAGS=""
   POSTGRESQL_CPPFLAGS=""
   POSTGRESQL_LDFLAGS=""
   POSTGRESQL_LIBS=""

   # display options
   withval=""
   AC_ARG_WITH(
      pg-config,
      [AS_HELP_STRING([--with-pg-config=PROG], [Search for PostgreSQL pg_config in PROG.])],
      [ WITH_PG_CONFIG=$withval ],
      [ WITH_PG_CONFIG=$withval ]
   )
   withval=""
   AC_ARG_WITH(
      psql,
      [AS_HELP_STRING([--with-psql=PROG], [Search for PostgreSQL CLI client in PROG.])],
      [ WITH_PSQL=$withval ],
      [ WITH_PSQL=$withval ]
   )

   # checks for pg_config
   if test "x${WITH_PG_CONFIG}" == "x" || \
      test "x${WITH_PG_CONFIG}" == "xno" || \
      test "x${WITH_PG_CONFIG}" == "xyes";then
      AC_PATH_PROG([PG_CONFIG], [pg_config], [])
   else
      PG_CONFIG_PROG=`basename ${WITH_PG_CONFIG}`
      PG_CONFIG_DIR=`dirname ${WITH_PG_CONFIG}`
      if test "x${PG_CONFIG_DIR}" == "x.";then
         PG_CONFIG_DIR=${PATH}
      fi
      AC_PATH_PROG([PG_CONFIG], [${PG_CONFIG_PROG}], [], [${PG_CONFIG_DIR}])
   fi
   if test "x${PG_CONFIG}" == "x";then
      HAVE_POSTGRESQL="no"
   fi

   # checks for psql
   if test "x${WITH_PSQL}" == "x" || \
      test "x${WITH_PSQL}" == "xno" || \
      test "x${WITH_PSQL}" == "xyes";then
      AC_PATH_PROG([PSQL], [psql], [])
   else
      PSQL_PROG=`basename ${WITH_PSQL}`
      PSQL_DIR=`dirname ${WITH_PSQL}`
      if test "x${PG_CONFIG_DIR}" == "x.";then
         PSQL_DIR=${PATH}
      fi
      AC_PATH_PROG([PSQL], [${PSQL_PROG}], [], [${PSQL_DIR}])
   fi
   if test "x${PSQL}" == "x";then
      HAVE_POSTGRESQL="no"
   fi

   if test "x${HAVE_POSTGRESQL}" == "xyes";then
      POSTGRESQL_CFLAGS="`${PG_CONFIG} --cflags`"
      POSTGRESQL_CPPFLAGS="-I`${PG_CONFIG} --includedir` `${PG_CONFIG} --cppflags`"
      POSTGRESQL_LDFLAGS="-L`${PG_CONFIG} --libdir` `${PG_CONFIG} --ldflags`"
      POSTGRESQL_LIBS="`${PG_CONFIG} --libs |sed -e 's/-lpgport//g'`"

      CFLAGS="${POSTGRESQL_CFLAGS}"
      CPPFLAGS="${POSTGRESQL_CPPFLAGS}"
      LDFLAGS="${POSTGRESQL_LDFLAGS}"
      LIBS="${POSTGRESQL_LIBS}"

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
   fi

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
