#
#   Bindle Binaries Tools
#   Copyright (C) 2011 Bindle Binaries <syzdek@bindlebinaries.com>.
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


# AC_BINDLE(srcdir)
# -----------------------------------
AC_DEFUN_ONCE([AC_BINDLE],[dnl

   AC_REQUIRE([AC_PROG_INSTALL])
   AC_REQUIRE([AC_PROG_MKDIR_P])
   AC_REQUIRE([AC_PROG_SED])

   # determines location of Bindle Tools directory
   bindletools_srcdir=""
   AC_MSG_CHECKING([for location of Bindle Tools directory])
   if test "x$1" != "x" && test -f "$srcdir/$1/build-aux/git-package-version.sh";then
      bindletools_srcdir=$1
   elif test "x$BINDLEDIR" != "x" && test -f "$BINDLEDIR/build-aux/git-package-version.sh";then
      bindletools_srcdir=$BINDLEDIR
   elif test -f "$srcdir/build-aux/git-package-version.sh";then
      bindletools_srcdir=.
   elif test -f "$srcdir/contrib/bindletools/build-aux/git-package-version.sh";then
      bindletools_srcdir=contrib/bindletools
   fi
   if test "x$bindletools_srcdir" == "x";then
      AC_MSG_RESULT([not found])
      AC_MSG_ERROR([Unable to determine location of bindletools directory.])
   fi
   AC_MSG_RESULT([$bindletools_srcdir])
   AC_SUBST([bindletools_srcdir], [$bindletools_srcdir])

])dnl

# end of m4 file
