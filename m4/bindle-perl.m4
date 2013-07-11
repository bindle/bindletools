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


# AC_BINDLE_WITH_PERL(action-if-true, action-if-false)
# ______________________________________________________________________________
AC_DEFUN([AC_BINDLE_WITH_PERL],[dnl

   # prerequists
   AC_REQUIRE([AC_PROG_CC])

   # display options
   withval=""
   AC_ARG_WITH(
      perl-path,
      [AS_HELP_STRING([--with-perl-path=PATH], [Search for PERL program in PATH.])],
      [ WITH_PERL_PATH=$withval ],
      [ WITH_PERL_PATH=$withval ]
   )
   AC_ARG_VAR(PERL,    [perl interpreter])
   AC_ARG_VAR(PERLDOC, [perldoc utility])

   # checks for perl-path
   PERL_PATH="${PATH}"
   if test "x${WITH_PERL_PATH}" != "x" && \
      test "x${WITH_PERL_PATH}" != "xno" && \
      test "x${WITH_PERL_PATH}" != "xyes";then
      PERL_PATH="${WITH_PERL_PATH}"
   fi

   # search for perl
   SEARCH_PROG="perl"
   SEARCH_PATH="${PERL_PATH}"
   if test "x${PERL}" != "x";then
      SEARCH_PROG="`dirname ${PERL}`"
      SEARCH_PATH="`basedir ${PERL}`"
      if test "x${SEARCH_PATH}" == "x.";then
         SEARCH_PATH="${PERL_PATH}"
      fi
   fi
   AC_PATH_PROG([PERL], [${SEARCH_PROG}], [no], [${SEARCH_PATH}])

   # search for perldoc
   SEARCH_PROG="perldoc"
   SEARCH_PATH="${PERL_PATH}"
   if test "x${PERLDOC}" != "x";then
      SEARCH_PROG="`dirname ${PERL}`"
      SEARCH_PATH="`basedir ${PERLDOC}`"
      if test "x${SEARCH_PATH}" == "x.";then
         SEARCH_PATH="${PERL_PATH}"
      fi
   fi
   AC_PATH_PROG([PERLDOC], [perldoc], [no], [$PERL_PATH])

   # determine if perl binaries were found
   HAVE_PERL="yes"
   if test "x${PERL}" == "xno";then
      HAVE_PERL="no"
   fi
   if test "x${PERLDOC}" == "xno";then
      HAVE_PERL="no"
   fi
   AC_SUBST([HAVE_PERL])

   # runs specified response
   if test "x${HAVE_PERL}" == "xno" && test "x${2}" != "x";then
      :
      $2
   elif test "x${HAVE_PERL}" != "xno" && test "x${1}" != "x";then
      :
      $1
   fi
])dnl

# end of m4 file
