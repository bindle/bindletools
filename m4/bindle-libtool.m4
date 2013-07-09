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


# AC_BINDLE_LIBTOOL_VERSION_INFO()
# ______________________________________________________________________________
AC_DEFUN([AC_BINDLE_LIBTOOL_VERSION_INFO],[dnl

   # prerequists
   AC_REQUIRE([AC_PROG_CC])

   ARGV_CURRENT=$1
   ARGV_REVISION=$2
   ARGV_AGE=$3

   # Version Info:
   #    Current    -- the current version number of this API
   #    Revision   -- the revision of the implementation of the API version
   #    Age        -- How many seqential past API versions is supported by
   #                  this implementation
   # Format => Current:Revision:Age
   AC_SUBST([LIB_VERSION_CURRENT],              [${ARGV_CURRENT}])
   AC_SUBST([LIB_VERSION_REVISION],             [${ARGV_REVISION}])
   AC_SUBST([LIB_VERSION_AGE],                  [${ARGV_AGE}])
   AC_SUBST([LIB_VERSION_INFO],                 [${ARGV_CURRENT}:${ARGV_REVISION}:${ARGV_AGE}])
   AC_SUBST([LIB_RELEASE_INFO],                 [${ARGV_CURRENT}])      # used by Win32 DLLs
   AC_DEFINE_UNQUOTED([LIB_VERSION_CURRENT],    [${ARGV_CURRENT}],      [current version number of this API])
   AC_DEFINE_UNQUOTED([LIB_VERSION_REVISION],   [${ARGV_REVISION}],     [revision of the implementation of the API version])
   AC_DEFINE_UNQUOTED([LIB_VERSION_AGE],        [${ARGV_AGE}],          [seqential past API versions supported by this implementation])
   AC_DEFINE_UNQUOTED([LIB_VERSION_INFO],       ["$LIB_VERSION_INFO"],  [libtool version string])
   AC_DEFINE_UNQUOTED([LIB_RELEASE_INFO],       ["$LIB_RELEASE_INFO"],  [Win32 DLL version string])

   AC_MSG_NOTICE([using libtool version info ${LIB_VERSION_INFO}])

])dnl


# end of m4 file
