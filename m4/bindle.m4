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

   # git package version script location
   if test "x${1}" != "x" && test -f "${srcdir}/$1";then
      if test -f "${srcdir}/$1";then
         GSH="${srcdir}/$1"
      else
         AC_MSG_WARN([hard coded git package version script does not exist])
      fi
   fi
   if test "x${GSH}" == "x";then
      GSH=${ac_aux_dir}/git-package-version.sh
   fi

   # determine location of cache file
   if test -f ${srcdir}/build-aux/git-package-version.txt;then
      GCF=${srcdir}/build-aux/git-package-version.txt
   elif test -f ${srcdir}/include/git-package-version.txt;then
      GCF=${srcdir}/include/git-package-version.txt
   elif test -f ${srcdir}/build-aux/git-tar-name.txt;then
      GPN=$(cat ${srcdir}/include/git-package-name.txt 2> /dev/null)
      if test -f ${srcdir}/${GPN}/git-package-version.txt;then
         GCF=${srcdir}/${GPN}/git-package-version.txt
      fi
   fi

   # attempt to use script to determine version and fall back to cache files
   GVS=$(${GSH} ${srcdir} 2> /dev/null)
   if test "x${GVS}" == "x";then
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


# AC_BINDLE_ENABLE_WARNINGS()
# ______________________________________________________________________________
AC_DEFUN([AC_BINDLE_ENABLE_WARNINGS],[dnl

   # prerequists
   AC_REQUIRE([AC_PROG_CC])

   # sets compiler
   if test "x${CC}" == "x";then
      ac_bindle_cc=cc;
   else
      ac_bindle_cc=${CC};
   fi

   # display options
   enableval=""
   AC_ARG_ENABLE(
      debug,
      [AS_HELP_STRING([--enable-debug], [enable debug messages in binaries])],
      [ EDEBUG=$enableval ],
      [ EDEBUG=$enableval ]
   )
   enableval=""
   AC_ARG_ENABLE(
      warnings,
      [AS_HELP_STRING([--disable-warnings], [disable compiler warnings])],
      [ EWARNINGS=$enableval ],
      [ EWARNINGS=$enableval ]
   )
   enableval=""
   AC_ARG_ENABLE(
      strictwarnings,
      [AS_HELP_STRING([--enable-strictwarnings], [disable compiler warnings])],
      [ ESTRICTWARNINGS=$enableval ],
      [ ESTRICTWARNINGS=$enableval ]
   )

   # sets options
   if test "x${ESTRICTWARNINGS}" != "xyes";then
      USE_STRICTWARNINGS=no
   else
      USE_STRICTWARNINGS=yes
   fi
   if test "x${EWARNINGS}" != "xno";then
      USE_WARNINGS=yes
   else
      USE_WARNINGS=no
      USE_STRICTWARNINGS=no
   fi
   if test "x${EDEBUG}" != "xno";then
      USE_DEBUG=yes
      AC_DEFINE_UNQUOTED(USE_DEBUG, 1, [Use debug messages])
   else
      USE_DEBUG=no
   fi

   # list of args
   ac_bindle_warnings_list=""
   if test "x${USE_WARNINGS}" == "xyes";then
      ac_bindle_warnings_list=" ${ac_bindle_warnings_list} -W"
      ac_bindle_warnings_list=" ${ac_bindle_warnings_list} -Wall"
      ac_bindle_warnings_list=" ${ac_bindle_warnings_list} -Werror"
   fi
   if test "x${USE_STRICTWARNINGS}" == "xyes";then
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -std=gnu99"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -pedantic"
      #ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wtraditional"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wpragma-once"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wextra-tokens"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wnewline-eof"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wsequence-point"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wdiv-by-zero"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wfloat-equal"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wdisabled-optimization"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wchar-subscripts"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wcomment"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wformat"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wimplicit"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wimplicit-function-declaration"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wmain"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wmissing-braces"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wparentheses"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wreturn-type"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wswitch"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wswitch-default"   # for some reason any call to memset() fails with this flag
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wtrigraphs"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wunused"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wunknown-pragmas"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wuninitialized"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wmultichar"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wundef"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wendif-labels"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wshadow"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wlarger-than-4096"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wpointer-arith"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wbad-function-cast"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wcast-qual"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wcast-align"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wwrite-strings"
      #ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wconversion" # prototypes with "short" or "unsigned short" cause warning: passing argument ?? with different width due to prototype
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wsign-compare"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Waggregate-return"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wshorten-64-to-32"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wstrict-prototypes"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wstrict-aliasing"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wmissing-prototypes"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wmissing-declarations"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wmissing-noreturn"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wmissing-format-attribute"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wredundant-decls"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wunreachable-code"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Winvalid-pch"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wpacked"
      #ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wpadded"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wnested-externs"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wenum-clash"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Winline"
      ac_bindle_warnings_list="${ac_bindle_warnings_list} -Wlong-long"
   fi

   # creates test source code
   AC_LANG_CONFTEST(
      [AC_LANG_PROGRAM(
         [[int main(void);]],
         [[return(0);]]
      )])

   # loops throough options
   CFLAGS_WARNINGS=""
   for ac_bindle_warning in ${ac_bindle_warnings_list};do
      AC_MSG_CHECKING(for gcc flag ${ac_bindle_warning})
      ${ac_bindle_cc} ${CFLAGS_WARNINGS} ${ac_bindle_warning} conftest.c -o conftest.o > /dev/null 2>&1
      if test "x$?" == "x0";then
         CFLAGS_WARNINGS="${CFLAGS_WARNINGS} ${ac_bindle_warning}"
         AC_MSG_RESULT(yes)
      else
         AC_MSG_RESULT(no)
      fi
   done

   # export variables
   AC_SUBST(CFLAGS_WARNINGS, "$CFLAGS_WARNINGS")
])dnl

# end of m4 file
