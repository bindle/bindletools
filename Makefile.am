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
#   @configure_input@
#
#   Makefile.am - automate compiling on a unix platform
#

# Sub directories
SUBDIRS					=
DIST_SUBDIRS				=


# Global flags
AM_LIBS					=
AM_LDFLAGS				= -L$(top_builddir)/lib
AM_CFLAGS				= -O2 $(CFLAGS_WARNINGS)
AM_CXXFLAGS				= -O2 @AM_CXXFLAGS@
AM_OBJCFLAGS				= -O2 @AM_OBJCFLAGS@
AM_CPPFLAGS				= -O2 -UPMARK -DHAVE_CONFIG_H=1 -I$(top_builddir)/include -I$(top_srcdir)/include
ACLOCAL_AMFLAGS				= -I m4 -W all -W error
AM_MAKEINFOFLAGS                        = --no-split
DEFS					=


# custom directories
bindleincludedir			= $(includedir)/bindle


# automake targets
check_PROGRAMS				=
doc_DATA				=
include_HEADERS				= include/bindle.h
bindleinclude_HEADERS			= \
					  include/bindle/cdefs.h \
					  include/bindle/errno.h \
					  include/bindle/fd.h \
					  include/bindle/utf8.h \
					  include/bindle/regex.h \
					  include/bindle/version.h
lib_LTLIBRARIES				= lib/libbindle.la
man_MANS				=
info_TEXINFOS				=
noinst_DATA				= \
					  tests/utf8/ascii.txt \
					  tests/utf8/invalid-continuation.txt \
					  tests/utf8/invalid-length.txt \
					  tests/utf8/mixed.txt \
					  tests/utf8/null.txt \
					  tests/utf8/utf8.txt
noinst_LTLIBRARIES			=
noinst_HEADERS				=
noinst_PROGRAMS				= tests/utf8/utf8test
noinst_SCRIPTS				= tests/utf8/ascii.sh \
					  tests/utf8/invalid-continuation.sh \
					  tests/utf8/invalid-length.sh \
					  tests/utf8/mixed.sh \
					  tests/utf8/null.sh \
					  tests/utf8/utf8.sh
bin_PROGRAMS				=
bin_SCRIPTS				=
sbin_SCRIPTS				=


# lists
PERL_FILES				= lib/perl-bindletools/Changes \
					  lib/perl-bindletools/MANIFEST \
					  lib/perl-bindletools/Makefile.PL \
					  lib/perl-bindletools/README \
					  lib/perl-bindletools/lib/Bindle/CGI.pm \
					  lib/perl-bindletools/lib/Bindle/Version.pm.in \
					  lib/perl-bindletools/src/vars.pl \
					  lib/perl-bindletools/t/CGI.t \
					  lib/perl-bindletools/t/Version.t
BUILT_SOURCES				=
TESTS					= $(noinst_SCRIPTS)
XFAIL_TESTS				=
EXTRA_MANS				=
EXTRA_DIST				= \
					  lib/libbindle/libbindle.sym \
					  m4/bindle-gcc.m4 \
					  m4/bindle-git.m4 \
					  m4/bindle-libtool.m4 \
					  m4/bindle-perl.m4 \
					  m4/bindle-postgresql.m4 \
					  m4/bindle-sqlite3.m4 \
					  m4/bindle-xxd.m4 \
					  tests/utf8/utf8test.sh \
					  $(PERL_FILES) \
					  $(noinst_DATA)
CLEANFILES				= \
					  $(builddir)/a.out   $(srcdir)/a.out \
					  $(builddir)/*/a.out $(srcdir)/*/a.out \
					  config.h.in~ $(srcdir)/config.h.in~ \
					  lib/perl-bindletools/built-by-make \
					  lib/perl-bindletools/lib/Bindle/Version.pm \
					  $(srcdir)/lib/perl-bindletools/lib/Bindle/Version.pm \
					  $(man_MANS) \
					  $(bin_SCRIPTS) \
					  $(sbin_SCRIPTS) \
					  $(noinst_SCRIPTS) \
					  @PACKAGE_TARNAME@-*.tar.* \
					  @PACKAGE_TARNAME@-*.zip
DISTCHECK_CONFIGURE_FLAGS		= --enable-strictwarnings --with-perl=$(PERL_PATH)


# macros for lib/libbindle.la
lib_libbindle_la_DEPENDENCIES		= Makefile lib/libbindle/libbindle.sym
lib_libbindle_la_LDFLAGS		= -rpath'$(libdir)'\
					  -version-info $(LIB_VERSION_INFO) \
					  $(AM_LDFLAGS) \
					  -export-symbols $(srcdir)/lib/libbindle/libbindle.sym
lib_libbindle_la_SOURCES		= \
					  lib/libbindle/errno.c \
					  lib/libbindle/fd.c \
					  lib/libbindle/utf8.c \
					  lib/libbindle/version.c


# macros for tests/utf8test
tests_utf8_utf8test_DEPENDENCIES	= lib/libbindle.la
tests_utf8_utf8test_LDADD		= -lbindle


# Makefile includes
GIT_PACKAGE_VERSION_DIR=include
include $(srcdir)/@bindletools_srcdir@/build-aux/makefile-autotools.am
include $(srcdir)/@bindletools_srcdir@/build-aux/makefile-subst.am
include $(srcdir)/@bindletools_srcdir@/build-aux/makefile-version.am


# substitution routine
make_utf8check = \
		echo "cp tests/utf8/utf8test.sh $(@)"; \
		mkdir -p `dirname ${@}` || exit 1; \
		rm -f $(@) || exit 1; \
		cp $(top_srcdir)/tests/utf8/utf8test.sh $(@) || exit 1; \
		chmod 0755 ${@}


# custom targets
.PHONY: install-perl

tests/utf8/ascii.sh: Makefile tests/utf8/utf8test.sh
	@$(make_utf8check)


tests/utf8/invalid-continuation.sh: Makefile tests/utf8/utf8test.sh
	@$(make_utf8check)


tests/utf8/invalid-length.sh: Makefile tests/utf8/utf8test.sh
	@$(make_utf8check)


tests/utf8/mixed.sh: Makefile tests/utf8/utf8test.sh
	@$(make_utf8check)


tests/utf8/null.sh: Makefile tests/utf8/utf8test.sh
	@$(make_utf8check)


tests/utf8/utf8.sh: Makefile tests/utf8/utf8test.sh
	@$(make_utf8check)


lib/perl-bindletools/lib/Bindle/Version.pm: Makefile lib/perl-bindletools/lib/Bindle/Version.pm.in
	chmod -R u+w $(srcdir)/lib/perl-bindletools
	@$(do_subst_dt)
	@mv lib/perl-bindletools/lib/Bindle/Version.pm $(srcdir)/lib/perl-bindletools/lib/Bindle/Version.pm

lib/perl-bindletools/Makefile: $(PERL_FILES) lib/perl-bindletools/lib/Bindle/Version.pm
	chmod -R u+w $(srcdir)/lib/perl-bindletools
	cd $(srcdir)/lib/perl-bindletools && $(PERL) Makefile.PL
	touch $(srcdir)/lib/perl-bindletools


lib/perl-bindletools/built-by-make: lib/perl-bindletools/Makefile
	rm -f $(@)
	chmod -R u+w $(srcdir)/lib/perl-bindletools
	cd $(srcdir)/lib/perl-bindletools && $(MAKE) test
	mkdir -p `dirname $(@)`
	touch $(@)


perl-install: lib/perl-bindletools/built-by-make
	cd $(srcdir)/lib/perl-bindletools && make install


# local targets
install-exec-local:

install-data-local:
	@echo "*******************************************************"
	@echo "*                                                     *"
	@echo "*  To install perl module, type: 'make perl-install'  *"
	@echo "*                                                     *"
	@echo "*******************************************************"

uninstall-local:

clean-local:
	test -f $(srcdir)/lib/perl-bindletools/Makefile \
	   && { cd $(srcdir)/lib/perl-bindletools && $(MAKE) distclean; } \
	   || { cd $(srcdir); }

distclean-local:
	rm -fR $(srcdir)/autom4te.cache

# end of automake file
