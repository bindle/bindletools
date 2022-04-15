/*
 *  Bindle Binaries Tools
 *  Copyright (C) 2022 David M. Syzdek <david@syzdek.net>.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of David M. Syzdek nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 *  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL DAVID M. SYZDEK BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 *  SUCH DAMAGE.
 */
#ifndef _TESTS_BINDLE_TESTS_H
#define _TESTS_BINDLE_TESTS_H 1


///////////////
//           //
//  Headers  //
//           //
///////////////
#pragma mark - Headers

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_BINDLE_PREFIX_H
#   include <bindle_prefix.h>
#endif
#include <bindle.h>



///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#pragma mark - Definitions

#ifndef PACKAGE_BUGREPORT
#   define PACKAGE_BUGREPORT "unknown"
#endif
#ifndef PACKAGE_COPYRIGHT
#   define PACKAGE_COPYRIGHT "unknown"
#endif
#ifndef PACKAGE_NAME
#   define PACKAGE_NAME "Bindle Binaries Tools"
#endif
#ifndef PACKAGE_VERSION
#   define PACKAGE_VERSION "unknown"
#endif


#define BNDLTEST_OPT_VERBOSE        0x00010000UL
#define BNDLTEST_OPT_QUIET          0x00020000UL


/////////////////
//             //
//  Variables  //
//             //
/////////////////
#pragma mark - Variables

#pragma mark program_name
extern const char * program_name;


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#pragma mark - Prototypes

_BINDLE_F int
bindle_tests_error(
         unsigned                      opts,
         char **                       errs,
         const char *                  fmt,
         ... );


_BINDLE_F void
bindle_tests_initialize(
         const char *                  prog_name );


_BINDLE_F void
bindle_tests_print(
         unsigned                      opts,
         const char *                  fmt,
         ... );


_BINDLE_F void
bindle_tests_verbose(
         unsigned                      opts,
         const char *                  fmt,
         ... );


_BINDLE_F void
bindle_tests_version( void );


#endif /* end of header */
