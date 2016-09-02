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


# AC_BINDLE_WITH_DOXYGEN(action-if-true, action-if-false)
# ______________________________________________________________________________
AC_DEFUN([AC_BINDLE_WITH_DOXYGEN],[dnl

   # prerequists
   AC_REQUIRE([AC_PROG_CC])

   # display options
   withval=""
   AC_ARG_WITH(
      doxygen-path,
      [AS_HELP_STRING([--with-doxygen-path=PATH], [Search for DOXYGEN program in PATH.])],
      [ WITH_DOXYGEN_PATH=$withval ],
      [ WITH_DOXYGEN_PATH=$withval ]
   )
   AC_ARG_VAR(DOXYGEN,    [doxygen documentation generator])

   # checks for doxygen-path
   DOXYGEN_PATH="${PATH}"
   if test "x${WITH_DOXYGEN_PATH}" != "x" && \
      test "x${WITH_DOXYGEN_PATH}" != "xno" && \
      test "x${WITH_DOXYGEN_PATH}" != "xyes";then
      DOXYGEN_PATH="${WITH_DOXYGEN_PATH}"
   fi

   # search for doxygen
   SEARCH_PROG="doxygen"
   SEARCH_PATH="${DOXYGEN_PATH}"
   if test "x${DOXYGEN}" != "x";then
      SEARCH_PROG="`dirname ${DOXYGEN}`"
      SEARCH_PATH="`basedir ${DOXYGEN}`"
      if test "x${SEARCH_PATH}" == "x.";then
         SEARCH_PATH="${DOXYGEN_PATH}"
      fi
   fi
   AC_PATH_PROG([DOXYGEN], [${SEARCH_PROG}], [no], [${SEARCH_PATH}])

   # determine if doxygen binaries were found
   HAVE_DOXYGEN="yes"
   if test "x${DOXYGEN}" == "xno";then
      HAVE_DOXYGEN="no"
   fi
   AC_SUBST([HAVE_DOXYGEN])

   # runs specified response
   if test "x${HAVE_DOXYGEN}" == "xno" && test "x${2}" != "x";then
      :
      $2
   elif test "x${HAVE_DOXYGEN}" != "xno" && test "x${1}" != "x";then
      :
      $1
   fi
])dnl

# end of m4 file
