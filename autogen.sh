#!/bin/sh
#
#   Bindle Binaries Tools
#   Copyright (c) 2014, Bindle Binaries
#   All rights reserved.
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
#   autogen.sh - runs GNU Autotools to create build environment
#

AUTOGENNAME="`basename ${0}`" || exit 1
SRCDIR="`dirname ${0}`"


# check for required programs
for TEST_PROG in which autoreconf autoscan find git;do
   which ${TEST_PROG} 2> /dev/null > /dev/null;
   if test $? -ne 0;then
      echo "${AUTOGENNAME}: unable to find \"${TEST_PROG}\""
      exit 1
   fi
done


# updates git repository
if test -d ${SRCDIR}/.git || test -f ${SRCDIR}/.git;then
   $(cd ${SRCDIR}; git submodule init)                              || exit 1
   $(cd ${SRCDIR}; git submodule sync)                              || exit 1
   $(cd ${SRCDIR}; git submodule update --init --recursive --merge) || exit 1
fi


# Performs some useful checks
autoscan ${SRCDIR} || exit 1


# generates/installs autotools files
autoreconf -v -i -f -Wall \
   -I m4 \
   -m \
   ${SRCDIR} \
   || exit 1


# makes build directory
mkdir -p ${SRCDIR}/build


# end of script
