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
#define _LIB_LIBBINDLE_LMAP_C 1
#include "lmap.h"


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

int
bindle_map_lookup(
         const bindle_map_t *          map,
         const char *                  name,
         uintptr_t                     value,
         const bindle_map_t **         mapp )
{
   BindleDebugTrace();
   assert(map  != NULL);
   assert(mapp != NULL);
   if (!(name))
      bindle_map_lookup_name(map, name, mapp);
   else
      bindle_map_lookup_value(map, value, mapp);
   return( ((*mapp)) ? BNDL_SUCCESS : EINVAL );
}


uintptr_t
bindle_map_lookup_name(
         const bindle_map_t *          map,
         const char *                  name,
         const bindle_map_t **         mapp )
{
   size_t  pos;
   BindleDebugTrace();
   assert(map  != NULL);
   assert(name != NULL);
   for(pos = 0; ((map[pos].map_name)); pos++)
   {
      if (!(strcasecmp(map[pos].map_name, name)))
      {
         if ((mapp))
            *mapp = &map[pos];
         return(map[pos].map_value);
      };
   };
   if ((mapp))
      *mapp = NULL;
   return(0);
}


const char *
bindle_map_lookup_value(
         const bindle_map_t *          map,
         uintptr_t                     value,
         const bindle_map_t **         mapp )
{
   size_t  pos;
   BindleDebugTrace();
   assert(map  != NULL);
   for(pos = 0; ((map[pos].map_name)); pos++)
   {
      if (map[pos].map_value == value)
      {
         if ((mapp))
            *mapp = &map[pos];
         return(map[pos].map_name);
      };
   };
   if ((mapp))
      *mapp = NULL;
   return(NULL);
}


/* end of source */
