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
#ifndef _SRC_BINDLETOOLS_H
#define _SRC_BINDLETOOLS_H 1


///////////////
//           //
//  Headers  //
//           //
///////////////
#pragma mark - Headers

// defined in the Single UNIX Specification
#ifndef _XOPEN_SOURCE
#   define _XOPEN_SOURCE 600
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_BINDLE_PREFIX_H
#   include <bindle_prefix.h>
#else
#   include <bindle.h>
#endif

#include <getopt.h>


//////////////
//          //
//  Macros  //
//          //
//////////////
#pragma mark - Macros


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#pragma mark - Definitions

#undef PROGRAM_NAME
#define PROGRAM_NAME "bindletools"
#ifndef PACKAGE_BUGREPORT
#   define PACKAGE_BUGREPORT "david@syzdek.net"
#endif
#ifndef PACKAGE_COPYRIGHT
#   define PACKAGE_COPYRIGHT ""
#endif
#ifndef PACKAGE_NAME
#   define PACKAGE_NAME ""
#endif
#ifndef PACKAGE_VERSION
#   define PACKAGE_VERSION ""
#endif


#define BINDLE_OPT_QUIET            0x0001UL
#define BINDLE_OPT_VERBOSE          0x0002UL


#define BINDLE_COMMON_SHORT "D:hqVv"
#define BINDLE_COMMON_LONG \
   { "debug",            optional_argument, NULL, 'D' }, \
   { "help",             no_argument,       NULL, 'h' }, \
   { "quiet",            no_argument,       NULL, 'q' }, \
   { "silent",           no_argument,       NULL, 'q' }, \
   { "version",          no_argument,       NULL, 'V' }, \
   { "verbose",          no_argument,       NULL, 'v' }


//////////////////
//              //
//  Data Types  //
//              //
//////////////////
#pragma mark - Data Types

typedef struct bindle_config     bindle_conf_t;
typedef struct bindle_widget     bindle_widget_t;

struct bindle_config
{
   unsigned                   opts;
   int                        argc;
   char **                    argv;
   const char *               prog_name;
   const bindle_widget_t *    widget;
   uintptr_t                  widget_flags;
};


struct bindle_widget
{
   const char *               name;
   const uintptr_t            flags;
   const char *               desc;
   const char *               usage;
   const char * const *       options;
   const char * const *       aliases;
   int  (*func_exec)(bindle_conf_t * cnf);
   int  (*func_usage)(bindle_conf_t * cnf);
};


/////////////////
//             //
//  Variables  //
//             //
/////////////////
#pragma mark - Variables

#pragma mark tinyrad_cmdmap[]
extern const bindle_widget_t bindle_widget_map[];


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#pragma mark - Prototypes

//-----------------//
// core prototypes //
//-----------------//
#pragma mark core prototypes

extern int
bindle_getopt(
         bindle_conf_t *               cnf,
         int                           argc,
         char * const *                argv,
         const char *                  optstring,
         const struct option *         longopts,
         int *                         longindex );


extern int
bindle_usage(
         bindle_conf_t *               cnf );


extern int
bindle_version(
         bindle_conf_t *               cnf );


//----------------//
// urldesc widget //
//----------------//
#pragma mark urldesc widget

extern const char * const bindle_widget_urldesc_options[];

extern int
bindle_widget_urldesc(
         bindle_conf_t *               cnf );


//--------------------//
// urlencoding widget //
//--------------------//
#pragma mark urlencoding widget

extern const char * const bindle_widget_urlencoding_options[];

extern int
bindle_widget_urlencoding(
         bindle_conf_t *               cnf );


#endif /* end of header */
