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
   GIT_VERSION_SCRIPT=""   # Git package version script.
   GIT_PACKAGE_VERSION_BUILD=""  # Git package version build (x.x.x.gbbbbb).
   GPV=""   # Git package version (x.x.x).
   GIT_PACKAGE_BUILD=""   # Git package build (gbbbbb).
   GIT_CACHED_STRING=""   # Cached git package version string.
   GIT_CACHE_FILE=""   # Cached git file.
   GIT_TARBALL_VERSION=""   # Git Tarball Version (x.x.0 or x.x.x.gbbbbb).
   GPN=""   # git package name.
   GOD=""   # git out directory

   # git package version script location
   if test "x$1" != "x";then
      if test -f "${srcdir}/$1";then
         GIT_VERSION_SCRIPT="${srcdir}/$1"
      else
         AC_MSG_WARN([hard coded git package version script does not exist])
      fi
   fi
   if test "x${GIT_VERSION_SCRIPT}" == "x";then
      GIT_VERSION_SCRIPT=${ac_aux_dir}/git-package-version.sh
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
   BINDLEDIR=`dirname ${GIT_VERSION_SCRIPT}`
   BINDLEDIR=`dirname ${BINDLEDIR}`
   AC_REQUIRE([AC_BINDLE])

   # attempt to use script to determine version
   if test "x${GIT_VERSION_SCRIPT}" != "x";then
      GIT_PACKAGE_VERSION_BUILD=$(${GIT_VERSION_SCRIPT} "${srcdir}" "${GOD}" 2> /dev/null)
   fi

   # attempt to fall back to cache files
   if test "x${GIT_PACKAGE_VERSION_BUILD}" == "x";then
      # determines location of cache file
      if test "x${GOD}" != "x" && test -f "${GOD}/git-package-version.txt";then
         GIT_CACHE_FILE="${GOD}/git-package-version.txt"
         
      elif test -f "${srcdir}/build-aux/git-package-version.txt";then
         GIT_CACHE_FILE="${srcdir}/build-aux/git-package-version.txt"
         
      elif test -f "${srcdir}/include/git-package-version.txt";then
         GIT_CACHE_FILE="${srcdir}/include/git-package-version.txt"
         
      elif test -f "${srcdir}/build-aux/git-tar-name.txt";then
         GPN=$(cat "${srcdir}/build-aux/git-tar-name.txt" 2> /dev/null)
         if test -f "${srcdir}/${GPN}/git-package-version.txt";then
            GIT_CACHE_FILE="${srcdir}/${GPN}/git-package-version.txt"
         fi
      fi

      # reads cache file and saves results
      if test "x${GIT_CACHE_FILE}" != "x";then
         GIT_CACHED_STRING=$(cat ${GIT_CACHE_FILE} 2> /dev/null)
      fi
      if test "x${GIT_CACHED_STRING}" != "x";then
         GIT_PACKAGE_VERSION_BUILD=${GIT_CACHED_STRING}
         AC_MSG_NOTICE([using cached git package version])
      fi
   fi

   # Saves data for use in build scripts
   if test "x${GIT_PACKAGE_VERSION_BUILD}" = "x";then
      AC_MSG_WARN([unable to determine package version from Git tags])
   else
      #
      # split version string into components
      GPV=[$(echo ${GIT_PACKAGE_VERSION_BUILD} |sed -e 's/\.g[[:xdigit:]]\{1,\}$//g')]
      GIT_PACKAGE_BUILD=[$(echo ${GIT_PACKAGE_VERSION_BUILD} |sed -e 's/.*\.\(g[[:xdigit:]]\{1,\}\)$/\1/g')]
      GIT_TARBALL_VERSION=[$(echo ${GIT_PACKAGE_VERSION_BUILD} |sed -e 's/\.0.g[[:xdigit:]]\{1,\}$/.0/g')]
      GIT_PACKAGE_MAJOR=[$(echo ${GIT_PACKAGE_VERSION_BUILD} |cut -d. -f1)]
      GIT_PACKAGE_MINOR=[$(echo ${GIT_PACKAGE_VERSION_BUILD} |cut -d. -f2)]
      GIT_PACKAGE_PATCH=[$(echo ${GIT_PACKAGE_VERSION_BUILD} |cut -d. -f3)]
      AC_MSG_NOTICE([using git package version ${GPV} (${GIT_PACKAGE_BUILD})])
      #
      # generate numeric version
      GIT_PACKAGE_VERSION_NUMBER=[$(echo ${GPV} |cut -d. -f1)];
      NUM=[$(echo ${GPV} |cut -d. -f2)];
      if test "x${NUM}" == "x";then
         NUM=0;
      fi
      GIT_PACKAGE_VERSION_NUMBER=[$(printf "${GIT_PACKAGE_VERSION_NUMBER}.%02i" ${NUM})]
      NUM=[$(echo ${GPV} |cut -d. -f3)]
      if test "x${NUM}" == "x";then
         NUM=0;
      fi
      GIT_PACKAGE_VERSION_NUMBER=[$(printf "${GIT_PACKAGE_VERSION_NUMBER}%04i" ${NUM})]
      AC_MSG_NOTICE([using git numeric version ${GIT_PACKAGE_VERSION_NUMBER}])
      #
      # set internal variables
      GIT_PACKAGE_VERSION=${GPV}
      PACKAGE_VERSION=${GIT_TARBALL_VERSION}
      PACKAGE_NUMERIC_VERSION=${GIT_PACKAGE_VERSION_NUMBER}
      VERSION=${GIT_TARBALL_VERSION}
      CONFIG_STATUS_DEPENDENCIES="${GIT_CACHE_FILE} ${CONFIG_STATUS_DEPENDENCIES}"
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
   unset GPV
   unset GIT_CACHED_STRING
   unset GPN
])dnl

# end of m4 file
