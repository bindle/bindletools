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
#define _EXAMPLES_EXAMPLE_BSEARCH_C 1


///////////////
//           //
//  Headers  //
//           //
///////////////
#pragma mark - Headers

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>
#include <bindle.h>


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#pragma mark - Definitions

#undef PROGRAM_NAME
#define PROGRAM_NAME "example-bsearch"


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
   { 0xFF00FF, "fuchsia" }, { 0x800080, "purple"  },
   { 0x321123, "test"    }, { 0x123321, "test"    }
};
#define nr_wcs (sizeof(wcs) / sizeof(wcs[0]))


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#pragma mark - Prototypes

static int
compkey(
         const void *                  keyp,
         const void *                  objp );


static int
compobj(
         const void *                  c1,
         const void *                  c2 );


int
main(
         int                           argc,
         char *                        argv[] );


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#pragma mark - Functions

int
compobj(
         const void *                  c1,
         const void *                  c2 )
{
   const struct wc * wc1 = *((const struct wc * const *)c1);
   const struct wc * wc2 = *((const struct wc * const *)c2);
   return(strcasecmp(wc1->name, wc2->name));
}


int
compkey(
         const void *                  keyp,
         const void *                  objp )
{
   const char *      key = (const char *)keyp;
   const struct wc * obj = *((const struct wc * const *)objp);
   return(strcasecmp(key, obj->name));
}


int
main(
         int                           argc,
         char *                        argv[] )
{
   int            i;
   unsigned       opts;
   size_t         pos;
   size_t         nel;
   size_t         width;
   size_t         wouldbe;
   ssize_t        rc;
   ssize_t        idx;
   struct wc **   list;
   struct wc *    obj;
   struct wc **   res;

   nel   = 0;
   list  = NULL;
   opts  = BNDL_BMERGE | BNDL_BDUPANY;
   width = sizeof(struct wc *);

   for(pos = 0; (pos < nr_wcs); pos++)
   {
      obj = &wcs[pos];
      rc = bindle_badd(&obj, (void **)&list, &nel, width, opts, compobj, NULL, realloc);
      if (rc == -1)
      {
         fprintf(stderr, "error adding %s to list\n", wcs[pos].name);
         free(list);
         return(1);
      };
   };

   for (i = 1; i < argc; i++)
   {
      idx = bindle_bindex(argv[i], list, nel, width, opts, &wouldbe, compkey);
      printf("bindle_bindex():  '%s': ", argv[i]);
      if (idx == -1)
         printf("list position would be %zu\n", wouldbe);
      else
         printf("list position  %zi\n", idx);

      res = bindle_bsearch(argv[i], list, nel, width, opts, compkey);
      printf("bindle_bsearch(): '%s': ", argv[i]);
      if (res == NULL)
         printf("unknown color\n");
      else
         printf("color value #%06x\n", (unsigned)(*res)->value);

      res = bsearch(argv[i], list, nel, width, compkey);
      printf("bsearch():        '%s': ", argv[i]);
      if (res == NULL)
         printf("unknown color\n");
      else
         printf("color value #%06x\n", (unsigned)(*res)->value);
   };

   for(pos = 0; (pos < nr_wcs); pos++)
   {
      res = bindle_bremove(wcs[pos].name, list, &nel, width, opts, compkey);
      if (res == NULL)
      {
         fprintf(stderr, "error removing %s from list\n", wcs[pos].name);
         free(list);
         return(1);
      };
   };

   free(list);

   return(1);
}

/* end of source */

