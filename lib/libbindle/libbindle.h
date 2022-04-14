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
#ifndef _LIB_LIBBINDLE_H
#define _LIB_LIBBINDLE_H 1


///////////////
//           //
//  Headers  //
//           //
///////////////
#pragma mark - Headers

//#include <bindle_compat.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdarg.h>

#include <bindle.h>


//////////////
//          //
//  Macros  //
//          //
//////////////
#pragma mark - Macros

#undef Debug
#ifdef USE_DEBUG
#   define BindleDebug( level, fmt, ... ) bindle_debug( level, fmt, __VA_ARGS__ )
#   define BindleDebugTrace() bindle_debug( BNDL_DEBUG_TRACE, "%s()", __func__ )
#else
#   define BindleDebug( level, fmt, ... ) ((void)0)
#   define BindleDebugTrace() ((void)0)
#endif


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#pragma mark - Definitions

#define BNDL_SUCCESS                      0


// library defaults
#define BNDL_DFLT_DEBUG_IDENT             "libbindle"
#define BNDL_DFLT_DEBUG_LEVEL             BNDL_DEBUG_NONE
#define BNDL_DFLT_DEBUG_SYSLOG            BNDL_NO


//////////////////
//              //
//  Data Types  //
//              //
//////////////////
#pragma mark - Data Types


/////////////////
//             //
//  Variables  //
//             //
/////////////////
#pragma mark - Variables


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#pragma mark - Prototypes


#endif /* end of header */
