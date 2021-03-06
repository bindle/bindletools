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

# AC_BINDLE_GIT_PACKAGE_VERSION(script, outdir)
# -----------------------------------
AC_DEFUN([AC_BINDLE_GIT_PACKAGE_VERSION],[dnl

   # sets vars to known states
   GSH=""   # Git package version script.
   GPVB=""  # Git package version build (x.x.x.gbbbbb).
   GPV=""   # Git package version (x.x.x).
   GPB=""   # Git package build (gbbbbb).
   GCS=""   # Cached git package version string.
   GCF=""   # Cached git file.
   GTV=""   # Git Tarball Version (x.x.0 or x.x.x.gbbbbb).
   GPN=""   # git package name.
   GOD=""   # git out directory

   # git package version script location
   if test "x$1" != "x";then
      if test -f "${srcdir}/$1";then
         GSH="${srcdir}/$1"
      else
         AC_MSG_WARN([hard coded git package version script does not exist])
      fi
   fi
   if test "x${GSH}" == "x";then
      GSH=${ac_aux_dir}/git-package-version.sh
   fi

   # git output directory
   if test "x$2" != "x";then
      if test -d "${2}";then
         GOD="${2}"
      elif test -d "${srcdir}/${2}";then
         GOD="${srcdir}/${2}"
      fi
   fi

   # sets bindletools directory
   BINDLEDIR=`dirname ${GSH}`
   BINDLEDIR=`dirname ${BINDLEDIR}`
   AC_REQUIRE([AC_BINDLE])

   # attempt to use script to determine version
   if test "x${GSH}" != "x";then
      GPVB=$(${GSH} "${srcdir}" "${GOD}" 2> /dev/null)
   fi

   # attempt to fall back to cache files
   if test "x${GPVB}" == "x";then
      # determines location of cache file
      if test "x${GOD}" != "x" && test -f "${GOD}/git-package-version.txt";then
         GCF="${GOD}/git-package-version.txt"
         
      elif test -f "${srcdir}/build-aux/git-package-version.txt";then
         GCF="${srcdir}/build-aux/git-package-version.txt"
         
      elif test -f "${srcdir}/include/git-package-version.txt";then
         GCF="${srcdir}/include/git-package-version.txt"
         
      elif test -f "${srcdir}/build-aux/git-tar-name.txt";then
         GPN=$(cat "${srcdir}/build-aux/git-tar-name.txt" 2> /dev/null)
         if test -f "${srcdir}/${GPN}/git-package-version.txt";then
            GCF="${srcdir}/${GPN}/git-package-version.txt"
         fi
      fi

      # reads cache file and saves results
      if test "x${GCF}" != "x";then
         GCS=$(cat ${GCF} 2> /dev/null)
      fi
      if test "x${GCS}" != "x";then
         GPVB=${GCS}
         AC_MSG_NOTICE([using cached git package version])
      fi
   fi
   unset GCS

   # Saves data for use in build scripts
   if test "x${GPVB}" = "x";then
      AC_MSG_WARN([unable to determine package version from Git tags])
   else
      #
      # split version string into components
      GPV=[$(echo ${GPVB} |sed -e 's/\.g[[:xdigit:]]\{1,\}$//g')]
      GPB=[$(echo ${GPVB} |sed -e 's/.*\.\(g[[:xdigit:]]\{1,\}\)$/\1/g')]
      GTV=[$(echo ${GPVB} |sed -e 's/\.0.g[[:xdigit:]]\{1,\}$/.0/g')]
      GMAJ=[$(echo ${GPVB} |cut -d. -f1)]
      GMIN=[$(echo ${GPVB} |cut -d. -f2)]
      GPAT=[$(echo ${GPVB} |cut -d. -f3)]
      AC_MSG_NOTICE([using git package version ${GPV} (${GPB})])
      #
      # generate numeric version
      GPVN=[$(echo ${GPV} |cut -d. -f1)];
      NUM=[$(echo ${GPV} |cut -d. -f2)];
      if test "x${NUM}" == "x";then
         NUM=0;
      fi
      GPVN=[$(printf "${GPVN}.%02i" ${NUM})]
      NUM=[$(echo ${GPV} |cut -d. -f3)]
      if test "x${NUM}" == "x";then
         NUM=0;
      fi
      GPVN=[$(printf "${GPVN}%04i" ${NUM})]
      AC_MSG_NOTICE([using git numeric version ${GPVN}])
      #
      # set internal variables
      GIT_VERSION_SCRIPT=${GSH}
      GIT_PACKAGE_VERSION=${GPV}
      GIT_PACKAGE_MAJOR=${GMAJ}
      GIT_PACKAGE_MINOR=${GMIN}
      GIT_PACKAGE_PATCH=${GPAT}
      GIT_PACKAGE_VERSION_BUILD=${GPVB}
      GIT_PACKAGE_VERSION_NUMBER=${GPVN}
      GIT_PACKAGE_BUILD=${GPB}
      GIT_TARBALL_VERSION=${GTV}
      PACKAGE_VERSION=${GTV}
      PACKAGE_NUMERIC_VERSION=${GPVN}
      VERSION=${GTV}
      CONFIG_STATUS_DEPENDENCIES="${GCF} ${CONFIG_STATUS_DEPENDENCIES}"
      #
      # set substitution variables
      AC_SUBST([GIT_VERSION_SCRIPT],            [${GIT_VERSION_SCRIPT}])
      AC_SUBST([GIT_PACKAGE_VERSION],           [${GIT_PACKAGE_VERSION}])
      AC_SUBST([GIT_PACKAGE_MAJOR],             [${GIT_PACKAGE_MAJOR}])
      AC_SUBST([GIT_PACKAGE_MINOR],             [${GIT_PACKAGE_MINOR}])
      AC_SUBST([GIT_PACKAGE_PATCH],             [${GIT_PACKAGE_PATCH}])
      AC_SUBST([GIT_PACKAGE_VERSION_BUILD],     [${GIT_PACKAGE_VERSION_BUILD}])
      AC_SUBST([GIT_PACKAGE_VERSION_NUMBER],    [${GIT_PACKAGE_VERSION_NUMBER}])
      AC_SUBST([GIT_PACKAGE_BUILD],             [${GIT_PACKAGE_BUILD}])
      AC_SUBST([PACKAGE_VERSION],               [${PACKAGE_VERSION}])
      AC_SUBST([VERSION],                       [${VERSION}])
      AC_SUBST([CONFIG_STATUS_DEPENDENCIES],    [${CONFIG_STATUS_DEPENDENCIES}])
      #
      # set C/C++/Objc preprocessor macros
      AC_DEFINE_UNQUOTED([GIT_VERSION_SCRIPT],           ["${GIT_VERSION_SCRIPT}"],          [script which determines package version and build from git repository])
      AC_DEFINE_UNQUOTED([GIT_PACKAGE_VERSION],          ["${GIT_PACKAGE_VERSION}"],         [package version determined from git repository (x.y.z)])
      AC_DEFINE_UNQUOTED([GIT_PACKAGE_VERSION_BUILD],    ["${GIT_PACKAGE_VERSION_BUILD}"],   [package version and build determined from git repository (x.y.z.b)])
      AC_DEFINE_UNQUOTED([GIT_PACKAGE_MAJOR],            [${GIT_PACKAGE_MAJOR}],             [package major version from git repository (x)])
      AC_DEFINE_UNQUOTED([GIT_PACKAGE_MINOR],            [${GIT_PACKAGE_MINOR}],             [package minor version from git repository (y)])
      AC_DEFINE_UNQUOTED([GIT_PACKAGE_PATCH],            [${GIT_PACKAGE_PATCH}],             [package patch version from git repository (z)])
      AC_DEFINE_UNQUOTED([GIT_PACKAGE_VERSION_NUMBER],   [${GIT_PACKAGE_VERSION_NUMBER}f],   [package version number determined from git repository (xx.yyzzzz)])
      AC_DEFINE_UNQUOTED([GIT_PACKAGE_BUILD],            ["${GIT_PACKAGE_BUILD}"],           [package build determined from git repository (b)])
      AC_DEFINE_UNQUOTED([PACKAGE_VERSION],              ["${PACKAGE_VERSION}"],             [package version and optionally build determined from git repository])
      AC_DEFINE_UNQUOTED([VERSION],                      ["${VERSION}"],                     [package version and optionally build determined from git repository])
   fi

   # clears vars
   unset GSH
   unset GPVB
   unset GPV
   unset GPB
   unset GCS
   unset GTV
   unset GPN
])dnl

# end of m4 file
