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
examplesdir				= $(docdir)/examples

# automake targets
check_PROGRAMS				=
doc_DATA				= AUTHORS.md \
					  ChangeLog.md \
					  COPYING.md \
					  README.md \
					  TODO.md
examples_DATA				=
include_HEADERS				=
lib_LTLIBRARIES				=
lib_LIBRARIES				=
man_MANS				= doc/mans/bindletools.1 \
					  doc/mans/bindletools.3 \
					  doc/mans/bindle_array.3 \
					  doc/mans/bindle_bindex.3 \
					  doc/mans/bindle_decode.3 \
					  doc/mans/bindle_enqueue.3 \
					  doc/mans/bindle_peek.3 \
					  doc/mans/bindle_push.3 \
					  doc/mans/bindle_strchomp.3 \
					  doc/mans/bindle_strdup.3 \
					  doc/mans/bindle_strings.3 \
					  doc/mans/bindle_strlcpy.3
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
EXTRA_PROGRAMS				= examples/example-bsearch \
					  examples/example-queue \
					  src/bindletools
EXTRA					=


# lists
AM_INSTALLCHECK_STD_OPTIONS_EXEMPT	=
BUILT_SOURCES				=
TESTS					=
XFAIL_TESTS				=
EXTRA_MANS				=
EXTRA_DIST				= $(doc_DATA) \
					  autogen.sh \
					  doc/mans/bindletools.1.in \
					  doc/mans/bindletools.3.in \
					  doc/mans/bindle_array.3.in \
					  doc/mans/bindle_bindex.3.in \
					  doc/mans/bindle_decode.3.in \
					  doc/mans/bindle_enqueue.3.in \
					  doc/mans/bindle_peek.3.in \
					  doc/mans/bindle_push.3.in \
					  doc/mans/bindle_strchomp.3.in \
					  doc/mans/bindle_strdup.3.in \
					  doc/mans/bindle_strings.3.in \
					  doc/mans/bindle_strlcpy.3.in \
					  tests/bindletools/README.md \
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
					  include/bindle_prefix.h \
					  @PACKAGE_TARNAME@-*.tar.* \
					  @PACKAGE_TARNAME@-*.zip
DISTCHECK_CONFIGURE_FLAGS		= --enable-strictwarnings \
					  --enable-libbindle \
					  --enable-examples \
					  --enable-documentation


# conditional build
if ENABLE_UTILITY
   bin_PROGRAMS				+= src/bindletools
endif
if ENABLE_EXAMPLES
   noinst_PROGRAMS			+= examples/example-bsearch
   noinst_PROGRAMS			+= examples/example-queue
   examples_DATA			+= examples/example-bsearch.c
   examples_DATA			+= examples/example-queue.c
endif


# macros for examples/example-bsearch
examples_example_bsearch_DEPENDENCIES	= lib/libbindle.la
examples_example_bsearch_LDADD		= lib/libbindle.la
examples_example_bsearch_SOURCES	= examples/example-bsearch.c


# macros for examples/example-queue
examples_example_queue_DEPENDENCIES	= lib/libbindle.la
examples_example_queue_LDADD		= lib/libbindle.la
examples_example_queue_SOURCES		= examples/example-queue.c


# macros for src/bindletools
src_bindletools_DEPENDENCIES		= lib/libbindle.la
src_bindletools_LDADD			= lib/libbindle.la
src_bindletools_SOURCES			= src/bindletools.c \
					  src/bindletools.h \
					  src/widget-encodings.c \
					  src/widget-urldesc.c \
					  src/widget-urlencoding.c


# Makefile includes
GIT_PACKAGE_VERSION_DIR=include
SUBST_EXPRESSIONS =
include build-aux/makefile-subst.am
include build-aux/makefile-version.am
include build-aux/makefile-libbindle.am


# custom targets
.PHONY: dep git-clean

dep: include/bindle_prefix.h

git-clean:
	git fsck --full --unreachable
	git gc --auto --aggressive


doc/mans/bindletools.1: Makefile $(srcdir)/doc/mans/bindletools.1.in
	@$(do_subst_dt)

doc/mans/bindletools.3: Makefile $(srcdir)/doc/mans/bindletools.3.in
	@$(do_subst_dt)

doc/mans/bindle_array.3: Makefile $(srcdir)/doc/mans/bindle_array.3.in
	@$(do_subst_dt)

doc/mans/bindle_bindex.3: Makefile $(srcdir)/doc/mans/bindle_bindex.3.in
	@$(do_subst_dt)

doc/mans/bindle_decode.3: Makefile $(srcdir)/doc/mans/bindle_decode.3.in
	@$(do_subst_dt)

doc/mans/bindle_enqueue.3: Makefile $(srcdir)/doc/mans/bindle_enqueue.3.in
	@$(do_subst_dt)

doc/mans/bindle_peek.3: Makefile $(srcdir)/doc/mans/bindle_peek.3.in
	@$(do_subst_dt)

doc/mans/bindle_push.3: Makefile $(srcdir)/doc/mans/bindle_push.3.in
	@$(do_subst_dt)

doc/mans/bindle_strchomp.3: Makefile $(srcdir)/doc/mans/bindle_strchomp.3.in
	@$(do_subst_dt)

doc/mans/bindle_strdup.3: Makefile $(srcdir)/doc/mans/bindle_strdup.3.in
	@$(do_subst_dt)

doc/mans/bindle_strings.3: Makefile $(srcdir)/doc/mans/bindle_strings.3.in
	@$(do_subst_dt)

doc/mans/bindle_strlcpy.3: Makefile $(srcdir)/doc/mans/bindle_strlcpy.3.in
	@$(do_subst_dt)

# local targets
install-exec-local:

install-data-local: install-man
	if test -f $(DESTDIR)/$(mandir)/man3/bindle_bindex.3;then\
	   cd $(DESTDIR)/$(mandir)/man3 && \
	   ln -fs bindle_bindex.3 bindle_badd.3 && \
	   ln -fs bindle_bindex.3 bindle_bremove.3 && \
	   ln -fs bindle_bindex.3 bindle_bsearch.3; \
	fi;
	if test -f $(DESTDIR)/$(mandir)/man3/bindle_decode.3;then\
	   cd $(DESTDIR)/$(mandir)/man3 && \
	   ln -fs bindle_decode.3 bindle_decode_size.3 && \
	   ln -fs bindle_decode.3 bindle_encode.3 && \
	   ln -fs bindle_decode.3 bindle_encode_size.3 && \
	   ln -fs bindle_decode.3 bindle_encoding_verify.3; \
	fi;
	if test -f $(DESTDIR)/$(mandir)/man3/bindle_enqueue.3;then\
	   cd $(DESTDIR)/$(mandir)/man3 && \
	   ln -fs bindle_enqueue.3 bindle_dequeue.3; \
	fi;
	if test -f $(DESTDIR)/$(mandir)/man3/bindle_push.3;then\
	   cd $(DESTDIR)/$(mandir)/man3 && \
	   ln -fs bindle_push.3 bindle_pop.3; \
	fi;
	if test -f $(DESTDIR)/$(mandir)/man3/bindle_strchomp.3;then\
           cd $(DESTDIR)/$(mandir)/man3 && \
           ln -fs bindle_strchomp.3 bindle_strchop.3; \
	fi;
	if test -f $(DESTDIR)/$(mandir)/man3/bindle_strdup.3;then\
           cd $(DESTDIR)/$(mandir)/man3 && \
           ln -fs bindle_strdup.3 bindle_strndup.3; \
	fi;
	if test -f $(DESTDIR)/$(mandir)/man3/bindle_strlcpy.3;then\
           cd $(DESTDIR)/$(mandir)/man3 && \
           ln -fs bindle_strlcpy.3 bindle_strlcat.3; \
	fi;

uninstall-local:

clean-local:

distclean-local:
	rm -fR $(srcdir)/autom4te.cache

# end of automake file
