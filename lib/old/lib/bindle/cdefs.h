/*
 *  Xen Herder
 *  Copyright (C) 2014 Bindle Binaries <syzdek@bindlebinaries.com>.
 *
 *  @BINDLE_BINARIES_BSD_LICENSE_START@
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
 *     * Neither the name of Bindle Binaries nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 *  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL BINDLE BINARIES BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 *  SUCH DAMAGE.
 *
 *  @BINDLE_BINARIES_BSD_LICENSE_END@
 */
/**
 *   @file bindle/cdefs.h
 *   Bindletools C portability declarations
 */
#ifndef __BINDLE_CDEFS_H
#define __BINDLE_CDEFS_H 1
#undef __BINDLE_PMARK


///////////////
//           //
//  Headers  //
//           //
///////////////
#ifdef __BINDLE_PMARK
#pragma mark - Headers
#endif


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#ifdef __BINDLE_PMARK
#pragma mark - Definitions
#endif

// Exports function type
#undef BINDLE_C_DECLS
#if defined(__cplusplus) || defined(c_plusplus)
#   define _BINDLE_I             extern "C" inline
#   define BINDLE_C_DECLS        "C"             ///< exports as C functions
#   define BINDLE_BEGIN_C_DECLS  extern "C" {    ///< exports as C functions
#   define BINDLE_END_C_DECLS    }               ///< exports as C functions
#else
#   define _BINDLE_I             inline
#   define BINDLE_C_DECLS        /* empty */     ///< exports as C functions
#   define BINDLE_BEGIN_C_DECLS  /* empty */     ///< exports as C functions
#   define BINDLE_END_C_DECLS    /* empty */     ///< exports as C functions
#endif
#ifdef WIN32
#   ifdef _LIB_LIBBINDLE_H
#      define _BINDLE_F   extern BINDLE_C_DECLS __declspec(dllexport)   ///< used for library calls
#      define _BINDLE_V   extern BINDLE_C_DECLS __declspec(dllexport)   ///< used for library calls
#   else
#      define _BINDLE_F   extern BINDLE_C_DECLS __declspec(dllimport)   ///< used for library calls
#      define _BINDLE_V   extern BINDLE_C_DECLS __declspec(dllimport)   ///< used for library calls
#   endif
#else
#   ifdef _LIB_LIBBINDLE_H
#      define _BINDLE_F   /* empty */                                   ///< used for library calls
#      define _BINDLE_V   extern BINDLE_C_DECLS                         ///< used for library calls
#   else
#      define _BINDLE_F   extern BINDLE_C_DECLS                         ///< used for library calls
#      define _BINDLE_V   extern BINDLE_C_DECLS                         ///< used for library calls
#   endif
#endif


#endif /* end of header */
