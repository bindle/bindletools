#
#   Bindle Binaries Tools
#   Copyright (C) 2012, 2022 Bindle Binaries <syzdek@bindlebinaries.com>.
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
#   @configure_input@
#
#   Makefile.am - automate compiling on a unix platform
#

# Sub directories
SUBDIRS					=
DIST_SUBDIRS				=


# GNU make options
V					= 0


# Global flags
AM_LIBS					=
AM_LDFLAGS				=
AM_CFLAGS				= -O2 $(CFLAGS_WARNINGS)
AM_CXXFLAGS				= -O2 @AM_CXXFLAGS@
AM_OBJCFLAGS				= -O2 @AM_OBJCFLAGS@
AM_CPPFLAGS				= -O2 \
					  -DHAVE_CONFIG_H=1 \
					  -I$(top_builddir)/include \
					  -I$(top_srcdir)/include \
					  -DSYSCONFDIR="\"$(sysconfdir)\""
ACLOCAL_AMFLAGS				= -I m4 -W all -W error
AM_MAKEINFOFLAGS                        = --no-split
DEFS					=


# custom directories


# automake targets
check_PROGRAMS				=
doc_DATA				= AUTHORS.md \
					  ChangeLog.md \
					  COPYING.md \
					  README.md \
					  TODO.md
include_HEADERS				=
lib_LTLIBRARIES				=
lib_LIBRARIES				=
man_MANS				=
info_TEXINFOS				=
noinst_DATA				=
noinst_LTLIBRARIES			=
noinst_LIBRARIES			=
noinst_HEADERS				=
noinst_PROGRAMS				=
noinst_SCRIPTS				=
bin_PROGRAMS				=
bin_SCRIPTS				=
sbin_SCRIPTS				=
EXTRA_PROGRAMS				=
EXTRA					= lib/libbindle.a \
					  lib/libbindle.la \
					  include/bindle.h


# lists
AM_INSTALLCHECK_STD_OPTIONS_EXEMPT	=
BUILT_SOURCES				=
TESTS					=
XFAIL_TESTS				=
EXTRA_MANS				=
EXTRA_DIST				= $(doc_DATA) \
					  autogen.sh \
					  lib/libbindle/libbindle.sym \
					  m4/bindle-doxygen.m4 \
					  m4/bindle-fd-poller.m4 \
					  m4/bindle-gcc.m4 \
					  m4/bindle-git.m4 \
					  m4/bindle-libtool.m4 \
					  m4/bindle-perl.m4 \
					  m4/bindle-postgresql.m4 \
					  m4/bindle-sqlite3.m4 \
					  m4/bindle-xxd.m4 \
					  $(noinst_DATA)
CLEANFILES				= $(builddir)/a.out   $(srcdir)/a.out \
					  $(builddir)/*/a.out $(srcdir)/*/a.out \
					  config.h.in~ $(srcdir)/config.h.in~ \
					  $(man_MANS) \
					  $(bin_SCRIPTS) \
					  $(sbin_SCRIPTS) \
					  $(noinst_SCRIPTS) \
					  $(EXTRA_PROGRAMS) \
					  @PACKAGE_TARNAME@-*.tar.* \
					  @PACKAGE_TARNAME@-*.zip
DISTCHECK_CONFIGURE_FLAGS		= --enable-strictwarnings


# conditional build
if ENABLE_LIBBINDLE
   include_HEADERS			+= include/bindle.h
   lib_LTLIBRARIES			+= lib/libbindle.la
else
   noinst_HEADERS			+= include/bindle.h
   noinst_LIBRARIES			+= lib/libbindle.a
endif


# Makefile includes
GIT_PACKAGE_VERSION_DIR=include
SUBST_EXPRESSIONS =
include build-aux/makefile-subst.am
include build-aux/makefile-version.am
include build-aux/makefile-libbindle.am


# macros for lib/libbindle.a
lib_libbindle_a_DEPENDENCIES		= Makefile \
					  config.h \
					  include/bindle_prefix.h
lib_libbindle_a_CPPFLAGS		= $(AM_CPPFLAGS) \
					  -I$(srcdir)/lib/libbindle
lib_libbindle_a_CFLAGS			= $(AM_CFLAGS)
lib_libbindle_a_SOURCES			= $(libbindle_sources)


# macros for lib/libbindle.la
lib_libbindle_la_DEPENDENCIES		= $(lib_libbindle_a_DEPENDENCIES) \
					  lib/libbindle/libbindle.sym
lib_libbindle_la_CPPFLAGS		= $(AM_CPPFLAGS) \
					  -I$(srcdir)/lib/libbindle
lib_libbindle_la_CFLAGS			= $(AM_CFLAGS)
lib_libbindle_la_LDFLAGS		= -rpath'$(libdir)' \
					  -version-info $(LIB_VERSION_INFO) \
					  $(AM_LDFLAGS) \
					  -export-symbols $(srcdir)/lib/libbindle/libbindle.sym
lib_libbindle_la_SOURCES		= $(libbindle_sources)


# custom targets
.PHONY: git-clean

git-clean:
	git fsck --full --unreachable
	git gc --auto --aggressive


# local targets
install-exec-local:

install-data-local:

uninstall-local:

clean-local:

distclean-local:
	rm -fR $(srcdir)/autom4te.cache

# end of automake file
