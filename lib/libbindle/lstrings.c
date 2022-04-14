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
#define _LIB_LIBBINDLE_LSTRINGS_C 1
#include "lstrings.h"


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
#include <ctype.h>
#include <sys/errno.h>
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

int
bindle_strsadd(
         char ***                      strsp,
         const char *                  str )
{
   size_t     count;
   char **    strs;

   BindleDebugTrace();

   assert(strsp != NULL);
   assert(str   != NULL);

   count = bindle_strscount(*strsp);

   // increase size of array
   if ((strs = realloc(*strsp, (sizeof(char *)*(count+2)))) == NULL)
      return(ENOMEM);
   *strsp        = strs;

   // copy string
   if ((strs[count] = bindle_strdup(str)) == NULL)
      return(ENOMEM);

   // terminate array
   count++;
   strs[count] = NULL;

   return(BNDL_SUCCESS);
}


size_t
bindle_strscount(
         char * const *                strs )
{
   size_t count;
   BindleDebugTrace();
   if (!(strs))
      return(0);
   for(count = 0; ((strs != NULL)&&(strs[count] != NULL)); count++);
   return(count);
}


int
bindle_strsdup(
         char ***                      dstp,
         char * const *                src )
{
   size_t      len;
   size_t      size;
   size_t      pos;
   char **     dst;

   BindleDebugTrace();

   assert(dstp != NULL);
   assert(src   != NULL);

   len  = bindle_strscount(src);
   size = sizeof(char *) * (len+1);

   if ((dst = malloc(size)) == NULL)
      return(ENOMEM);

   for(pos = 0; (pos < len); pos++)
   {
      if ((dst[pos] = bindle_strdup(src[pos])) == NULL)
      {
         bindle_strsfree(dst);
         return(ENOMEM);
      };
   };
   dst[pos] = NULL;

   *dstp = dst;

   return(BNDL_SUCCESS);
}


void
bindle_strsfree(
         char **                       strs )
{
   int i;
   BindleDebugTrace();
   if (!(strs))
      return;
   for(i = 0; ((strs[i])); i++)
   {
      free(strs[i]);
      strs[i] = NULL;
   };
   free(strs);
   return;
}

/* end of source */
