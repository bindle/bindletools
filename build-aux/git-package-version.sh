#!/bin/sh
#
#   Bindle Binaries Tools
#   Copyright (c) 2012, Bindle Binaries
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
#   build-aux/git-package-version.sh - determines Git version if available
#


# saves srcdir from command line arguments
if test "x${1}" == "x";then
   echo "Usage: ${0} srcdir [ savedir ]" 1>&2;
   echo "Usage: ${0} auto" 1>&2;
   exit 1;
fi;
SRCDIR=$1;


# determines script directory
SCRIPT_DIR=`dirname ${0}`
SCRIPT_DIR=`echo ${SCRIPT_DIR} |sed -e 's/build-aux\/.*$/build-aux/g' -e 's/\/$//g'`


# determines top of Git directory
GIT_TOP_DIR=`git rev-parse --show-toplevel 2> /dev/null`
if test "x${GIT_TOP_DIR}" == "x" && test "x${SRCDIR}" == "xauto";then
   GIT_TOP_DIR=`( cd ${SRCDIR} 2> /dev/null && git rev-parse --show-toplevel 2> /dev/null; )`
fi
if test "x${GIT_TOP_DIR}" == "x" && test "x${SRCDIR}" == "xauto";then
   echo "${0}: unable to determine top level of git repository." 1>&2;
   exit 0;
elif test "x${GIT_TOP_DIR}" == "x";then
   GIT_TOP_DIR=${SRCDIR}
fi


# auto determines source directory
if test "x${SRCDIR}" == "xauto";then
   # determines root of source by root of Git repo
   SRCDIR="${GIT_TOP_DIR}"
fi
if test "x${SRCDIR}" == "x";then
   echo "${0}: unable to detect root of source directory." 1>&2;
   exit 1;
fi


# determines build-aux directory
if test -d "${SRCDIR}/build-aux";then
   BUILD_AUX="${SRCDIR}/build-aux"
else
   BUILD_AUX="${SCRIPT_DIR}"
fi


# determines package tar name
if test "x${PACKAGE_TARNAME}" == "x";then
   if test -f "${BUILD_AUX}/git-tar-name.txt";then
      PACKAGE_TARNAME=`cat "${BUILD_AUX}/git-tar-name.txt" 2> /dev/null`
   elif test -f "${SRCDIR}/git-tar-name.txt";then
      PACKAGE_TARNAME=`cat "${SRCDIR}/git-tar-name.txt" 2> /dev/null`
   elif test -f "${SCRIPT_DIR}/git-tar-name.txt";then
      TMP=`( cd ${SCRIPT_DIR} 2> /dev/null && git rev-parse --show-toplevel 2> /dev/null; )`
      if test "x${TMP}" == "x{GIT_TOP_DIR}";then
         PACKAGE_TARNAME=`cat "${SCRIPT_DIR}/git-tar-name.txt" 2> /dev/null`
      fi
   fi
   if test "x${PACKAGE_TARNAME}" == "x";then
      echo "${0}: WARNING: missing \"${BUILD_AUX}/git-tar-name.txt\"" 1>&2;
      echo "${0}: WARNING: It is advised to create a \"git-tar-name.txt\"" 1>&2;
      echo "${0}: WARNING: file containing the tar name of the package." 1>&2;
   fi
fi
if test "x${PACKAGE_TARNAME}" == "x";then
   PACKAGE_TARNAME=`basename "${GIT_TOP_DIR}"`
fi


# determines output dir
if test "x${2}" != "x";then
   OUTDIR="${2}"
   if test ! -d "${OUTDIR}";then
      echo "${0}: ${OUTDIR}: directory does not exist" 1>&2;
      exit 1;
   fi
else
   # attempt "srcdir/include" for headers
   if test -d "${SRCDIR}/include";then
      OUTDIR="${SRCDIR}/include"

   # attempt "srcdir/PackageName" for headers
   elif test -d "${SRCDIR}/${PACKAGE_TARNAME}";then
      OUTDIR="${SRCDIR}/${PACKAGE_TARNAME}"

   # attempt "srcdir/build-aux" for headers
   elif test -d "${SRCDIR}/build-aux";then
      OUTDIR="${SRCDIR}/build-aux"

   # use script directory for headers
   else
      OUTDIR=`dirname ${0}`
      OUTDIR=`echo ${OUTDIR} |sed -e 's/build-aux\/.*$/build-aux/g' -e 's/\/$//g'`
      if test ! -d "${OUTDIR}";then
         echo "${0}: unable to determine output directory." 1>&2
         exit 1;
      fi
   fi
fi


# clears git information
GDR="" # Git describe output.
GVS="" # Git package version string (x.x.x.gCCC).
GPV="" # Git package version (x.x.x).
GPB="" # Git package build (gCCC).
GCS="" # Cached git package version string.


# set default file names
if test "x${GIT_VERSION_FILE}" == "x";then
   GIT_VERSION_FILE="${OUTDIR}/git-package-version.txt"
fi
if test "x${GIT_VERSION_HEADER}" == "x";then
   GIT_VERSION_HEADER="${OUTDIR}/git-package-version.h"
fi
if test "x${GIT_VERSION_PREFIX_HEADER}" == "x";then
   GIT_VERSION_PREFIX_HEADER="${OUTDIR}/git-package-version-prefix.h"
fi


# sets default directories
GIT_VERSION_FILE_DIR=`dirname ${GIT_VERSION_FILE}`
GIT_VERSION_HEADER_DIR=`dirname ${GIT_VERSION_HEADER}`
GIT_VERSION_PREFIX_HEADER_DIR=`dirname ${GIT_VERSION_PREFIX_HEADER}`


# reads cached version
if test -f "${GIT_VERSION_FILE}";then
   GCS=`cat "${GIT_VERSION_FILE}" 2> /dev/null`
fi


# retrieve raw output of git describe
if test -f ${SRCDIR}/.git || test -d ${SRCDIR}/.git;then
   GDR=`git --git-dir=${SRCDIR}/.git describe --long --abbrev=7 HEAD 2> /dev/null`;
   GVS=`echo ${GDR} |sed -e 's/-/./g' -e 's/^v//g'`;
   GPV=`echo ${GVS} |sed -e 's/\.g[[:xdigit:]]\{0,\}$//g' -e 's/\.0$//g'`;
   GPB=`echo ${GVS} |sed -e 's/.*\.\(g[[:xdigit:]]\{1,\}\)$/\1/g'`;

   # write data to file and display results
   if test "x${GVS}" != "x" && test "x${GVS}" != "x${GCS}";then
      rm -f ${GIT_VERSION_FILE} ${GIT_VERSION_HEADER} ${GIT_VERSION_PREFIX_HEADER}

      # writes git version file
      if test -d ${GIT_VERSION_FILE_DIR};then
         echo "${GVS}" > "${GIT_VERSION_FILE}" 2>&1;
      fi

      # writes git version C header
      if test -d ${GIT_VERSION_HEADER_DIR};then
         echo "#define GIT_PACKAGE_VERSION     \"${GVS}\""  > "${GIT_VERSION_HEADER}" 2>&1;
         echo "#define GIT_APPLICATION_VERSION \"${GPV}\"" >> "${GIT_VERSION_HEADER}" 2>&1;
         echo "#define GIT_BUILD_VERSION       \"${GPB}\"" >> "${GIT_VERSION_HEADER}" 2>&1;
      fi

      # writes git version Info.plist preprocessor prefix file
      if test -d ${GIT_VERSION_PREFIX_HEADER_DIR};then
         echo "#define GIT_PACKAGE_VERSION     ${GVS}"  > "${GIT_VERSION_PREFIX_HEADER}" 2>&1;
         echo "#define GIT_APPLICATION_VERSION ${GPV}" >> "${GIT_VERSION_PREFIX_HEADER}" 2>&1;
         echo "#define GIT_BUILD_VERSION       ${GPB}" >> "${GIT_VERSION_PREFIX_HEADER}" 2>&1;
      fi
   fi;
fi


# displays summary
if test "x${GVS}" != "x";then
   echo "${GVS}";
elif test "x${GCS}" != "x";then
   echo "${GCS}";
else
   echo "${GVS}";
fi


# end of script
