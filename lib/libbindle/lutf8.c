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
#define _LIB_LIBBINDLE_LUTF8_C 1
#include "lutf8.h"


///////////////
//           //
//  Headers  //
//           //
///////////////
#pragma mark - Headers

#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <assert.h>


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#pragma mark - Definitions


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#pragma mark - Prototypes


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#pragma mark - Functions


size_t
bindle_utf8len(
         const char *                  s )
{
   const uint8_t * b;
   size_t          l;
   size_t          p;

   b = (const uint8_t *) s;
   l = 0;

   for(p = 0; (b[p] != 0x00); p++)
      if ((b[p] & 0xC0) != 0x80)
         l++;

   return(l);
}


size_t
bindle_utf8nlen(
         const char *                  s,
         size_t                        maxlen )
{
   const uint8_t * b;
   size_t          l;
   size_t          p;

   b = (const uint8_t *) s;
   l = 0;

   for(p = 0; ((b[p] != 0x00) && (p < maxlen)); p++)
      if ((b[p] & 0xC0) != 0x80)
         l++;

   return(l);
}


// UTF-8 Description Table:
//    Bits   First       Last     Bytes   Byte 0    Byte 1    Byte 2    Byte 3    Byte 4    Byte 5
//     7      U+0000      U+007F    1    0xxxxxxx
//     0         N/A         N/A         10xxxxxx
//    11      U+0080      U+07FF    2    110xxxxx  10xxxxxx
//    16      U+0800      U+FFFF    3    1110xxxx  10xxxxxx  10xxxxxx
//    21     U+10000    U+1FFFFF    4    11110xxx  10xxxxxx  10xxxxxx  10xxxxxx
//    26    U+200000   U+3FFFFFF    5    111110xx  10xxxxxx  10xxxxxx  10xxxxxx  10xxxxxx
//    31   U+4000000  U+7FFFFFFF    6    1111110x  10xxxxxx  10xxxxxx  10xxxxxx  10xxxxxx  10xxxxxx
ssize_t
bindle_utf8test(
         const char *                  s,
         size_t                        maxlen )
{
   const uint8_t * buff;
   ssize_t         bit;
   ssize_t         byte;
   ssize_t         width;
   ssize_t         len;
   ssize_t         pos;

   buff = (const uint8_t *) s;
   len  = 0;

   for(pos = 0; ((buff[pos] != 0x00) && (pos < ((ssize_t)maxlen))); pos++)
   {
      // determines character width (byte size)
      width = 1;
      for(bit = 0; ((bit < 8) && (width == 1)); bit++)
         if (((buff[pos] << bit) & 0x80) == 0x00)
            width = bit;

      // only continuation bytes have a calculated width of 1
      if (width == 1)
         return(-1);

      // verify that UTF-8 string is not longer than buffer
      if ((pos+width) >= ((ssize_t)maxlen))
         return(-1);

      // skip continuation bits
      for (byte = 1; byte < width; byte++)
      {
         pos++;
         if ((buff[pos] & 0xC0) != 0x80)
            return(-1);
      };

      len++;
   };

   return(len);
}

/* end of source */
