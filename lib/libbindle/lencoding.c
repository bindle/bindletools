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
#define _LIB_LIBBINDLE_LENCODING_C 1
#include "lencoding.h"

///////////////
//           //
//  Headers  //
//           //
///////////////
// MARK: - Headers

#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <errno.h>


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
// MARK: - Prototypes

static ssize_t
bindle_base32_decode(
         const int8_t *                map,
         uint8_t *                     dst,
         size_t                        s,
         const char *                  src,
         size_t                        n );


static ssize_t
bindle_base32_encode(
         const char *                  map,
         char *                        dst,
         size_t                        s,
         const uint8_t *               src,
         size_t                        n,
         int                           nopad );


static ssize_t
bindle_base32_verify(
         const int8_t *                map,
         const char *                  src,
         size_t                        n );


static ssize_t
bindle_base64_decode(
         const int8_t *                map,
         uint8_t *                     dst,
         size_t                        s,
         const char *                  src,
         size_t                        n );


static ssize_t
bindle_base64_encode(
         const char *                  map,
         char *                        dst,
         size_t                        s,
         const uint8_t *               src,
         size_t                        n,
         int                           nopad );


static ssize_t
bindle_base64_verify(
         const int8_t *                map,
         const char *                  src,
         size_t                        n );


static ssize_t
bindle_hex_decode(
         const int8_t *                map,
         uint8_t *                     dst,
         size_t                        s,
         const char *                  src,
         size_t                        n );


static ssize_t
bindle_hex_encode(
         const char *                  map,
         char *                        dst,
         size_t                        s,
         const uint8_t *               src,
         size_t                        n );


static ssize_t
bindle_hex_verify(
         const int8_t *                map,
         const char *                  src,
         size_t                        n );


static int
bindle_encode_method(
         int                           method );


static ssize_t
bindle_none_decode(
         uint8_t *                     dst,
         size_t                        s,
         const char *                  src,
         size_t                        n );


static ssize_t
bindle_none_encode(
         char *                        dst,
         size_t                        s,
         const int8_t *                src,
         size_t                        n );


static ssize_t
bindle_pctenc_decode(
         uint8_t *                     dst,
         size_t                        s,
         const char *                  src,
         size_t                        n );


static size_t
bindle_pctenc_decode_size(
         const char *                  src );


static ssize_t
bindle_pctenc_encode(
         char *                        dst,
         size_t                        s,
         const uint8_t *               src,
         size_t                        n );


static size_t
bindle_pctenc_encode_size(
         const uint8_t *               src,
         size_t                        n );


static ssize_t
bindle_pctenc_verify(
         const char *                  src,
         size_t                        n );


/////////////////
//             //
//  Variables  //
//             //
/////////////////
// MARK: - Variables

// MARK: base32_vals[]
static const int8_t base32_vals[256] =
{
//    This map cheats and interprets:
//       - the numeral zero as the letter "O" as in oscar
//       - the numeral one as the letter "L" as in lima
//       - the numeral eight as the letter "B" as in bravo
// 00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x00
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x10
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x20
   14, 11, 26, 27, 28, 29, 30, 31,  1, -1, -1, -1, -1,  0, -1, -1, // 0x30
   -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, // 0x40
   15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, // 0x50
   -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, // 0x60
   15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, -1, -1, -1, -1, // 0x70
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x80
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x90
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xA0
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xB0
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xC0
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xD0
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xE0
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xF0
};
// MARK: base32_chars[]
static const char * base32_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567=";


// MARK: base32hex_vals[]
static const int8_t base32hex_vals[256] =
{
// 00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x00
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x10
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x20
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1,  0, -1, -1, // 0x30
   -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, // 0x40
   25, 26, 27, 28, 29, 30, 31, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x50
   -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, // 0x60
   25, 26, 27, 28, 29, 30, 31, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x70
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x80
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x90
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xA0
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xB0
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xC0
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xD0
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xE0
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xF0
};
// MARK: base32hex_chars[]
static const char * base32hex_chars = "0123456789ABCDEFGHIJKLMNOPQRSTUV=";


// MARK: base64_vals[]
static const int8_t base64_vals[256] =
{
// 00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x00
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x10
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63, // 0x20
   52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1,  0, -1, -1, // 0x30
   -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, // 0x40
   15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, // 0x50
   -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, // 0x60
   41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1, // 0x70
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x80
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x90
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xA0
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xB0
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xC0
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xD0
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xE0
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xF0
};
// MARK: base64_chars[]
static const char * base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";


// MARK: crockford_vals[]
static const int8_t crockford_vals[256] =
{
// 00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x00
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x10
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x20
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1, // 0x30
   -1, 10, 11, 12, 13, 14, 15, 16, 17,  1, 18, 19,  1, 20, 21,  0, // 0x40
   22, 23, 24, 25, 26, -1, 27, 28, 29, 30, 31, -1, -1, -1, -1, -1, // 0x50
   -1, 10, 11, 12, 13, 14, 15, 16, 17,  1, 18, 19,  1, 20, 21,  0, // 0x60
   22, 23, 24, 25, 26, -1, 27, 28, 29, 30, 31, -1, -1, -1, -1, -1, // 0x70
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x80
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x90
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xA0
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xB0
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xC0
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xD0
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xE0
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xF0
};
// MARK: crockford_chars[]
static const char * crockford_chars = "0123456789ABCDEFGHJKMNPQRSTVWXYZ";


// MARK: hex_vals[]
static const int8_t hex_vals[256] =
{
// 00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x00
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x10
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x20
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1, // 0x30
   -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x40
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x50
   -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x60
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x70
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x80
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0x90
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xA0
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xB0
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xC0
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xD0
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xE0
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0xF0
};
// MARK: hex_chars[]
static const char * hex_chars = "0123456789abcdef";


// MARK: pctenc_vals[]
static const int8_t pctenc_vals[256] =
{
//  00   01   02   03   04   05   06   07   08   09   0A   0B   0C   0D   0E   0F
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, // 0x00
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, // 0x10
   '+',  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, ',', '-', '.', '/', // 0x20
   '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';',  -1,  -1,  -1,  -1, // 0x30
   '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', // 0x40
   'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',  -1,  -1,  -1,  -1, '_', // 0x50
    -1, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', // 0x60
   'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',  -1,  -1,  -1, '~',  -1, // 0x70
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, // 0x80
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, // 0x90
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, // 0xA0
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, // 0xB0
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, // 0xC0
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, // 0xD0
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, // 0xE0
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, '~',  -1, // 0xF0
};


/////////////////
//             //
//  Functions  //
//             //
/////////////////
// MARK: - Functions

//------------------//
// base32 functions //
//------------------//
// MARK: base32 functions

ssize_t
bindle_base32_decode(
         const int8_t *                map,
         uint8_t *                     dst,
         size_t                        s,
         const char *                  src,
         size_t                        n )
{
   size_t      datlen;
   size_t      pos;
   ssize_t     rc;

   assert(dst != NULL);
   assert(src != NULL);
   assert(s   >  0);

   // verifies encoded data contains only valid characters
   if ((rc = bindle_base32_verify(map, (const char *)src, n)) == -1)
   {
      errno = EBADMSG;
      return(-1);
   };
   if ( (rc > (ssize_t)s) && ((dst)) )
   {
      errno = ENOBUFS;
      return(-1);
   };

   // decodes base32 encoded data
   datlen = 0;
   for(pos = 0; (pos < n); pos++)
   {
      // MSB is Most Significant Bits  (0x80 == 10000000 ~= MSB)
      // MB is middle bits             (0x7E == 01111110 ~= MB)
      // LSB is Least Significant Bits (0x01 == 00000001 ~= LSB)
      switch(pos%8)
      {
         // byte 0
         case 1:
         dst[datlen]  = (map[(unsigned char)src[pos-1]] << 3) & 0xF8; // 5 MSB
         dst[datlen] |= (map[(unsigned char)src[pos-0]] >> 2) & 0x07; // 3 LSB
         datlen++;
         break;

         // byte 2
         case 2:
         if (src[pos] == '=')
            return((ssize_t)datlen);
         break;

         // byte 3
         case 3:
         dst[datlen]  = (map[(unsigned char)src[pos-2]] << 6) & 0xC0; // 2 MSB
         dst[datlen] |= (map[(unsigned char)src[pos-1]] << 1) & 0x3E; // 5  MB
         dst[datlen] |= (map[(unsigned char)src[pos-0]] >> 4) & 0x01; // 1 LSB
         datlen++;
         break;

         // byte 4
         case 4:
         if (src[pos] == '=')
            return((ssize_t)datlen);
         dst[datlen]  = (map[(unsigned char)src[pos-1]] << 4) & 0xF0; // 4 MSB
         dst[datlen] |= (map[(unsigned char)src[pos-0]] >> 1) & 0x0F; // 4 LSB
         datlen++;
         break;

         // byte 5;
         case 5:
         if (src[pos] == '=')
            return((ssize_t)datlen);
         break;

         // byte 6
         case 6:
         dst[datlen]  = (map[(unsigned char)src[pos-2]] << 7) & 0x80; // 1 MSB
         dst[datlen] |= (map[(unsigned char)src[pos-1]] << 2) & 0x7C; // 5  MB
         dst[datlen] |= (map[(unsigned char)src[pos-0]] >> 3) & 0x03; // 2 LSB
         datlen++;
         break;

         // byte 7
         case 7:
         if (src[pos] == '=')
            return((ssize_t)datlen);
         dst[datlen]  = (map[(unsigned char)src[pos-1]] << 5) & 0xE0; // 3 MSB
         dst[datlen] |= (map[(unsigned char)src[pos-0]] >> 0) & 0x1F; // 5 LSB
         datlen++;
         break;

         default:
         if (src[pos] == '=')
            return((ssize_t)datlen);
         break;
      };
   };

   return((ssize_t)datlen);
}


ssize_t
bindle_base32_encode(
         const char *                  map,
         char *                        dst,
         size_t                        s,
         const uint8_t *               src,
         size_t                        n,
         int                           nopad )
{
   ssize_t     len;
   size_t      dpos;
   size_t      spos;
   size_t      byte;
   uint8_t *   dat;

   assert(dst != NULL);
   assert(src != NULL);
   assert(s   >  0);

   dat = (uint8_t *)dst;

   // calculates each digit's value
   byte = 0;
   dpos = 0;
   for(spos = 0; (spos < n); spos++)
   {
      // MSB is Most Significant Bits  (0x80 == 10000000 ~= MSB)
      // MB is middle bits             (0x7E == 01111110 ~= MB)
      // LSB is Least Significant Bits (0x01 == 00000001 ~= LSB)
      switch(byte)
      {
         case 0:
         dat[dpos++]  =  src[spos] >> 3;         // 5 MSB
         dat[dpos++]  = (src[spos] & 0x07) << 2; // 3 LSB   2 bits unused
         byte++;
         break;

         case 1:
         dat[dpos-1] |= (src[spos] >> 6) & 0x03;  // 2 MSB
         dat[dpos++]  = (src[spos] >> 1) & 0x1f ; // 5 MB
         dat[dpos++]  = (src[spos] << 4) & 0x10;  // 1 LSB   4 bits unused
         byte++;
         break;

         case 2:
         dat[dpos-1] |=  src[spos] >> 4;          // 4 MSB
         dat[dpos++]  = (src[spos] << 1) & 0x1e ; // 4 LSB   1 bits unused
         byte++;
         break;

         case 3:
         dat[dpos-1] |=  src[spos] >> 7;          // 1 MSB
         dat[dpos++]  = (src[spos] >> 2) & 0x1f ; // 5 MB
         dat[dpos++]  = (src[spos] << 3) & 0x18 ; // 2 LSB   3 bits unused
         byte++;
         break;

         case 4:
         default:
         dat[dpos-1] |=  src[spos] >> 5;          // 3 MSB
         dat[dpos++]  =  src[spos] & 0x1f;        // 5 LSB
         byte = 0;
         break;
      };
   };

   // encodes each value
   for(len = 0; ((size_t)len) < dpos; len++)
      dst[len] = map[(unsigned char)dat[len]];

   // add padding
   if (!(nopad))
      for(; ((len % 8)); len++)
         dst[len] = '=';

   dst[len] = '\0';

   return(len);
}


ssize_t
bindle_base32_verify(
         const int8_t *                map,
         const char *                  src,
         size_t                        n )
{
   size_t   pos;
   size_t   datlen;

   assert(map != NULL);
   assert(src != NULL);

   datlen = 0;

   // verifies encoded data contains only valid characters
   for(pos = 0; (pos < n); pos++)
   {
      // verify that data is valid character
      if (map[(unsigned char)src[pos]] == -1)
      {
         errno = EINVAL;
         return(-1);
      };
      // verify valid use of padding
      if (src[pos] != '=')
         continue;
      if (!(datlen))
         datlen = pos;
      if ((pos % 8) < 2)
      {
         errno = EINVAL;
         return(-1);
      };
      if ((pos + (8-(pos%8))) != n)
      {
         errno = EINVAL;
         return(-1);
      };
      for(; (pos < n); pos++)
      {
         if (src[pos] != '=')
         {
            errno = EINVAL;
            return(-1);
         };
      };
   };

   if (!(datlen))
      datlen = pos;

   switch(datlen % 8)
   {
      case 0:
      case 2:
      case 4:
      case 5:
      case 7:
      break;

      case 1:
      case 3:
      case 6:
      default:
      return(-1);
   };

   return((datlen * 5) / 8);
}


//------------------//
// base64 functions //
//------------------//
// MARK: base64 functions

ssize_t
bindle_base64_decode(
         const int8_t *                map,
         uint8_t *                     dst,
         size_t                        s,
         const char *                  src,
         size_t                        n )
{
   size_t      datlen;
   size_t      pos;
   ssize_t     rc;

   assert(dst != NULL);
   assert(src != NULL);
   assert(s   >  0);

   // verifies encoded data contains only valid characters
   if ((rc = bindle_base64_verify(map, (const char *)src, n)) == -1)
   {
      errno = EBADMSG;
      return(-1);
   };
   if ( (rc > (ssize_t)s) && ((dst)) )
   {
      errno = ENOBUFS;
      return(-1);
   };

   // decodes base64 encoded data
   datlen = 0;
   for(pos = 0; (pos < n); pos++)
   {
      // MSB is Most Significant Bits  (0x80 == 10000000 ~= MSB)
      // MB is middle bits             (0x7E == 01111110 ~= MB)
      // LSB is Least Significant Bits (0x01 == 00000001 ~= LSB)
      switch(pos%4)
      {
         // byte 0
         case 0:
         dst[datlen++]  = (map[(unsigned char)src[pos]] & 0x3f) << 2;  // 6 MSB
         break;

         // byte 1
         case 1:
         dst[datlen-1] |= (map[(unsigned char)src[pos]] & 0x30) >> 4; // 2 LSB
         dst[datlen++]  = (map[(unsigned char)src[pos]] & 0x0f) << 4; // 4 MSB
         break;

         // byte 2
         case 2:
         if (src[pos] == '=')
            return((ssize_t)datlen-1);
         dst[datlen-1] |= (map[(unsigned char)src[pos]] & 0x3c) >> 2; // 4 MSB
         dst[datlen++]  = (map[(unsigned char)src[pos]] & 0x03) << 6; // 2 LSB
         break;

         // byte 3
         case 3:
         default:
         if (src[pos] == '=')
            return((ssize_t)datlen-1);
         dst[datlen-1] |= (map[(unsigned char)src[pos]] & 0x3f);    // 1 MSB
         break;
      };
   };
   return((ssize_t)datlen);
}


ssize_t
bindle_base64_encode(
         const char *                  map,
         char *                        dst,
         size_t                        s,
         const uint8_t *               src,
         size_t                        n,
         int                           nopad )
{
   ssize_t   len;
   size_t    dpos;
   size_t    spos;
   size_t    byte;
   uint8_t * dat;

   assert(dst != NULL);
   assert(src != NULL);
   assert(s   >  0);

   dat = (uint8_t *)dst;

   // calculates each digit's value
   byte = 0;
   dpos = 0;
   for(spos = 0; (spos < n); spos++)
   {
      // MSB is Most Significant Bits  (0x80 == 10000000 ~= MSB)
      // MB is middle bits             (0x7E == 01111110 ~= MB)
      // LSB is Least Significant Bits (0x01 == 00000001 ~= LSB)
      switch(byte)
      {
         case 0:
         dat[dpos++]  = (src[spos] & 0xfc) >> 2;  // 6 MSB
         dat[dpos++]  = (src[spos] & 0x03) << 4;  // 2 LSB
         byte++;
         break;

         case 1:
         dat[dpos-1] |= (src[spos] & 0xf0) >> 4;  // 4 MSB
         dat[dpos++]  = (src[spos] & 0x0f) << 2;  // 4 LSB
         byte++;
         break;

         case 2:
         default:
         dat[dpos-1] |= (src[spos] & 0xc0) >> 6;  // 2 MSB
         dat[dpos++]  =  src[spos] & 0x3f;        // 6 LSB
         byte = 0;
         break;
      };
   };

   // encodes each value
   for(len = 0; ((size_t)len) < dpos; len++)
      dst[len] = map[dat[len]];

   // add padding
   if (!(nopad))
      for(; ((len % 4)); len++)
         dst[len] = '=';

   dst[len] = '\0';

   return(len);
}


ssize_t
bindle_base64_verify(
         const int8_t *                map,
         const char *                  src,
         size_t                        n )
{
   size_t   pos;
   size_t   datlen;

   assert(map != NULL);
   assert(src != NULL);

   datlen = 0;

   // verifies encoded data contains only valid characters
   for(pos = 0; (pos < n); pos++)
   {
      // verify that data is valid character
      if (map[(unsigned char)src[pos]] == -1)
         return(-1);
      // verify valid use of padding
      if (src[pos] != '=')
         continue;
      if (!(datlen))
         datlen = pos;
      if ((pos % 4) < 2)
         return(-1);
      if ((pos + (4-(pos%4))) != n)
         return(-1);
      for(; (pos < n); pos++)
         if (src[pos] != '=')
            return(-1);
   };

   if (!(datlen))
      datlen = pos;

   switch(datlen % 4)
   {
      case 0:
      case 2:
      case 3:
      break;

      case 1:
      default:
      return(-1);
   };

   return((datlen * 6) / 8);
}


//---------------//
// hex functions //
//---------------//
// MARK: hex functions

ssize_t
bindle_hex_decode(
         const int8_t *                map,
         uint8_t *                     dst,
         size_t                        s,
         const char *                  src,
         size_t                        n )
{
   size_t      datlen;
   size_t      pos;
   ssize_t     rc;

   assert(dst != NULL);
   assert(src != NULL);
   assert(s   >  0);

   // verifies encoded data contains only valid characters
   if ((rc = bindle_base64_verify(map, (const char *)src, n)) == -1)
   {
      errno = EBADMSG;
      return(-1);
   };
   if ( (rc > (ssize_t)s) && ((dst)) )
   {
      errno = ENOBUFS;
      return(-1);
   };

   // decodes base64 encoded data
   datlen = 0;
   for(pos = 0; (pos < n); pos++)
   {
      switch(pos%2)
      {
         // byte 0
         case 0:
         dst[datlen++]  = (map[(unsigned char)src[pos]] & 0x0f) << 4;
         break;

         // byte 1
         case 1:
         default:
         dst[datlen-1] |=  map[(unsigned char)src[pos]] & 0x0f;
         break;
      };
   };

   return((ssize_t)datlen);
}


ssize_t
bindle_hex_encode(
         const char *                  map,
         char *                        dst,
         size_t                        s,
         const uint8_t *               src,
         size_t                        n )
{
   ssize_t   len;
   size_t    dpos;
   size_t    spos;
   uint8_t * dat;

   assert(dst != NULL);
   assert(src != NULL);
   assert(s   >  0);

   dat = (uint8_t *)dst;

   // calculates each digit's value
   dpos = 0;
   for(spos = 0; (spos < n); spos++)
   {
      dat[dpos++]  = (src[spos] & 0xf0) >> 4;
      dat[dpos++]  = (src[spos] & 0x0f);
   };

   // encodes each value
   for(len = 0; ((size_t)len) < dpos; len++)
      dst[len] = map[dat[len]];

   dst[len] = '\0';

   return(len);
}


ssize_t
bindle_hex_verify(
         const int8_t *                map,
         const char *                  src,
         size_t                        n )
{
   size_t   pos;

   assert(map != NULL);
   assert(src != NULL);

   if ((n%2))
      return(-1);

   for(pos = 0; (pos < n); pos++)
      if (map[(unsigned char)src[pos]] == -1)
         return(-1);

   return(n/2);
}


//----------------//
// none functions //
//----------------//
// MARK: none functions

ssize_t
bindle_none_decode(
         uint8_t *                     dst,
         size_t                        s,
         const char *                  src,
         size_t                        n )
{
   assert(dst != NULL);
   assert(src != NULL);
   if (n > s)
   {
      errno = ENOBUFS;
      return(-1);
   };
   memcpy(dst, src, n);
   return((ssize_t)n);
}


ssize_t
bindle_none_encode(
         char *                        dst,
         size_t                        s,
         const int8_t *                src,
         size_t                        n )
{
   assert(dst != NULL);
   assert(src != NULL);

   if (n > s)
   {
      errno = ENOBUFS;
      return(-1);
   };
   memcpy(dst, src, n);

   return((ssize_t)n);
}


//----------------------------//
// percent-encoding functions //
//----------------------------//
// MARK: percent-encoding functions

ssize_t
bindle_pctenc_decode(
         uint8_t *                     dst,
         size_t                        s,
         const char *                  src,
         size_t                        n )
{
   ssize_t     dpos;
   size_t      spos;
   ssize_t     rc;
   char        hex[3];

   assert(dst != NULL);
   assert(src != NULL);
   assert(s   >  0);

   // verifies encoded data contains only valid characters
   if ((rc = bindle_pctenc_verify((const char *)src, n)) == -1)
   {
      errno = EBADMSG;
      return(-1);
   };
   if ( (rc >= ((ssize_t)s-1)) && ((dst)) )
   {
      errno = ENOBUFS;
      return(-1);
   };

   // decodes percent-encode encoded data
   for(dpos = 0, spos = 0; (spos < n); dpos++, spos++)
   {
      if (pctenc_vals[(uint8_t)src[spos]] != -1)
         dst[dpos] = (uint8_t)src[spos];
      else if (src[spos] == '+')
         dst[dpos] = ' ' ;
      else if (src[spos] == '%')
      {
         hex[0] = src[spos+1];
         hex[1] = src[spos+2];
         hex[2] = '\0';
         dst[dpos] = strtoul(hex, NULL, 16);
         spos += 2;
      }
      else
         return(-1);
   };

   dst[dpos] = '\0';

   return(dpos);
}


size_t
bindle_pctenc_decode_size(
         const char *               src )
{
   size_t      size;
   size_t      pos;
   size_t      n;
   if (!(src))
      return(0);
   n    = strlen(src);
   size = n;
   for(pos = 0; (pos < n); pos++)
   {
      if (src[pos] == '%')
      {
         size -= 2;
         pos  += 2;
      };
   };
   return(size);
}


ssize_t
bindle_pctenc_encode(
         char *                        dst,
         size_t                        s,
         const uint8_t *               src,
         size_t                        n )
{
   size_t      dpos;
   size_t      spos;

   assert(dst != NULL);
   assert(src != NULL);
   assert(s   >  0);

   if (s == 0)
      return(0);
   s--;

   for(spos = 0, dpos = 0; (spos < n); spos++, dpos++)
   {
      if ((dst[dpos] = pctenc_vals[src[spos]]) != -1)
         continue;
      if (src[spos] == ' ')
      {
         dst[dpos] = '+';
         continue;
      };
      dst[dpos+0] = '%';
      dst[dpos+1] = hex_chars[(src[spos] >> 4) & 0x0f];
      dst[dpos+2] = hex_chars[ src[spos]       & 0x0f];
      dpos += 2;
   };

   dst[dpos] = '\0';

   return((ssize_t)dpos);
}


size_t
bindle_pctenc_encode_size(
         const uint8_t *               src,
         size_t                        n )
{
   size_t      size;
   size_t      pos;
   if (!(src))
      return(0);
   for(pos = 0, size = 1; (pos < n); pos++, size++)
      if (pctenc_vals[src[pos]] == -1)
         size += 2;
   return(size-1);
}


ssize_t
bindle_pctenc_verify(
         const char *                  src,
         size_t                        n )
{
   size_t   pos;
   ssize_t  size;
   assert(src != NULL);
   for(pos = 0, size = 0; (pos < n); pos++, size++)
   {
      if (src[pos] == '%')
      {
         if ((isxdigit(src[pos+1]) == 0) || (isxdigit(src[pos+2]) == 0))
            return(-1);
         pos += 2;
         continue;
      }
      if (src[pos] == '+')
         continue;
      if (pctenc_vals[(unsigned char)src[pos]] == -1)
         return(-1);
   };
   return(size);
}


//--------------------//
// frontend functions //
//--------------------//
// MARK: frontend functions

ssize_t
bindle_decode(
         int                           method,
         void *                        dst,
         size_t                        s,
         const char *                  src,
         size_t                        n )
{
   assert(dst != NULL);
   assert(src != NULL);

   // validate encoding method
   if (bindle_encode_method(method) == -1)
      return(-1);

   // validates buffer is big enough
   if ((method & ~BNDL_NOPAD) == BNDL_PCTENC)
   {
      if (s <= (size_t)bindle_pctenc_decode_size(src))
      {
         errno = ENOBUFS;
         return(-1);
      };
   } else if (s <= (size_t)bindle_decode_size(method, n))
   {
      errno = ENOBUFS;
      return(-1);
   };

   switch(method & ~BNDL_NOPAD)
   {
      case BNDL_BASE32:
      return(bindle_base32_decode(base32_vals, dst, s, src, n));

      case BNDL_BASE32HEX:
      return(bindle_base32_decode(base32hex_vals, dst, s, src, n));

      case BNDL_BASE64:
      return(bindle_base64_decode(base64_vals, dst, s, src, n));

      case BNDL_CROCKFORD:
      return(bindle_base32_decode(crockford_vals, dst, s, src, n));

      case BNDL_HEX:
      return(bindle_hex_decode(hex_vals, dst, s, src, n));

      case BNDL_NONE:
      return(bindle_none_decode(dst, s, src, n));

      case BNDL_PCTENC:
      return(bindle_pctenc_decode(dst, s, src, n));

      default:
      break;
   };

   errno = ENOTSUP;
   return(-1);
}


ssize_t
bindle_decode_size(
         int                           method,
         size_t                        n )
{
   ssize_t len;

   switch(method)
   {
      case BNDL_BASE32:
      case BNDL_BASE32HEX:
      case BNDL_CROCKFORD:
      return( ((n / 8) + (((n % 8)) ? 1 : 0)) * 5 );

      case BNDL_BASE32_NOPAD:
      case BNDL_BASE32HEX_NOPAD:
      case BNDL_CROCKFORD_NOPAD:
      len = (n / 8) * 5;
      switch( n % 8 )
      {
         case 1: return(-1);
         case 2: return(len + 1);
         case 3: return(-1);
         case 4: return(len + 2);
         case 5: return(len + 3);
         case 6: return(-1);
         case 7: return(len + 4);
         default: break;
      };
      return( len );

      case BNDL_BASE64:
      return( ((n / 4) + (((n % 4)) ? 1 : 0)) * 3 );

      case BNDL_BASE64_NOPAD:
      len = (n / 4) * 3;
      switch( n % 4 )
      {
         case 1: return(-1);
         case 2: return(len + 1);
         case 3: return(len + 2);
         default: break;
      };
      return( len );

      case BNDL_HEX:
      case BNDL_HEX_NOPAD:
      return( (n / 2) + (((n % 2)) ? 1 : 0) );

      case BNDL_NONE:
      case BNDL_NONE_NOPAD:
      return((ssize_t)n);

      case BNDL_PCTENC:
      case BNDL_PCTENC_NOPAD:
      return((ssize_t)(n+1));

      default:
      break;
   };

   return(-1);
}


ssize_t
bindle_encode(
         int                           method,
         char *                        dst,
         size_t                        s,
         const void *                  src,
         size_t                        n )
{
   int nopad;

   assert(dst != NULL);
   assert(src != NULL);

   nopad = ((method & BNDL_NOPAD)) ? 1 : 0;

   // validate encoding method
   if (bindle_encode_method(method) == -1)
      return(-1);

   // validates buffer is big enough
   if ((method & ~BNDL_NOPAD) == BNDL_PCTENC)
   {
      if (s <= (size_t)bindle_pctenc_encode_size(src, n))
      {
         errno = ENOBUFS;
         return(-1);
      };
   } else if (s <= (size_t)bindle_encode_size(method, n))
   {
      errno = ENOBUFS;
      return(-1);
   };

   switch(method & ~BNDL_NOPAD)
   {
      case BNDL_BASE32:
      return(bindle_base32_encode(base32_chars, dst, s, src, n, nopad));

      case BNDL_BASE32HEX:
      return(bindle_base32_encode(base32hex_chars, dst, s, src, n, nopad));

      case BNDL_BASE64:
      return(bindle_base64_encode(base64_chars, dst, s, src, n, nopad));

      case BNDL_CROCKFORD:
      return(bindle_base32_encode(crockford_chars, dst, s, src, n, 1));

      case BNDL_HEX:
      return(bindle_hex_encode(hex_chars, dst, s, src, n));

      case BNDL_NONE:
      return(bindle_none_encode(dst, s, src, n));

      case BNDL_PCTENC:
      return(bindle_pctenc_encode(dst, s, src, n));

      default:
      errno = ENOTSUP;
      break;
   };

   return(-1);
}


int
bindle_encode_method(
         int                           method )
{
   method &= ~ BNDL_NOPAD;
   switch(method)
   {
      case BNDL_BASE32:
      case BNDL_BASE32HEX:
      case BNDL_BASE64:
      case BNDL_CROCKFORD:
      case BNDL_HEX:
      case BNDL_NONE:
      case BNDL_PCTENC:
      return(method);

      default:
      errno = ENOTSUP;
      break;
   };
   return(-1);
}


ssize_t
bindle_encode_size(
         int                           method,
         size_t                        n )
{
   ssize_t len;

   switch(method)
   {
      case BNDL_BASE32:
      case BNDL_BASE32HEX:
      return( ((n / 5) + (((n % 5)) ? 1 : 0)) * 8 );

      case BNDL_BASE32_NOPAD:
      case BNDL_BASE32HEX_NOPAD:
      case BNDL_CROCKFORD:
      case BNDL_CROCKFORD_NOPAD:
      len = (n / 5) * 8;
      switch(n % 5)
      {
         case 1:  return(len + 2);
         case 2:  return(len + 4);
         case 3:  return(len + 5);
         case 4:  return(len + 7);
         default: break;
      };
      return( len );

      case BNDL_BASE64:
      return( ((n / 3) + (((n % 3)) ? 1 : 0)) * 4 );

      case BNDL_BASE64_NOPAD:
      len = (n / 3) * 4;
      switch(n % 3)
      {
         case 1:  return(len + 2);
         case 2:  return(len + 3);
         default: break;
      };
      return( len );

      case BNDL_HEX:
      case BNDL_HEX_NOPAD:
      return( (ssize_t)(n * 2) );

      case BNDL_NONE:
      case BNDL_NONE_NOPAD:
      return((ssize_t)n);

      case BNDL_PCTENC:
      case BNDL_PCTENC_NOPAD:
      return((ssize_t)((n*3)+1));

      default:
      errno = ENOTSUP;
      break;
   };

   return(-1);
}


int
bindle_encoded_char(
         int                           method,
         char                          c )
{
   unsigned char u = (unsigned char)c;
   switch(method)
   {
      case BNDL_BASE32:
      return(base32_vals[u]);

      case BNDL_BASE32HEX:
      return(base32hex_vals[u]);

      case BNDL_BASE64:
      return(base64_vals[u]);

      case BNDL_CROCKFORD:
      return(crockford_vals[u]);

      case BNDL_HEX:
      return(hex_vals[u]);

      case BNDL_NONE:
      return((int)u);

      case BNDL_PCTENC:
      return(pctenc_vals[u]);

      default:
      break;
   };
   return(-1);
}


ssize_t
bindle_encoding_verify(
         int                           method,
         const char *                  src,
         size_t                        n )
{
   switch(method)
   {
      case BNDL_BASE32:
      return(bindle_base32_verify(base32_vals, src, n));

      case BNDL_BASE32HEX:
      return(bindle_base32_verify(base32hex_vals, src, n));

      case BNDL_BASE64:
      return(bindle_base64_verify(base64_vals, src, n));

      case BNDL_CROCKFORD:
      return(bindle_base32_verify(crockford_vals, src, n));

      case BNDL_HEX:
      return(bindle_hex_verify(hex_vals, src, n));

      case BNDL_NONE:
      return((ssize_t)n);

      case BNDL_PCTENC:
      return(bindle_pctenc_verify(src, n));

      default:
      errno = ENOTSUP;
      break;
   };

   return(-1);
}


/* end of source file */
