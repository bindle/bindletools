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

# AC_DMS_BINDLE_PACKAGE_VERSION()
# -----------------------------------
AC_DEFUN([AC_BINDLE_GIT_PACKAGE_VERSION],[dnl

   # sets vars to known states
   GSH="" # Git package version script.
   GVS="" # Git package version string (x.x.x.gbbbbb).
   GPV="" # Git package version (x.x.x).
   GPB="" # Git package build (gbbbbb).
   GCS="" # Cached git package version string.
   GCF="" # Cached git file.
   GTV="" # Git Tarball Version (x.x.0 or x.x.x.gbbbbb).
   GPN="" # git package name.
   GOD="" # git out directory

   # git package version script location
   if test "x${1}" != "x";then
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
   if test "x${2}" != "x";then
      if test -d "${2}";then
         GOD="${2}"
      elif test -d "${srcdir}/${2}";then
         GOD="${srcdir}/${2}"
      fi
   fi

   # attempt to use script to determine version
   if test "x${GSH}" != "x";then
      GVS=$(${GSH} "${srcdir}" "${GOD}" 2> /dev/null)
   fi

   # attempt to fall back to cache files
   if test "x${GVS}" == "x";then
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
         GVS=${GCS}
         AC_MSG_NOTICE([using cached git package version])
      fi
   fi
   unset GCS

   # Saves data for use in build scripts
   if test "x${GVS}" = "x";then
      AC_MSG_WARN([unable to determine package version from Git tags])
   else
      #
      # split version string into components
      GPV=[$(echo ${GVS} |sed -e 's/\.g[[:xdigit:]]\{1,\}$//g')]
      GPB=[$(echo ${GVS} |sed -e 's/.*\.\(g[[:xdigit:]]\{1,\}\)$/\1/g')]
      GTV=[$(echo ${GVS} |sed -e 's/\.0.g[[:xdigit:]]\{1,\}$/.0/g')]
      AC_MSG_NOTICE([using git package version ${GPV} (${GPB})])
      #
      # set internal variables
      GIT_VERSION_SCRIPT=${GSH}
      GIT_PACKAGE_STRING=${GVS}
      GIT_PACKAGE_VERSION=${GPV}
      GIT_PACKAGE_BUILD=${GPB}
      GIT_TARBALL_VERSION=${GTV}
      PACKAGE_VERSION=${GTV}
      VERSION=${GTV}
      CONFIG_STATUS_DEPENDENCIES="${GCF} ${CONFIG_STATUS_DEPENDENCIES}"
      #
      # set substitution variables
      AC_SUBST([GIT_VERSION_SCRIPT],    [${GIT_VERSION_SCRIPT}])
      AC_SUBST([GIT_PACKAGE_STRING],    [${GIT_PACKAGE_STRING}])
      AC_SUBST([GIT_PACKAGE_VERSION],   [${GIT_PACKAGE_VERSION}])
      AC_SUBST([GIT_PACKAGE_BUILD],     [${GIT_PACKAGE_BUILD}])
      AC_SUBST([PACKAGE_VERSION],       [${GIT_TARBALL_VERSION}])
      AC_SUBST([VERSION],               [${GIT_TARBALL_VERSION}])
      AC_SUBST([CONFIG_STATUS_DEPENDENCIES], [${CONFIG_STATUS_DEPENDENCIES}])
      #
      # set C/C++/Objc preprocessor macros
      AC_DEFINE_UNQUOTED([GIT_VERSION_SCRIPT],    ["${GIT_VERSION_SCRIPT}"],  [script which determines package version and build from git repository])
      AC_DEFINE_UNQUOTED([GIT_PACKAGE_STRING],    ["${GIT_PACKAGE_STRING}"],  [package version and build determined from git repository])
      AC_DEFINE_UNQUOTED([GIT_PACKAGE_VERSION],   ["${GIT_PACKAGE_VERSION}"], [package version determined from git repository])
      AC_DEFINE_UNQUOTED([GIT_PACKAGE_BUILD],     ["${GIT_PACKAGE_BUILD}"],   [package build determined from git repository])
      AC_DEFINE_UNQUOTED([PACKAGE_VERSION],       ["${GIT_TARBALL_VERSION}"], [package version and optionally build determined from git repository])
      AC_DEFINE_UNQUOTED([VERSION],               ["${GIT_TARBALL_VERSION}"], [package version and optionally build determined from git repository])
   fi

   # clears vars
   unset GSH
   unset GVS
   unset GPV
   unset GPB
   unset GCS
   unset GTV
   unset GPN
])dnl

# end of m4 file
