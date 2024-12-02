/*
 *  Bindle Binaries Tools
 *  Copyright (C) 2024 David M. Syzdek <david@syzdek.net>.
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
#define _LIB_LIBBINDLE_LBERVAL_C 1
#include "lberval.h"


///////////////
//           //
//  Headers  //
//           //
///////////////
// MARK: - Headers

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <assert.h>


/////////////////
//             //
//  Functions  //
//             //
/////////////////
// MARK: - Functions

char *
bindle_berval_bv2str(
         const bindle_berval_t *       bv )
{
   char *   str;
   BindleDebugTrace();
   assert(bv != NULL);
   if ((str = malloc(bv->bv_len + 1)) == NULL)
      return(NULL);
   memcpy(str, bv->bv_val, bv->bv_len);
   str[bv->bv_len] = '\0';
   return(str);
}


int
bindle_berval_cmp(
         const bindle_berval_t *       bv1,
         const bindle_berval_t *       bv2 )
{
   size_t                     len;
   int                        rc;

   BindleDebugTrace();
   assert(bv1 != NULL);
   assert(bv2 != NULL);

   assert(bv1 != NULL);
   assert(bv2 != NULL);

   len = (bv1->bv_len < bv2->bv_len) ? bv1->bv_len : bv2->bv_len;
   if ((rc = memcmp(bv1->bv_val, bv2->bv_val, len)) != 0)
      return(rc);

   if (bv1->bv_len < bv2->bv_len)
      return(-1);
   if (bv1->bv_len > bv2->bv_len)
      return(1);

   return(0);
}


_BINDLE_F bindle_berval_t *
bindle_berval_dup(
         const bindle_berval_t *       bv )
{
   BindleDebugTrace();
   assert(bv != NULL);
   return(bindle_berval_init(bv->bv_val, bv->bv_len));
}


void
bindle_berval_free(
         bindle_berval_t *             bv )
{
   BindleDebugTrace();
   assert(bv != NULL);

   bv->bv_len = 0;

   if ((bv->bv_val))
      free(bv->bv_val);
   bv->bv_val = NULL;

   free(bv);

   return;
}


bindle_berval_t *
bindle_berval_init(
         const void *                  val,
         size_t                        len )
{
   size_t               alloc_size;
   bindle_berval_t *    bv;

   BindleDebugTrace();
   assert( ((!(len)) && (!(val))) || ((val)) );

   alloc_size = ((len)) ? len : 1;
   val        = ((len)) ? val : "";

   if ((bv = malloc(sizeof(bindle_berval_t))) == NULL)
      return(NULL);
   memset(bv, 0, sizeof(bindle_berval_t));

   bv->bv_len = len;

   if ((bv->bv_val = malloc(alloc_size)) == NULL)
   {
      bindle_berval_free(bv);
      return(NULL);
   };
   memcpy(bv->bv_val, val, alloc_size);

   return(bv);
}


bindle_berval_t *
bindle_berval_str2bv(
         const char *                  val )
{
   BindleDebugTrace();
   assert(val != NULL);
   return(bindle_berval_init(val, (strlen(val) + 1)));
}


int
bindle_bvarray_add(
         bindle_berval_t ***           bvap,
         const bindle_berval_t *       bv )
{
   size_t               len;
   size_t               size;
   bindle_berval_t **   bva;
   bindle_berval_t *    dup;

   BindleDebugTrace();
   assert( bvap != NULL );
   assert( bv != NULL );

   if ((dup = bindle_berval_dup(bv)) == NULL)
      return(-1);

   if (!(*bvap))
   {
      if (bindle_bvarray_init(bvap, 1) == -1)
      {
         bindle_berval_free(dup);
         return(-1);
      };
      (*bvap)[0] = dup;
      (*bvap)[1] = NULL;
      return(0);
   };

   len   = bindle_bvarray_len(*bvap);
   size  = (len+2) * sizeof(bindle_berval_t *);
   if ((bva = realloc(*bvap, size)) == NULL)
   {
      bindle_berval_free(dup);
      return(-1);
   };
   bva[len+0] = dup;
   bva[len+1] = NULL;
   *bvap       = bva;

   return(0);
}


void
bindle_bvarray_free(
         bindle_berval_t **            bva )
{
   size_t pos;
   BindleDebugTrace();
   assert( bva != NULL );
   for(pos = 0; ((bva[pos])); pos++)
      bindle_berval_free(bva[pos]);
   free(bva);
   return;
}


_BINDLE_F int
bindle_bvarray_init(
         bindle_berval_t ***           bvap,
         size_t                        n )
{
   bindle_berval_t **   bva;
   size_t               size;
   BindleDebugTrace();
   assert( bvap != NULL );
   n++;
   size = sizeof(bindle_berval_t *) * n;
   if ((bva = malloc(size)) == NULL)
      return(-1);
   bva[0] = NULL;
   *bvap = bva;
   return(0);
}


_BINDLE_F size_t
bindle_bvarray_len(
         const bindle_berval_t **      bva )
{
   size_t   pos;
   BindleDebugTrace();
   assert( bva != NULL );
   for(pos = 0; ((bva[pos])); pos++);
   return(pos);
}

/* end of source */
