#!/bin/sh
#
#   Bindle Binaries Tools
#   Copyright (C) 2012 Bindle Binaries <syzdek@bindlebinaries.com>.
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
#   build-aux/autogen.sh - runs GNU Autotools to create build environment
#

AUTOGENNAME="`basename ${0}`" || exit $?
BINDLEDIR="`dirname ${0}`" || exit 1
if test "x${1}" == "x";then
   echo "${AUTOGENNAME}: automatically determining source directory"

   # use base directory as starting point
   AUTOSRCDIR="`dirname ${0}`"                  || exit $?
   AUTOSRCDIR="`git rev-parse --show-toplevel`" || exit 1

   # loop while .git/ is just a file which implies the working directory
   # is a submodule.
   while test -f "${AUTOSRCDIR}/.git";do
      AUTOSRCDIR="${AUTOSRCDIR}/.."
      AUTOSRCDIR="`git rev-parse --show-toplevel`" || exit 1
   done

   # verifies that a source directory was defined
   if test "x${AUTOSRCDIR}" == "x";then
      echo "${AUTOGENNAME}: unable to determine source directory" 1>&2
      exit 1;
   fi

   # verifies that a source directory contains a root repository
   if test ! -d "${AUTOSRCDIR}/.git";then
      echo "${AUTOGENNAME}: unable to determine source directory" 1>&2
      exit 1;
   fi

   # verifies that
   if test ! -f "${AUTOSRCDIR}/Makefile.am" || \
      test ! -f "${AUTOSRCDIR}/acinclude.m4" ||
      test ! -f "${AUTOSRCDIR}/configure.ac";then
      echo "${AUTOGENNAME}: unable to find required files." 1>&2
      exit 1;
   fi
else
   AUTOSRCDIR="${1}"
fi
echo "${AUTOGENNAME}: using source directory \"${AUTOSRCDIR}\""

# checks for required files
echo "${AUTOGENNAME}: checking for required files"
if test ! -f "${AUTOSRCDIR}/Makefile.am" || \
   test ! -f "${AUTOSRCDIR}/acinclude.m4" ||
   test ! -f "${AUTOSRCDIR}/configure.ac";then
   echo "${AUTOGENNAME}: unable to find required files." 1>&2
   exit 1;
fi

echo "${AUTOGENNAME}: running \"autoreconf -i -f -I m4 -W all -W error ${AUTOSRCDIR}\""
autoreconf -i -f -I m4 -W all -W error "${AUTOSRCDIR}" || exit $?


echo "${AUTOGENNAME}: rm -rf \"${AUTOSRCDIR}/autom4te.cache\""
rm -rf "${AUTOSRCDIR}/autom4te.cache" || exit $?


if test -d "${AUTOSRCDIR}/build-aux";then
   if test ! -f "${AUTOSRCDIR}/build-aux/autogen.sh";then
      echo "${AUTOGENNAME}: installing autogen.sh script"
      cp "${BINDLEDIR}/autogen.sh" "${AUTOSRCDIR}/build-aux/autogen.sh" || exit 1;
      chmod 755 "${BINDLEDIR}/autogen.sh" || exit 1
   else
      if test -f "${BINDLEDIR}/autogen.sh";then
         diff "${BINDLEDIR}/autogen.sh" "${AUTOSRCDIR}/build-aux/autogen.sh" > /dev/null 2> /dev/null
         if test $? -ne 0;then
            echo "${AUTOGENNAME}: updating autogen.sh script"
            cp "${BINDLEDIR}/autogen.sh" "${AUTOSRCDIR}/build-aux/autogen.sh" || exit 1;
            chmod 755 "${BINDLEDIR}/autogen.sh" || exit 1
         fi
      fi
   fi
fi

# end of script
