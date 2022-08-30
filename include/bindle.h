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
#include <stdatomic.h>


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

#define BNDL_SUCCESS                0
#define BNDL_OFF                    0
#define BNDL_ON                     1
#define BNDL_NO                     0
#define BNDL_YES                    1


// library limits
#define BNDL_LINE_MAX_LEN           256


// library get/set options
#define BNDL_OPT_DEBUG_LEVEL        1
#define BNDL_OPT_DEBUG_IDENT        2
#define BNDL_OPT_DEBUG_SYSLOG       3
#define BNDL_OPT_RANDOM             4


// library debug levels
#define BNDL_DEBUG_NONE             0
#define BNDL_DEBUG_TRACE            0x0000001
#define BNDL_DEBUG_ARGS             0x0000002
#define BNDL_DEBUG_CONNS            0x0000004
#define BNDL_DEBUG_PACKETS          0x0000008
#define BNDL_DEBUG_PARSE            0x0000010
#define BNDL_DEBUG_ANY              (~0x00)


// array function options
#define BNDL_BINSERT             0x0001      ///< add type: insert unique object to sorted array
#define BNDL_BREPLACE            0x0002      ///< add type: replace deplucate object in sorted array, or insert if unique
#define BNDL_BMERGE              0x0004      ///< add type: add object to sorted array regardless if unique or duplicate
#define BNDL_BDUPLAST            0x0010      ///< dup handling: add/return/remove last duplicate object in series of matching objects
#define BNDL_BDUPFIRST           0x0020      ///< dup handling: add/return/remove first duplicate object in series of matching objects
#define BNDL_BDUPANY             0x0040      ///< dup handling: add/return/remove any one duplicate object in series of matching objects
#define BNDL_BDEFAULT            ( BNDL_BINSERT | BNDL_BDUPANY )                         ///< default sorted array options
#define BNDL_BMASK_ADD           ( BNDL_BINSERT | BNDL_BMERGE   | BNDL_BREPLACE )   ///< mask for add type
#define BNDL_BMASK_DUPS          ( BNDL_BDUPANY | BNDL_BDUPLAST | BNDL_BDUPFIRST )  ///< mask for duplicate handling in sorted array


// encoding methods
#define BNDL_NONE                 0
#define BNDL_BASE32               1
#define BNDL_BASE32HEX            2
#define BNDL_BASE64               3
#define BNDL_HEX                  4
#define BNDL_CROCKFORD            5


//////////////////
//              //
//  Data Types  //
//              //
//////////////////
#pragma mark - Data Types

typedef struct _tinyrad_map
{
   const char *          map_name;
   uintptr_t             map_value;
} bindle_map_t;


typedef struct _bindle_obj
{
   uint8_t                 magic[8];
   atomic_intptr_t         ref_count;
   void (*free_func)(void * ptr);
} bindle_obj_t;


// URL = scheme ":" ["//" authority] path ["?" query] ["#" fragment]
// authority = [userinfo "@"] host [":" service]
typedef struct bindle_url_desc
{
   char *                        bud_scheme;
   char *                        bud_userinfo;
   char *                        bud_host;
   char *                        bud_service;
   uintptr_t                     bud_port;
   char *                        bud_path;
   char *                        bud_query;
   char *                        bud_fragment;
   void **                       bud_sockaddrs;
} BindleURLDesc;


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

//------------------//
// array prototypes //
//------------------//
#pragma mark array prototypes

_BINDLE_F ssize_t
bindle_badd(
         void *                        obj,
         void **                       basep,
         size_t *                      nelp,
         size_t                        width,
         unsigned                      opts,
         int (*compar)(const void *, const void *),
         void (*freeobj)(void *),
         void * (*reallocbase)(void *, size_t) );


_BINDLE_F ssize_t
bindle_bindex(
         const void *                  key,
         const void *                  base,
         size_t                        nel,
         size_t                        width,
         unsigned                      opts,
         size_t *                      wouldbep,
         int (*compar)(const void *, const void *) );


_BINDLE_F void *
bindle_bremove(
         const void *                  key,
         void *                        base,
         size_t *                      nelp,
         size_t                        width,
         unsigned                      opts,
         int (*compar)(const void *, const void *) );


_BINDLE_F void *
bindle_bsearch(
         const void *                  key,
         void *                        base,
         size_t                        nel,
         size_t                        width,
         unsigned                      opts,
         int (*compar)(const void *, const void *) );


_BINDLE_F void *
bindle_dequeue(
         void *                        base,
         size_t *                      nelp,
         size_t                        width );


_BINDLE_F ssize_t
bindle_enqueue(
         void *                        obj,
         void **                       basep,
         size_t *                      nelp,
         size_t                        width,
         void * (*reallocbase)(void *, size_t) );


_BINDLE_F void *
bindle_peek(
         void *                        base,
         size_t                        nel,
         size_t                        width );


_BINDLE_F void *
bindle_pop(
         void *                        base,
         size_t *                      nelp,
         size_t                        width );


_BINDLE_F ssize_t
bindle_push(
         void *                        obj,
         void **                       basep,
         size_t *                      nelp,
         size_t                        width,
         void * (*reallocbase)(void *, size_t) );


//------------------//
// debug prototypes //
//------------------//
#pragma mark debug prototypes

_BINDLE_F void
bindle_debug(
         int                           level,
         const char *                  fmt,
         ... );


//---------------------//
// encoding prototypes //
//---------------------//
#pragma mark encoding prototypes

_BINDLE_F ssize_t
bindle_decode(
         int                           method,
         void *                        dst,
         size_t                        s,
         const char *                  src,
         size_t                        n );


_BINDLE_F ssize_t
bindle_decode_size(
         int                           method,
         size_t                        n );


_BINDLE_F ssize_t
bindle_encode(
         int                           method,
         char *                        dst,
         size_t                        s,
         const void *                  src,
         size_t                        n,
         int                           nopad );


_BINDLE_F ssize_t
bindle_encode_size(
         int                           method,
         size_t                        n );


_BINDLE_F ssize_t
bindle_encoding_verify(
         int                           method,
         const char *                  src,
         size_t                        n );


//-----------------//
// file prototypes //
//-----------------//
#pragma mark file prototypes

_BINDLE_F int
bindle_filetostr(
         char *                        dst,
         const char *                  filename,
         size_t                        size );


_BINDLE_F int
bindle_readline(
         int                           fd,
         char *                        str,
         size_t                        size );


//----------------//
// map prototypes //
//----------------//
#pragma mark map prototypes

_BINDLE_F int
bindle_map_lookup(
         const bindle_map_t *          map,
         const char *                  name,
         uintptr_t                     value,
         const bindle_map_t **         mapp );


_BINDLE_F uintptr_t
bindle_map_lookup_name(
         const bindle_map_t *          map,
         const char *                  name,
         const bindle_map_t **         mapp );


_BINDLE_F const char *
bindle_map_lookup_value(
         const bindle_map_t *          map,
         uintptr_t                     value,
         const bindle_map_t **         mapp );


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
bindle_release(
         bindle_obj_t *                obj );


_BINDLE_F void *
bindle_retain(
         bindle_obj_t *                obj );


_BINDLE_F intptr_t
bindle_retain_count(
         bindle_obj_t *                obj );


//-------------------//
// option prototypes //
//-------------------//
#pragma mark option prototypes

_BINDLE_F int
bindle_get_option(
         void *                        ptr,
         int                           option,
         void *                        outvalue );


_BINDLE_F int
bindle_set_option(
         void *                        ptr,
         int                           option,
         const void *                  invalue );


//------------------------//
// user prompt prototypes //
//------------------------//
#pragma mark user prompt prototypes

char *
bindle_getpass(
         const char *                  prompt );


char *
bindle_getpass_r(
         const char *                  prompt,
         char *                        pass,
         size_t                        passlen );


//-------------------//
// string prototypes //
//-------------------//
#pragma mark string prototypes

_BINDLE_F char *
bindle_basename(
         const char *                  path );


_BINDLE_F char *
bindle_basename_r(
         const char *                  path,
         char *                        bname,
         size_t                        bnamelen );


_BINDLE_F char *
bindle_dirname(
         const char *                  path );


_BINDLE_F char *
bindle_dirname_r(
         const char *                  path,
         char *                        dname,
         size_t                        dnamelen );


// based on perl chomp()
_BINDLE_F size_t
bindle_strchomp(
         char *                        str,
         const char *                  separator );


// based on perl chop()
_BINDLE_F char
bindle_strchop(
         char *                        str );


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


_BINDLE_F int
bindle_strsplit(
         const char *                  str,
         int                           delim,
         char ***                      argvp,
         int *                         argcp );


//----------------//
// URI prototypes //
//----------------//
#pragma mark URI prototypes

_BINDLE_F int
bindle_is_url(
         const char *                  str );


_BINDLE_F char *
bindle_urldesc2str(
         BindleURLDesc *               budp );


_BINDLE_F void
bindle_urldesc_free(
         BindleURLDesc *               budp );


_BINDLE_F int
bindle_urldesc_parse(
         const char *                  url,
         BindleURLDesc **              budpp );


_BINDLE_F int
bindle_urldesc_resolve(
         BindleURLDesc *               budp,
         int                           family,
         unsigned                      default_port );


_BINDLE_F ssize_t
bindle_url_decode(
         const char *                  src,
         char * restrict               dst,
         size_t                        size );


_BINDLE_F ssize_t
bindle_url_encode(
         const char *                  src,
         char * restrict               dst,
         size_t                        size );


_BINDLE_F size_t
bindle_url_encode_len(
         const char *                  str );


//-----------------//
// UTF8 prototypes //
//-----------------//
#pragma mark UTF8 prototypes

_BINDLE_F size_t
bindle_utf8len(
         const char *                  s );


_BINDLE_F size_t
bindle_utf8nlen(
         const char *                  s,
         size_t                        maxlen );


_BINDLE_F ssize_t
bindle_utf8test(
         const char *                  s,
         size_t                        maxlen );


#endif /* end of header */
