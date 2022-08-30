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
#define _EXAMPLES_EXAMPLE_STACK_C 1


///////////////
//           //
//  Headers  //
//           //
///////////////
#pragma mark - Headers

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <bindle.h>


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#pragma mark - Definitions

#undef PROGRAM_NAME
#define PROGRAM_NAME "example-stack"


//////////////////
//              //
//  Data Types  //
//              //
//////////////////
#pragma mark - Data Types

struct wc
{
   uintptr_t      value;
   const char *   name;
};


/////////////////
//             //
//  Variables  //
//             //
/////////////////
#pragma mark - Variables

struct wc wcs[] =
{
   { 0xFFFFFF, "white"   }, { 0xC0C0C0, "silver"  },
   { 0x808080, "gray"    }, { 0x000000, "black"   },
   { 0xFF0000, "red"     }, { 0x800000, "maroon"  },
   { 0xFFFF00, "yellow"  }, { 0x808000, "olive"   },
   { 0x00FF00, "lime"    }, { 0x008000, "green"   },
   { 0x00FFFF, "aqua"    }, { 0x008080, "teal"    },
   { 0x0000FF, "blue"    }, { 0x000080, "navy"    },
   { 0xFF00FF, "fuchsia" }, { 0x800080, "purple"  }
};
#define nr_wcs (sizeof(wcs) / sizeof(wcs[0]))


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#pragma mark - Prototypes

int
main(
         void );


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#pragma mark - Functions

int
main(
         void )
{
   size_t         pos;
   size_t         nel;
   size_t         width;
   ssize_t        rc;
   struct wc **   list;
   struct wc *    obj;
   struct wc **   res;

   nel   = 0;
   list  = NULL;
   width = sizeof(struct wc *);

   for(pos = 0; (pos < nr_wcs); pos++)
   {
      obj = &wcs[pos];
      printf("pushing: #%06x %s\n", (unsigned)obj->value, obj->name);
      rc = bindle_push(&obj, (void **)&list, &nel, width, realloc);
      if (rc == -1)
      {
         fprintf(stderr, "error adding %s to stack\n", wcs[pos].name);
         free(list);
         return(1);
      };
   };

   if ((res = bindle_peek(list, nel, width)) != NULL)
      printf("peeked:  #%06x %s\n", (unsigned)(*res)->value, (*res)->name);
   if ((res = bindle_peek(list, nel, width)) != NULL)
      printf("peeked:  #%06x %s\n", (unsigned)(*res)->value, (*res)->name);

   while((res = bindle_pop(list, &nel, width)) != NULL)
      printf("popped:  #%06x %s\n", (unsigned)(*res)->value, (*res)->name);

   free(list);

   return(1);
}

/* end of source */

