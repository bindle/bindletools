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
					  doc/mans/bindletools-base16.1 \
					  doc/mans/bindletools-base32.1 \
					  doc/mans/bindletools-base32hex.1 \
					  doc/mans/bindletools-base64.1 \
					  doc/mans/bindletools-crockfordbase32.1 \
					  doc/mans/bindletools-pctenc.1 \
					  doc/mans/bindletools-urldesc.1 \
					  doc/mans/bindle_array.3 \
					  doc/mans/bindle_basename_r.3 \
					  doc/mans/bindle_bindex.3 \
					  doc/mans/bindle_decode.3 \
					  doc/mans/bindle_dirname_r.3 \
					  doc/mans/bindle_enqueue.3 \
					  doc/mans/bindle_filetostr.3 \
					  doc/mans/bindle_getpass.3 \
					  doc/mans/bindle_peek.3 \
					  doc/mans/bindle_push.3 \
					  doc/mans/bindle_refcount.3 \
					  doc/mans/bindle_readline.3 \
					  doc/mans/bindle_strchomp.3 \
					  doc/mans/bindle_strdup.3 \
					  doc/mans/bindle_strexpand.3 \
					  doc/mans/bindle_strings.3 \
					  doc/mans/bindle_strlcpy.3 \
					  doc/mans/bindle_strsadd.3 \
					  doc/mans/bindle_strsplit.3 \
					  doc/mans/bindle_strtoargs.3 \
					  doc/mans/bindle_strtobool.3 \
					  doc/mans/bindle_strtrim.3 \
					  doc/mans/bindle_urldesc.3
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
					  examples/example-decode \
					  examples/example-encode \
					  examples/example-queue \
					  examples/example-refcount \
					  examples/example-stack \
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
					  doc/mans/bindletools-base16.1.in \
					  doc/mans/bindletools-base32.1.in \
					  doc/mans/bindletools-base32hex.1.in \
					  doc/mans/bindletools-base64.1.in \
					  doc/mans/bindletools-crockfordbase32.1.in \
					  doc/mans/bindletools-pctenc.1.in \
					  doc/mans/bindletools-urldesc.1.in \
					  doc/mans/bindle_array.3.in \
					  doc/mans/bindle_basename_r.3.in \
					  doc/mans/bindle_bindex.3.in \
					  doc/mans/bindle_decode.3.in \
					  doc/mans/bindle_dirname_r.3.in \
					  doc/mans/bindle_enqueue.3.in \
					  doc/mans/bindle_filetostr.3.in \
					  doc/mans/bindle_getpass.3.in \
					  doc/mans/bindle_peek.3.in \
					  doc/mans/bindle_push.3.in \
					  doc/mans/bindle_readline.3.in \
					  doc/mans/bindle_refcount.3.in \
					  doc/mans/bindle_strchomp.3.in \
					  doc/mans/bindle_strdup.3.in \
					  doc/mans/bindle_strexpand.3.in \
					  doc/mans/bindle_strings.3.in \
					  doc/mans/bindle_strlcpy.3.in \
					  doc/mans/bindle_strsadd.3.in \
					  doc/mans/bindle_strsplit.3.in \
					  doc/mans/bindle_strtoargs.3.in \
					  doc/mans/bindle_strtobool.3.in \
					  doc/mans/bindle_strtrim.3.in \
					  doc/mans/bindle_urldesc.3.in \
					  examples/Makefile \
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
   noinst_PROGRAMS			+= examples/example-decode
   noinst_PROGRAMS			+= examples/example-encode
   noinst_PROGRAMS			+= examples/example-queue
   noinst_PROGRAMS			+= examples/example-refcount
   noinst_PROGRAMS			+= examples/example-stack
   examples_DATA			+= examples/Makefile
   examples_DATA			+= examples/example-bsearch.c
   examples_DATA			+= examples/example-decode.c
   examples_DATA			+= examples/example-encode.c
   examples_DATA			+= examples/example-queue.c
   examples_DATA			+= examples/example-refcount.c
   examples_DATA			+= examples/example-stack.c
endif


# macros for examples/example-bsearch
examples_example_bsearch_DEPENDENCIES	= lib/libbindle.la
examples_example_bsearch_LDADD		= lib/libbindle.la
examples_example_bsearch_SOURCES	= examples/example-bsearch.c


# macros for examples/example-decode
examples_example_decode_DEPENDENCIES	= lib/libbindle.la
examples_example_decode_LDADD		= lib/libbindle.la
examples_example_decode_SOURCES		= examples/example-decode.c


# macros for examples/example-encode
examples_example_encode_DEPENDENCIES	= lib/libbindle.la
examples_example_encode_LDADD		= lib/libbindle.la
examples_example_encode_SOURCES		= examples/example-encode.c


# macros for examples/example-queue
examples_example_queue_DEPENDENCIES	= lib/libbindle.la
examples_example_queue_LDADD		= lib/libbindle.la
examples_example_queue_SOURCES		= examples/example-queue.c


# macros for examples/example-refcount
examples_example_refcount_DEPENDENCIES	= lib/libbindle.la
examples_example_refcount_LDADD		= lib/libbindle.la
examples_example_refcount_SOURCES	= examples/example-refcount.c


# macros for examples/example-stack
examples_example_stack_DEPENDENCIES	= lib/libbindle.la
examples_example_stack_LDADD		= lib/libbindle.la
examples_example_stack_SOURCES		= examples/example-stack.c


# macros for src/bindletools
src_bindletools_DEPENDENCIES		= lib/libbindle.la
src_bindletools_LDADD			= lib/libbindle.la
src_bindletools_SOURCES			= src/bindletools.c \
					  src/bindletools.h \
					  src/widget-encodings.c \
					  src/widget-expand.c \
					  src/widget-urldesc.c


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

doc/mans/bindletools-base16.1: Makefile $(srcdir)/doc/mans/bindletools-base16.1.in
	@$(do_subst_dt)

doc/mans/bindletools-base32.1: Makefile $(srcdir)/doc/mans/bindletools-base32.1.in
	@$(do_subst_dt)

doc/mans/bindletools-base32hex.1: Makefile $(srcdir)/doc/mans/bindletools-base32hex.1.in
	@$(do_subst_dt)

doc/mans/bindletools-base64.1: Makefile $(srcdir)/doc/mans/bindletools-base64.1.in
	@$(do_subst_dt)

doc/mans/bindletools-crockfordbase32.1: Makefile $(srcdir)/doc/mans/bindletools-crockfordbase32.1.in
	@$(do_subst_dt)

doc/mans/bindletools-pctenc.1: Makefile $(srcdir)/doc/mans/bindletools-pctenc.1.in
	@$(do_subst_dt)

doc/mans/bindletools-urldesc.1: Makefile $(srcdir)/doc/mans/bindletools-urldesc.1.in
	@$(do_subst_dt)

doc/mans/bindle_array.3: Makefile $(srcdir)/doc/mans/bindle_array.3.in
	@$(do_subst_dt)

doc/mans/bindle_basename_r.3: Makefile $(srcdir)/doc/mans/bindle_basename_r.3.in
	@$(do_subst_dt)

doc/mans/bindle_bindex.3: Makefile $(srcdir)/doc/mans/bindle_bindex.3.in
	@$(do_subst_dt)

doc/mans/bindle_decode.3: Makefile $(srcdir)/doc/mans/bindle_decode.3.in
	@$(do_subst_dt)

doc/mans/bindle_dirname_r.3: Makefile $(srcdir)/doc/mans/bindle_dirname_r.3.in
	@$(do_subst_dt)

doc/mans/bindle_enqueue.3: Makefile $(srcdir)/doc/mans/bindle_enqueue.3.in
	@$(do_subst_dt)

doc/mans/bindle_filetostr.3: Makefile $(srcdir)/doc/mans/bindle_filetostr.3.in
	@$(do_subst_dt)

doc/mans/bindle_getpass.3: Makefile $(srcdir)/doc/mans/bindle_getpass.3.in
	@$(do_subst_dt)

doc/mans/bindle_peek.3: Makefile $(srcdir)/doc/mans/bindle_peek.3.in
	@$(do_subst_dt)

doc/mans/bindle_push.3: Makefile $(srcdir)/doc/mans/bindle_push.3.in
	@$(do_subst_dt)

doc/mans/bindle_readline.3: Makefile $(srcdir)/doc/mans/bindle_readline.3.in
	@$(do_subst_dt)

doc/mans/bindle_refcount.3: Makefile $(srcdir)/doc/mans/bindle_refcount.3.in
	@$(do_subst_dt)

doc/mans/bindle_strchomp.3: Makefile $(srcdir)/doc/mans/bindle_strchomp.3.in
	@$(do_subst_dt)

doc/mans/bindle_strdup.3: Makefile $(srcdir)/doc/mans/bindle_strdup.3.in
	@$(do_subst_dt)

doc/mans/bindle_strexpand.3: Makefile $(srcdir)/doc/mans/bindle_strexpand.3.in
	@$(do_subst_dt)

doc/mans/bindle_strings.3: Makefile $(srcdir)/doc/mans/bindle_strings.3.in
	@$(do_subst_dt)

doc/mans/bindle_strlcpy.3: Makefile $(srcdir)/doc/mans/bindle_strlcpy.3.in
	@$(do_subst_dt)

doc/mans/bindle_strsadd.3: Makefile $(srcdir)/doc/mans/bindle_strsadd.3.in
	@$(do_subst_dt)

doc/mans/bindle_strsplit.3: Makefile $(srcdir)/doc/mans/bindle_strsplit.3.in
	@$(do_subst_dt)

doc/mans/bindle_strtobool.3: Makefile $(srcdir)/doc/mans/bindle_strtobool.3.in
	@$(do_subst_dt)

doc/mans/bindle_strtoargs.3: Makefile $(srcdir)/doc/mans/bindle_strtoargs.3.in
	@$(do_subst_dt)

doc/mans/bindle_strtrim.3: Makefile $(srcdir)/doc/mans/bindle_strtrim.3.in
	@$(do_subst_dt)

doc/mans/bindle_urldesc.3: Makefile $(srcdir)/doc/mans/bindle_urldesc.3.in
	@$(do_subst_dt)


# local targets
install-exec-local:

install-data-local: install-man
	if test -f $(DESTDIR)/$(mandir)/man3/bindle_basename_r.3;then \
	   cd $(DESTDIR)/$(mandir)/man3 && \
	   ln -fs bindle_basename_r.3 bindle_basename.3; \
	fi;
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
	if test -f $(DESTDIR)/$(mandir)/man3/bindle_dirname_r.3;then \
	   cd $(DESTDIR)/$(mandir)/man3 && \
	   ln -fs bindle_dirname_r.3 bindle_dirname.3; \
	fi;
	if test -f $(DESTDIR)/$(mandir)/man3/bindle_enqueue.3;then\
	   cd $(DESTDIR)/$(mandir)/man3 && \
	   ln -fs bindle_enqueue.3 bindle_dequeue.3; \
	fi;
	if test -f $(DESTDIR)/$(mandir)/man3/bindle_getpass.3;then\
	   cd $(DESTDIR)/$(mandir)/man3 && \
	   ln -fs bindle_getpass.3 bindle_getpass_r.3; \
	fi;
	if test -f $(DESTDIR)/$(mandir)/man3/bindle_push.3;then\
	   cd $(DESTDIR)/$(mandir)/man3 && \
	   ln -fs bindle_push.3 bindle_pop.3; \
	fi;
	if test -f $(DESTDIR)/$(mandir)/man3/bindle_refcount.3;then\
	   cd $(DESTDIR)/$(mandir)/man3 && \
	   ln -fs bindle_refcount.3 bindle_alloc.3 && \
	   ln -fs bindle_refcount.3 bindle_free.3 && \
	   ln -fs bindle_refcount.3 bindle_realloc.3 && \
	   ln -fs bindle_refcount.3 bindle_release.3 && \
	   ln -fs bindle_refcount.3 bindle_retain.3 && \
	   ln -fs bindle_refcount.3 bindle_retain_count.3; \
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
	if test -f $(DESTDIR)/$(mandir)/man3/bindle_strsadd.3;then\
	   cd $(DESTDIR)/$(mandir)/man3 && \
	   ln -fs bindle_strsadd.3 bindle_strscount.3 && \
	   ln -fs bindle_strsadd.3 bindle_strsdup.3 && \
	   ln -fs bindle_strsadd.3 bindle_strsfree.3; \
	fi;
	if test -f $(DESTDIR)/$(mandir)/man3/bindle_urldesc.3;then\
	   cd $(DESTDIR)/$(mandir)/man3 && \
	   ln -fs bindle_urldesc.3 bindle_is_url.3 && \
	   ln -fs bindle_urldesc.3 bindle_urldesc2str.3 && \
	   ln -fs bindle_urldesc.3 bindle_urldesc_free.3 && \
	   ln -fs bindle_urldesc.3 bindle_urldesc_parse.3 && \
	   ln -fs bindle_urldesc.3 bindle_urldesc_resolve.3; \
	fi;


uninstall-local:

clean-local:

distclean-local:
	rm -fR $(srcdir)/autom4te.cache

# end of automake file
