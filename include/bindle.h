/*
 *   Bindle Binaries Tools
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
#ifndef __BINDLE_H
#define __BINDLE_H 1


///////////////
//           //
//  Headers  //
//           //
///////////////
#pragma mark - Headers

#include <stddef.h>
#include <inttypes.h>
#include <sys/types.h>


//////////////
//          //
//  Macros  //
//          //
//////////////
#pragma mark - Macros

// Exports function type
#undef BINDLE_C_DECLS
#undef BINDLE_BEGIN_C_DECLS
#undef BINDLE_END_C_DECLS
#undef _BINDLE_I
#undef _BINDLE_F
#undef _BINDLE_V
#if defined(__cplusplus) || defined(c_plusplus)
#   define BINDLE_C_DECLS        "C"             ///< exports as C functions
#   define BINDLE_BEGIN_C_DECLS  extern "C" {    ///< exports as C functions
#   define BINDLE_END_C_DECLS    }               ///< exports as C functions
#else
#   define BINDLE_C_DECLS        /* empty */     ///< exports as C functions
#   define BINDLE_BEGIN_C_DECLS  /* empty */     ///< exports as C functions
#   define BINDLE_END_C_DECLS    /* empty */     ///< exports as C functions
#endif
#ifdef WIN32
#   ifdef _LIB_LIBBINDLE_H
#      define _BINDLE_I   inline
#      define _BINDLE_F   extern BINDLE_C_DECLS __declspec(dllexport)   ///< used for library calls
#      define _BINDLE_V   extern BINDLE_C_DECLS __declspec(dllexport)   ///< used for library calls
#   else
#      define _BINDLE_I   extern BINDLE_C_DECLS __declspec(dllimport)   ///< used for library calls
#      define _BINDLE_F   extern BINDLE_C_DECLS __declspec(dllimport)   ///< used for library calls
#      define _BINDLE_V   extern BINDLE_C_DECLS __declspec(dllimport)   ///< used for library calls
#   endif
#else
#   ifdef _LIB_LIBBINDLE_H
#      define _BINDLE_I   inline
#      define _BINDLE_F   /* empty */                                    ///< used for library calls
#      define _BINDLE_V   extern BINDLE_C_DECLS                         ///< used for library calls
#   else
#      define _BINDLE_I   extern BINDLE_C_DECLS                         ///< used for library calls
#      define _BINDLE_F   extern BINDLE_C_DECLS                         ///< used for library calls
#      define _BINDLE_V   extern BINDLE_C_DECLS                         ///< used for library calls
#   endif
#endif


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#pragma mark - Definitions

#define BNDL_OFF                    0
#define BNDL_ON                     1
#define BNDL_NO                     0
#define BNDL_YES                    1


// library debug levels
#define BNDL_DEBUG_NONE             0
#define BNDL_DEBUG_TRACE            0x0000001
#define BNDL_DEBUG_ARGS             0x0000002
#define BNDL_DEBUG_CONNS            0x0000004
#define BNDL_DEBUG_PACKETS          0x0000008
#define BNDL_DEBUG_PARSE            0x0000010
#define BNDL_DEBUG_ANY              (~0x00)


//////////////////
//              //
//  Data Types  //
//              //
//////////////////
#pragma mark - Data Types

typedef struct _bindle_obj
{
   uint8_t                 magic[8];
   atomic_intptr_t         ref_count;
   void (*free_func)(void * ptr);
} bindle_obj_t;


/////////////////
//             //
//  Variables  //
//             //
/////////////////
#pragma mark - Variables

extern const char *   bindle_debug_ident;
extern int            bindle_debug_level;
extern int            bindle_debug_syslog;


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#pragma mark - Prototypes

//------------------//
// debug prototypes //
//------------------//
#pragma mark debug prototypes

_BINDLE_F void
bindle_debug(
         int                           level,
         const char *                  fmt,
         ... );


//-------------------//
// object prototypes //
//-------------------//
#pragma mark object prototypes

_BINDLE_F void
bindle_free(
         void *                        ptr );


_BINDLE_F void *
bindle_obj_alloc(
         size_t                        size,
         void (*free_func)(void * ptr) );


_BINDLE_F void
bindle_obj_release(
         bindle_obj_t *                obj );


_BINDLE_F void *
bindle_obj_retain(
         bindle_obj_t *                obj );


_BINDLE_F intptr_t
bindle_obj_retain_count(
         bindle_obj_t *                obj );


//-------------------//
// string prototypes //
//-------------------//
#pragma mark string prototypes

_BINDLE_F char *
bindle_strdup(
         const char *                  s1 );


_BINDLE_F char *
bindle_strexpand(
         char *                        dst,
         const char * restrict         src,
         size_t                        len,
         int                           force_expansion );


_BINDLE_F size_t
bindle_strlcpy(
         char * restrict               dst,
         const char * restrict         src,
         size_t                        dstsize );


_BINDLE_F size_t
bindle_strlcat(
         char * restrict               dst,
         const char * restrict         src,
         size_t                        dstsize );


_BINDLE_F char *
bindle_strndup(
         const char *                  s1,
         size_t                        n );


_BINDLE_F int
bindle_strtoargs(
         char *                        str,
         char ***                      argvp,
         int *                         argcp );


_BINDLE_F int
bindle_strtobool(
         const char *                  str );


_BINDLE_F char *
bindle_strtrim(
         char *                        str );


//-------------------------//
// string array prototypes //
//-------------------------//
#pragma mark string array prototypes

_BINDLE_F int
bindle_strsadd(
         char ***                      strsp,
         const char *                  str );


_BINDLE_F size_t
bindle_strscount(
         char * const *                strs );


_BINDLE_F int
bindle_strsdup(
         char ***                      dstp,
         char * const *                src );


_BINDLE_F void
bindle_strsfree(
         char **                       strs );


#endif /* end of header */
