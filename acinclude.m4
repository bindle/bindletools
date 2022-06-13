#
#   Bindle Binaries Tools
#   Copyright (C) 2012, 2022 Bindle Binaries <syzdek@bindlebinaries.com>.
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
#   acinclude.m4 - custom m4 macros used by configure.ac
#

# AC_BINDLETOOLS_DOCUMENTATION()
# ______________________________________________________________________________
AC_DEFUN([AC_BINDLETOOLS_DOCUMENTATION],[dnl
   enableval=""
   AC_ARG_ENABLE(
      documentation,
      [AS_HELP_STRING([--enable-documentation], [install extra documentation])],
      [ EDOCUMENATION=$enableval ],
      [ EDOCUMENATION=$enableval ]
   )

   if test "x${EDOCUMENATION}" == "xyes";then
      ENABLE_DOCUMENATION="yes"
   else
      ENABLE_DOCUMENATION="no"
   fi

   AM_CONDITIONAL([ENABLE_DOCUMENATION],  [test "$ENABLE_DOCUMENATION" = "yes"])
   AM_CONDITIONAL([DISABLE_DOCUMENATION], [test "$ENABLE_DOCUMENATION" = "no"])
])dnl


# AC_BINDLETOOLS_EXAMPLES()
# ______________________________________________________________________________
AC_DEFUN([AC_BINDLETOOLS_EXAMPLES],[dnl
   enableval=""
   AC_ARG_ENABLE(
      examples,
      [AS_HELP_STRING([--enable-examples], [build tinyrad examples])],
      [ EEXAMPLES=$enableval ],
      [ EEXAMPLES=$enableval ]
   )

   if test "x${EEXAMPLES}" == "xyes";then
      ENABLE_EXAMPLES="yes"
   else
      ENABLE_EXAMPLES="no"
   fi

   AM_CONDITIONAL([ENABLE_EXAMPLES],  [test "$ENABLE_EXAMPLES" = "yes"])
   AM_CONDITIONAL([DISABLE_EXAMPLES], [test "$ENABLE_EXAMPLES" = "no"])
])dnl


# AC_BINDLETOOLS_LIBBINDLE()
# -----------------------------------
AC_DEFUN([AC_BINDLETOOLS_LIBBINDLE],[dnl

   enableval=""
   AC_ARG_ENABLE(
      libbindle,
      [AS_HELP_STRING([--disable-libbindle], [disable bindletools library])],
      [ ELIBBINDLE=$enableval ],
      [ ELIBBINDLE=$enableval ]
   )

   if test "x${ELIBBINDLE}" = "xno";then
      ENABLE_LIBBINDLE="no"
   else
      ENABLE_LIBBINDLE="yes"
   fi

   AM_CONDITIONAL([ENABLE_LIBBINDLE],  [test "$ENABLE_LIBBINDLE" = "yes"])
   AM_CONDITIONAL([DISABLE_LIBBINDLE], [test "$ENABLE_LIBBINDLE" = "no"])
])dnl


# end of m4 file
