#
#   Bindle Binaries Tools
#   Copyright (C) 2014 Bindle Binaries <syzdek@bindlebinaries.com>.
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
#   m4/bindle-fdpoller.m4 - m4 macros used by configure.ac
#


# AC_BINDLE_FD_POLLER()
# ______________________________________________________________________________
AC_DEFUN([AC_BINDLE_FD_POLLER],[dnl

   # prerequists
   AC_REQUIRE([AC_PROG_CC])

   # default values
   BINDLE_FD_POLLER="x"
   BINDLE_FD_POLLER_LIST=epoll,kqueue64,kqueue,poll

   # validates input argument
   if test "x$1" != "x" && test "x$1" != "xdummy" && test "x$1" != "xall";then
      BINDLE_FD_POLLER_LIST=$1
   fi

   # display options
   withval="x"
   AC_ARG_WITH(
      fd-poller,
      [AS_HELP_STRING([--with-fd-poller=method], [file descriptor polling method [default: auto]])],
      [ WFDPOLLER=$withval ],
      [ WFDPOLLER=$withval ]
   )

   # cleans up options
   if test "x${WFDPOLLER}" = "xyes";then
      WFDPOLLER="x"
   elif test "x${WFDPOLLER}" = "xno";then
      WFDPOLLER="x"
   elif test "x${WFDPOLLER}" = "x";then
      WFDPOLLER="x"
   fi

   for POLLER in `echo ${BINDLE_FD_POLLER_LIST} |sed -e 's/,/ /g'`;do
      case ${POLLER} in

         # checks for epoll
         'epoll')
         if test "${WFDPOLLER}" = "x" || test "${WFDPOLLER}" = "epoll";then
            if test "${BINDLE_FD_POLLER}" = "x";then
               AC_BINDLE_EPOLL
               if test ${HAVE_EPOLL} = yes;then
                  BINDLE_FD_POLLER="epoll"
                  AC_DEFINE_UNQUOTED([USE_EPOLL], 1, [Use epoll()])
               fi
            fi
         fi
         ;;

         # checks for kqueue & kevent64
         'kqueue64')
         if test "${WFDPOLLER}" = "x" || test "${WFDPOLLER}" = "kevent64";then
            if test "${BINDLE_FD_POLLER}" = "x";then
               AC_BINDLE_KQUEUE64
               if test ${HAVE_KQUEUE64} = yes;then
                  BINDLE_FD_POLLER="kqueue64"
                  AC_DEFINE_UNQUOTED([USE_KQUEUE64], 1, [Use kqueue() & kevent64()])
               fi
            fi
         fi
         ;;

         # checks for kqueue & kevent
         'kqueue')
         if test "${WFDPOLLER}" = "x" || test "${WFDPOLLER}" = "kevent";then
            if test "${BINDLE_FD_POLLER}" = "x";then
               AC_BINDLE_KQUEUE
               if test ${HAVE_KQUEUE} = yes;then
                  BINDLE_FD_POLLER="kqueue"
                  AC_DEFINE_UNQUOTED([USE_KQUEUE], 1, [Use kqueue() & kevent()])
               fi
            fi
         fi
         ;;

         # checks for poll
         'poll')
         if test "${WFDPOLLER}" = "x" || test "${WFDPOLLER}" = "poll";then
            if test "${BINDLE_FD_POLLER}" = "x";then
               AC_BINDLE_POLL
               if test ${HAVE_POLL} = yes;then
                  BINDLE_FD_POLLER="poll"
                  AC_DEFINE_UNQUOTED([USE_POLL], 1, [Use poll()])
               fi
            fi
         fi
         ;;

         *)
            AC_MSG_WARN([unknown FD poller "${POLLER}"])
         ;;
      esac
   done

   # checks for error
   if test "${BINDLE_FD_POLLER}" = "x";then
      AC_MSG_ERROR([unable to determine system socket poller interface.])
   fi

   AM_CONDITIONAL([USE_EPOLL],      [test "${BINDLE_FD_POLLER}" == "epoll"])
   AM_CONDITIONAL([USE_KQUEUE],     [test "${BINDLE_FD_POLLER}" == "kqueue"])
   AM_CONDITIONAL([USE_KQUEUE64],   [test "${BINDLE_FD_POLLER}" == "kqueue64"])
   AM_CONDITIONAL([USE_POLL],       [test "${BINDLE_FD_POLLER}" == "poll"])
])dnl


# AC_BINDLE_EPOLL()
# -----------------------------------
AC_DEFUN([AC_BINDLE_EPOLL],[dnl

   # checks for epoll
   HAVE_EPOLL=yes
   AC_CHECK_HEADERS([sys/epoll.h],              [],   [HAVE_EPOLL=no])
   AC_CHECK_LIB([c], [epoll_create],            [],   [HAVE_EPOLL=no])
   AC_CHECK_LIB([c], [epoll_ctl],               [],   [HAVE_EPOLL=no])
   AC_CHECK_LIB([c], [epoll_wait],              [],   [HAVE_EPOLL=no])
   AC_CHECK_MEMBER([union epoll_data.ptr],      [],   [HAVE_EPOLL=no],   [[#include <sys/epoll.h>]])
   AC_CHECK_MEMBER([union epoll_data.gf],       [],   [HAVE_EPOLL=no],   [[#include <sys/epoll.h>]])
   AC_CHECK_MEMBER([union epoll_data.u32],      [],   [HAVE_EPOLL=no],   [[#include <sys/epoll.h>]])
   AC_CHECK_MEMBER([union epoll_data.u64],      [],   [HAVE_EPOLL=no],   [[#include <sys/epoll.h>]])
   AC_CHECK_MEMBER([struct epoll_event.events], [],   [HAVE_EPOLL=no],   [[#include <sys/epoll.h>]])
   AC_CHECK_MEMBER([struct epoll_event.data],   [],   [HAVE_EPOLL=no],   [[#include <sys/epoll.h>]])

   if test ${HAVE_EPOLL} = yes;then
      AC_DEFINE_UNQUOTED([HAVE_EPOLL], 1, [Have epoll()])
   fi
])dnl


# AC_BINDLE_KQUEUE()
# -----------------------------------
AC_DEFUN([AC_BINDLE_KQUEUE],[dnl

   # checks for kqueue
   HAVE_KQUEUE=yes
   AC_CHECK_HEADERS([sys/types.h],              [],   [HAVE_KQUEUE=no])
   AC_CHECK_HEADERS([sys/event.h],              [],   [HAVE_KQUEUE=no])
   AC_CHECK_HEADERS([sys/time.h],               [],   [HAVE_KQUEUE=no])
   AC_CHECK_LIB([c], [kqueue],                  [],   [HAVE_KQUEUE=no])
   AC_CHECK_LIB([c], [kevent],                  [],   [HAVE_KQUEUE=no])
   AC_CHECK_MEMBER([struct kevent.ident],       [],   [HAVE_KQUEUE=no],   [[#include <sys/event.h>]])
   AC_CHECK_MEMBER([struct kevent.filter],      [],   [HAVE_KQUEUE=no],   [[#include <sys/event.h>]])
   AC_CHECK_MEMBER([struct kevent.flags],       [],   [HAVE_KQUEUE=no],   [[#include <sys/event.h>]])
   AC_CHECK_MEMBER([struct kevent.fflags],      [],   [HAVE_KQUEUE=no],   [[#include <sys/event.h>]])
   AC_CHECK_MEMBER([struct kevent.data],        [],   [HAVE_KQUEUE=no],   [[#include <sys/event.h>]])
   AC_CHECK_MEMBER([struct kevent.udata],       [],   [HAVE_KQUEUE=no],   [[#include <sys/event.h>]])

   if test ${HAVE_KQUEUE} = yes;then
      AC_DEFINE_UNQUOTED([HAVE_KQUEUE], 1, [Have kqueue()])
   fi
])dnl


# AC_BINDLE_KQUEUE64()
# -----------------------------------
AC_DEFUN([AC_BINDLE_KQUEUE64],[dnl

   # checks for kqueue64
   HAVE_KQUEUE64=yes
   AC_CHECK_HEADERS([sys/types.h],              [],   [HAVE_KQUEUE64=no])
   AC_CHECK_HEADERS([sys/event.h],              [],   [HAVE_KQUEUE64=no])
   AC_CHECK_HEADERS([sys/time.h],               [],   [HAVE_KQUEUE64=no])
   AC_CHECK_LIB([c], [kqueue],                  [],   [HAVE_KQUEUE64=no])
   AC_CHECK_LIB([c], [kevent64],                [],   [HAVE_KQUEUE64=no])
   AC_CHECK_MEMBER([struct kevent64_s.ident],   [],   [HAVE_KQUEUE=no],   [[#include <sys/event.h>]])
   AC_CHECK_MEMBER([struct kevent64_s.filter],  [],   [HAVE_KQUEUE=no],   [[#include <sys/event.h>]])
   AC_CHECK_MEMBER([struct kevent64_s.flags],   [],   [HAVE_KQUEUE=no],   [[#include <sys/event.h>]])
   AC_CHECK_MEMBER([struct kevent64_s.fflags],  [],   [HAVE_KQUEUE=no],   [[#include <sys/event.h>]])
   AC_CHECK_MEMBER([struct kevent64_s.data],    [],   [HAVE_KQUEUE=no],   [[#include <sys/event.h>]])
   AC_CHECK_MEMBER([struct kevent64_s.udata],   [],   [HAVE_KQUEUE=no],   [[#include <sys/event.h>]])
   AC_CHECK_MEMBER([struct kevent64_s.ext],     [],   [HAVE_KQUEUE=no],   [[#include <sys/event.h>]])

   if test ${HAVE_KQUEUE64} = yes;then
      AC_DEFINE_UNQUOTED([HAVE_KQUEUE64], 1, [Have kqueue()])
   fi
])dnl


# AC_BINDLE_POLL()
# -----------------------------------
AC_DEFUN([AC_BINDLE_POLL],[dnl
   # checks for poll
   HAVE_POLL=yes
   AC_CHECK_HEADERS([poll.h],                   [],   [HAVE_POLL=no])
   AC_CHECK_LIB([c], [poll],                    [],   [HAVE_POLL=no])
   AC_CHECK_MEMBER([struct pollfd.fd],          [],   [HAVE_POLL=no],   [[#include <poll.h>]])
   AC_CHECK_MEMBER([struct pollfd.events],      [],   [HAVE_POLL=no],   [[#include <poll.h>]])
   AC_CHECK_MEMBER([struct pollfd.revents],     [],   [HAVE_POLL=no],   [[#include <poll.h>]])

   if test ${HAVE_POLL} = yes;then
      AC_DEFINE_UNQUOTED([HAVE_POLL], 1, [Have poll()])
   fi
])dnl


# end of m4 file
