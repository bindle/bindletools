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
#define _LIB_LIBBINDLE_LARRAY_C 1
#include "larray.h"


///////////////
//           //
//  Headers  //
//           //
///////////////
#pragma mark - Headers

#include <stdlib.h>
#include <string.h>
#include <assert.h>


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

ssize_t
bindle_badd(
         void **                       basep,
         size_t *                      nelp,
         size_t                        width,
         void *                        obj,
         unsigned                      opts,
         int  (*compar)(const void *, const void *),
         void (*freeobj)(void *),
         void * (*reallocbase)(void *, size_t) )
{
   ssize_t     idx;
   size_t      wouldbe;
   size_t      size;
   ssize_t     pos;
   char *      src;
   char *      dst;
   void *      ptr;

   BindleDebugTrace();

   assert(basep != NULL);
   assert(nelp  != NULL);
   assert(obj   != NULL);
   assert(width  > 0);

   // set default add option
   if ((opts & BNDL_ARRAY_MASK_ADD) == 0)
      opts |= BNDL_ARRAY_MASK_ADD & BNDL_ARRAY_DEFAULT;

   if (!(*nelp))
   {
      if ((reallocbase))
      {
         size = width * (*nelp + 1);
         if ((ptr = (*reallocbase)(*basep, size)) == NULL)
            return(-2);
         *basep = ptr;
      };
      (*nelp)++;
      memcpy(*basep, obj, width);
      return(0);
   };

   // search for existing object which matches
   if ((idx = bindle_bindex(obj, *basep, *nelp, width, opts, &wouldbe, compar)) != -1)
   {
      if ((opts & BNDL_ARRAY_MASK_ADD) == BNDL_ARRAY_INSERT)
         return(-1);
      if ((opts & BNDL_ARRAY_MASK_ADD) == BNDL_ARRAY_REPLACE)
      {
         dst = ((char *)*basep) + (width * (size_t)idx);
         if ((freeobj))
            (*freeobj)(dst);
         memcpy(dst, obj, width);
         return(idx);
      };
      if ((opts & BNDL_ARRAY_MASK_ADD) != BNDL_ARRAY_MERGE)
         return(-1);
   };

   // increases size of base
   if ((reallocbase))
   {
      size = width * (*nelp + 1);
      if ((ptr = (*reallocbase)(*basep, size)) == NULL)
         return(-2);
      *basep = ptr;
   };

   // shift list
   for(pos = (ssize_t)((*nelp)-1); (pos >= (ssize_t)wouldbe); pos--)
   {
      src = ((char *)*basep) + (width * (size_t)(pos+0));
      dst = ((char *)*basep) + (width * (size_t)(pos+1));
      memcpy(dst, src, width);
   };

   // save object
   dst = ((char *)*basep) + (width * wouldbe);
   memcpy(dst, obj, width);

   // increment nelp
   (*nelp)++;

   return((ssize_t)wouldbe);
}


void *
bindle_bsearch(
         const void *                  key,
         void *                        base,
         size_t                        nel,
         size_t                        width,
         unsigned                      opts,
         int (*compar)(const void *, const void *) )
{
   ssize_t     idx;
   BindleDebugTrace();
   assert((base != NULL) || (!(nel)) );
   assert(key   != NULL);
   assert(width  > 0);
   if ((idx = bindle_bindex(key, base, nel, width, opts, NULL, compar)) == -1)
      return(NULL);
   return(((char *)base) + (width * (size_t)idx));
}


ssize_t
bindle_bremove(
         const void *                  key,
         void *                        base,
         size_t *                      nelp,
         size_t                        width,
         unsigned                      opts,
         int (*compar)(const void *, const void *),
         void (*freeobj)(void *) )
{
   ssize_t     idx;
   char *      src;
   char *      dst;
   size_t      pos;

   BindleDebugTrace();

   assert(nelp  != NULL);
   assert((base != NULL) || (!(*nelp)) );
   assert(key   != NULL);
   assert(width  > 0);

   // search for matching object
   if ((idx = bindle_bindex(key, base, *nelp, width, opts, NULL, compar)) == -1)
      return(-1);

   // free object
   if ((freeobj))
   {
      dst = ((char *)base) + (width * (size_t)idx);
      (*freeobj)(dst);
   };

   // decrement nelp
   (*nelp)--;

   // shift list
   for(pos = ((size_t)idx); (pos < (*nelp)); pos ++)
   {
      src = ((char *)base) + (width * (size_t)(pos+1));
      dst = ((char *)base) + (width * (size_t)(pos+0));
      memcpy(dst, src, width);
   };

   return(0);
}


ssize_t
bindle_bindex(
         const void *                  key,
         const void *                  base,
         size_t                        nel,
         size_t                        width,
         unsigned                      opts,
         size_t *                      wouldbep,
         int (*compar)(const void *, const void *) )
{
   size_t         wouldbe;
   ssize_t        low;
   ssize_t        mid;
   ssize_t        high;
   int            rc;
   const char *   ptr;

   BindleDebugTrace();

   assert((base != NULL) || (!(nel)) );
   assert(key   != NULL);
   assert(width  > 0);
   assert(opts  != ((unsigned)(~0)));

   // set default dup behavior
   if ((opts & BNDL_ARRAY_MASK_DUPS) == 0)
      opts |= BNDL_ARRAY_DEFAULT & BNDL_ARRAY_MASK_DUPS;

   if (nel == 0)
   {
      if ((wouldbep))
         *wouldbep = 0;
      return(-1);
   };

   if (!(wouldbep))
      wouldbep = &wouldbe;

   low  = 0;
   high = (ssize_t)(nel - 1);
   mid  = (ssize_t)((nel - 1) / 2);

   while ( (mid >= low) && (mid <= high) && (high != low) )
   {
      ptr = ((const char *)base) + (width * (size_t)mid);
      if ((rc = (*compar)(ptr, key)) == 0)
      {
         switch(opts & BNDL_ARRAY_MASK_DUPS)
         {
            case BNDL_ARRAY_LASTDUP:
            if (low == mid)
            {
               *wouldbep = (size_t)(mid+1);
               if (mid < high)
               {
                  ptr = ((const char *)base) + (width * (size_t)(mid+1));
                  if ((*compar)(ptr, key) == 0)
                  {
                     *wouldbep = (size_t)(mid+2);
                     return(mid+1);
                  };
               };
               return(mid);
            };
            low = mid;
            break;

            case BNDL_ARRAY_FIRSTDUP:
            high = mid;
            break;

            case BNDL_ARRAY_ANYDUP:
            default:
            return(*((ssize_t *)wouldbep) = mid);
         };
      }
      else if (rc > 0)
         high = mid - 1;
      else
         low = mid + 1;
      mid = (high + low) / 2;
   };

   ptr = ((const char *)base) + (width * (size_t)mid);
   if ((rc = (*compar)(ptr, key)) == 0)
   {
      *wouldbep = (size_t)mid;
      if ((opts & BNDL_ARRAY_MASK_DUPS) == BNDL_ARRAY_LASTDUP)
         *wouldbep = (size_t)(mid+1);
      return(mid);
   };
   *wouldbep = (size_t)((rc > 0) ? mid : mid+1);

   return(-1);
}


void *
bindle_dequeue(
         void *                        base,
         size_t *                      nelp,
         size_t                        width )
{
   BindleDebugTrace();
   assert(nelp  != NULL);
   assert((base != NULL) || (!(*nelp)) );
   assert(width  > 0);
   if (!(*nelp))
      return(NULL);
   (*nelp)--;
   return(((char *)base) + (width * (*nelp)));
}


ssize_t
bindle_enqueue(
         void **                       basep,
         size_t *                      nelp,
         size_t                        width,
         void *                        obj,
         void * (*reallocbase)(void *, size_t) )
{
   size_t      size;
   size_t      pos;
   void *      ptr;
   char *      src;
   char *      dst;

   BindleDebugTrace();

   assert(basep != NULL);
   assert(nelp  != NULL);
   assert(width  > 0);
   assert(obj   != NULL);

   // increases size of base
   if ((reallocbase))
   {
      size = width * (*nelp + 1);
      if ((ptr = (*reallocbase)(*basep, size)) == NULL)
         return(-2);
      *basep = ptr;
   };

   // shift list
   for(pos = (*nelp); (pos > 0); pos--)
   {
      src = ((char *)*basep) + (width * (size_t)(pos-1));
      dst = ((char *)*basep) + (width * (size_t)(pos-0));
      memcpy(dst, src, width);
   };

   // save object
   memcpy(((char *)*basep), obj, width);

   // increment nel
   (*nelp)++;

   return(0);
}


void *
bindle_peek(
         void *                        base,
         size_t                        nel,
         size_t                        width )
{
   BindleDebugTrace();
   assert((base  != NULL) || (!(nel)) );
   assert(width   > 0);
   if (!(nel))
      return(NULL);
   return(((char *)base) + (width * (nel - 1)));
}


void *
bindle_pop(
         void *                        base,
         size_t *                      nelp,
         size_t                        width )
{
   BindleDebugTrace();
   assert(nelp  != NULL);
   assert((base != NULL) || (!(*nelp)) );
   assert(width  > 0);
   if (!(*nelp))
      return(NULL);
   (*nelp)--;
   return(((char *)base) + (width * (*nelp)));
}


ssize_t
bindle_push(
         void **                       basep,
         size_t *                      nelp,
         size_t                        width,
         void *                        obj,
         void * (*reallocbase)(void *, size_t) )
{
   size_t      size;
   void *      ptr;

   BindleDebugTrace();

   assert(basep != NULL);
   assert(nelp  != NULL);
   assert(width  > 0);
   assert(obj   != NULL);

   // increases size of base
   if ((reallocbase))
   {
      size = width * (*nelp + 1);
      if ((ptr = (*reallocbase)(*basep, size)) == NULL)
         return(-2);
      *basep = ptr;
   };

   // save object
   ptr = ((char *)*basep) + (width * (*nelp));
   memcpy(ptr, obj, width);

   // increment nel
   (*nelp)++;

   return((ssize_t)((*nelp) - 1));
}

/* end of source */
