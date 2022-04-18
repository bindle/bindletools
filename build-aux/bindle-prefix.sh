#!/bin/sh
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

SCRIPT_NAME="$(basename "${0}")"
SRCDIR="$(dirname "${0}")/.."
BINDLE_INCLUDE="${1}"
BINDLE_PREFIX="${2}"


bindle_symbol_list()
{
   egrep \
      '^[[:space:]]{0,1}bindle_[_[:alnum:]]{1,}[\(\;][[:space:]]{0,}$' \
      $(find "${SRCDIR}/include" "${SRCDIR}/lib/libbindle" -name '*.h') \
      |cut -d: -f2 \
      |sed -e 's/[[:space:]]//g' -e 's/[(;]$//g' -e 's/^bindle_//g' \
      |sort \
      |uniq
}


if test $# -lt 1 || test $# -gt 2;then
   echo "Usage: ${SCRIPT_NAME} <file> [ <prefix> ] 1" 1>&2
   exit
fi
if test -z "${1}";then
   echo "Usage: ${SCRIPT_NAME} <file> [ <prefix> ] 2" 1>&2
   exit
fi
if test "${BINDLE_PREFIX}" = "bindle_";then
   BINDLE_PREFIX=""
fi


rm -f "${BINDLE_INCLUDE}" || exit 1
mkdir -p "$(dirname "${BINDLE_INCLUDE}")" || exit 1
{
   echo "/* generated from include/bindle.h by make*/";
   echo "#ifndef __BINDLE_H";
   if test "x${BINDLE_PREFIX}" != "x";then
      for FUNC in `bindle_symbol_list`;do
         echo "#undef bindle_${FUNC}";
         echo "#define bindle_${FUNC} ${BINDLE_PREFIX}${FUNC}";
      done;
   fi;
   echo "#endif";
   cat "${SRCDIR}/include/bindle.h";
} > "${BINDLE_INCLUDE}"
touch "${BINDLE_INCLUDE}"  || exit 1


# end of script
