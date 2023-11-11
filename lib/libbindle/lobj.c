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
#define _LIB_LIBBINDLE_LOBJ_C 1
#include "lobj.h"


///////////////
//           //
//  Headers  //
//           //
///////////////
// MARK: - Headers

#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <syslog.h>
#include <stdatomic.h>
#include <assert.h>


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
// MARK: - Definitions

// Magic number used to determine if memory passed to bindle_free() is a
// "bindle_obj_t" or raw virtual memory.  The magic number purposely starts
// and ends with a byte equal to zero so that it cannot match a string.
#define BNDL_MAGIC                     ((const uint8_t *)"\0Bindle\0")


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
// MARK: - Prototypes

static int
bindle_verify_is_obj(
         bindle_obj_t *                obj );


/////////////////
//             //
//  Functions  //
//             //
/////////////////
// MARK: - Functions

void *
bindle_alloc(
         size_t                        size,
         void (*free_func)(void * ptr) )
{
   bindle_obj_t * obj;
   BindleDebugTrace();
   assert(size >= BNDL_OBJ_SIZE);
   if ((obj = malloc(size)) == NULL)
      return(NULL);
   memset(obj, 0, size);
   memcpy(obj->magic, BNDL_MAGIC, 8);
   atomic_init(&obj->ref_count, 0);
   obj->free_func = ((free_func)) ? free_func : &free;
   return(obj);
}


void
bindle_free(
         void *                        ptr )
{
   BindleDebugTrace();
   if (!(ptr))
      return;
   if (bindle_verify_is_obj(ptr) == BNDL_NO)
   {
      free(ptr);
      return;
   };
   bindle_release(ptr);
   return;
}


void *
bindle_realloc(
         void *                        ptr,
         size_t                        size )
{
   BindleDebugTrace();
   assert(size >= BNDL_OBJ_SIZE);
   return(realloc(ptr, size));
}


void
bindle_release(
         bindle_obj_t *                obj )
{
   BindleDebugTrace();
   assert(obj != NULL);
   assert(bindle_verify_is_obj(obj) == BNDL_YES);
   if (atomic_fetch_sub(&obj->ref_count, 1) > 1)
      return;
   obj->free_func(obj);
   return;
}


void *
bindle_retain(
         bindle_obj_t *                obj )
{
   BindleDebugTrace();
   if (obj == NULL)
      return(NULL);
   assert(bindle_verify_is_obj(obj) == BNDL_YES);
   atomic_fetch_add(&obj->ref_count, 1);
   return(obj);
}


int
bindle_retain_count(
         bindle_obj_t *                obj )
{
   BindleDebugTrace();
   if (obj == NULL)
      return(0);
   assert(bindle_verify_is_obj(obj) == BNDL_YES);
   return((int)atomic_fetch_add(&obj->ref_count, 0));
}


int
bindle_verify_is_obj(
         bindle_obj_t *                obj )
{
   size_t   pos;
   BindleDebugTrace();
   if (!(obj))
      return(BNDL_NO);
   for(pos = 0; (pos < 8); pos++)
      if (obj->magic[pos] != BNDL_MAGIC[pos])
         return(BNDL_NO);
   return(BNDL_YES);
}


/* end of source */
