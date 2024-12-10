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
#define _EXAMPLES_EXAMPLE_HASH_PTR_C 1


///////////////
//           //
//  Headers  //
//           //
///////////////
// MARK: - Headers

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>
#include <bindle.h>
#include <assert.h>

#include "../lib/libbindle/lhash.h"


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
// MARK: - Definitions

#undef PROGRAM_NAME
#define PROGRAM_NAME "example-hash-ptr"


//////////////////
//              //
//  Data Types  //
//              //
//////////////////
// MARK: - Data Types

typedef struct us_term
{
   unsigned          start;
   unsigned          end;
} us_term_t;


typedef struct us_president
{
   char *            givenname;
   char *            surname;
   char *            party;
   int               intpad;
   int               num;
   struct us_term    term;
} us_president_t;


struct data
{
   const char *      givenname;
   const char *      surname;
   const char *      party;
   us_term_t         term;
};


/////////////////
//             //
//  Variables  //
//             //
/////////////////
// MARK: - Variables

struct data test_data[] =
{
   {  .givenname  = "George",
      .surname    = "Washington",
      .party      = "Unaffiliated",
      .term       = { 17890430, 17970304 },
   },
   {  .givenname  = "John",
      .surname    = "Adams",
      .party      = "Federalist",
      .term       = { 17970304, 18010304 }
   },
   {  .givenname  = "Thomas",
      .surname    = "Jefferson",
      .party      = "Democratic-Republican",
      .term       = { 18010304, 18090304 }
   },
   {  .givenname  = "James",
      .surname    = "Madison",
      .party      = "Democratic-Republican",
      .term       = { 18090304, 18170304 }
   },
   {  .givenname  = "James",
      .surname    = "Monroe",
      .party      = "Democratic-Republican",
      .term       = { 18170304, 18250304 }
   },
   {  .givenname  = "John Quincy",
      .surname    = "Adams",
      .party      = "Democratic-Republican / National Republican",
      .term       = { 18250304, 18290304 }
   },
   {  .givenname  = "Andrew",
      .surname    = "Jackson",
      .party      = "Democratic",
      .term       = { 18290304, 18370304 }
   },
   {  .givenname  = "Martin",
      .surname    = "Van Buren",
      .party      = "Democratic",
      .term       = { 18370304, 18410304 }
   },
   {  .givenname  = "William Henry",
      .surname    = "Harrison",
      .party      = "Whig",
      .term       = { 18410304, 18410404 }
   },
   {  .givenname  = "John",
      .surname    = "Tyler",
      .party      = "Whig",
      .term       = { 18410404, 18450304 }
   },
   {  .givenname  = "James K.",
      .surname    = "Polk",
      .party      = "Democratic",
      .term       = { 18450304, 18490304 }
   },
   {  .givenname  = "Zachary",
      .surname    = "Taylor",
      .party      = "Whig",
      .term       = { 18490304, 18500709 }
   },
   {  .givenname  = "Millard",
      .surname    = "Fillmore",
      .party      = "Whig",
      .term       = { 18500709, 18530304 }
   },
   {  .givenname  = "Franklin",
      .surname    = "Pierce",
      .party      = "Democratic",
      .term       = { 18530304, 18570304 }
   },
   {  .givenname  = "James",
      .surname    = "Buchanan",
      .party      = "Democratic",
      .term       = { 18570304, 18610304 }
   },
   {  .givenname  = "Abraham",
      .surname    = "Lincoln",
      .party      = "Republican / National Union",
      .term       = { 18610304, 18650415 }
   },
   {  .givenname  = "Andrew",
      .surname    = "Johnson",
      .party      = "National Union / Democratic",
      .term       = { 18650415, 18690304 }
   },
   {  .givenname  = "Ulysses S.",
      .surname    = "Grant",
      .party      = "Republican",
      .term       = { 18690304, 18770304 }
   },
   {  .givenname  = "Rutherford B.",
      .surname    = "Hayes",
      .party      = "Republican",
      .term       = { 18770304, 18810304 }
   },
   {  .givenname  = "James A.",
      .surname    = "Garfield",
      .party      = "Republican",
      .term       = { 18810304, 18810919 }
   },
   {  .givenname  = "Chester A.",
      .surname    = "Arthur",
      .party      = "Republican",
      .term       = { 18810919, 18850304 }
   },
   {  .givenname  = "Grover",
      .surname    = "Cleveland",
      .party      = "Democratic",
      .term       = { 18850304, 18890304 }
   },
   {  .givenname  = "Benjamin",
      .surname    = "Harrison",
      .party      = "Republican",
      .term       = { 18890304, 18930304 }
   },
   {  .givenname  = "Grover",
      .surname    = "Cleveland",
      .party      = "Democratic",
      .term       = { 18930304, 18970304 }
   },
   {  .givenname  = "William",
      .surname    = "McKinley",
      .party      = "Republican",
      .term       = { 18970304, 19010914 }
   },
   {  .givenname  = "Theodore",
      .surname    = "Roosevelt",
      .party      = "Republican",
      .term       = { 19010914, 19090304 }
   },
   {  .givenname  = "William Howard",
      .surname    = "Taft",
      .party      = "Republican",
      .term       = { 19090304, 19130304 }
   },
   {  .givenname  = "Woodrow",
      .surname    = "Wolson",
      .party      = "Democratic",
      .term       = { 19130304, 19210304 }
   },
   {  .givenname  = "Warren G.",
      .surname    = "Harding",
      .party      = "Republican",
      .term       = { 19210304, 19230802 }
   },
   {  .givenname  = "Calvin",
      .surname    = "Coolidge",
      .party      = "Republican",
      .term       = { 19230802, 19290304 }
   },
   {  .givenname  = "Herbert",
      .surname    = "Hoover",
      .party      = "Republican",
      .term       = { 19290304, 19330304 }
   },
   {  .givenname  = "Franklin D.",
      .surname    = "Roosevelt",
      .party      = "Democratic",
      .term       = { 19330304, 19450412 }
   },
   {  .givenname  = "Harry S.",
      .surname    = "Truman",
      .party      = "Democratic",
      .term       = { 19450412, 19530120 }
   },
   {  .givenname  = "Dwight D.",
      .surname    = "Eisenhower",
      .party      = "Republican",
      .term       = { 19530120, 19610120 }
   },
   {  .givenname  = "John F.",
      .surname    = "Kennedy",
      .party      = "Democratic",
      .term       = { 19610120, 19631122 }
   },
   {  .givenname  = "Lyndon B.",
      .surname    = "Johnson",
      .party      = "Democratic",
      .term       = { 19631122, 19690120 }
   },
   {  .givenname  = "Richard",
      .surname    = "Nixon",
      .party      = "Republican",
      .term       = { 19690120, 19740809 }
   },
   {  .givenname  = "Gerald",
      .surname    = "Ford",
      .party      = "Republican",
      .term       = { 19740809, 19770120 }
   },
   {  .givenname  = "Jimmy",
      .surname    = "Carter",
      .party      = "Democratic",
      .term       = { 19770120, 19810120 }
   },
   {  .givenname  = "Ronald",
      .surname    = "Reagan",
      .party      = "Republican",
      .term       = { 19810120, 19890120 }
   },
   {  .givenname  = "George H. W.",
      .surname    = "Bush",
      .party      = "Republican",
      .term       = { 19890120, 19930120 }
   },
   {  .givenname  = "Bill",
      .surname    = "Clinton",
      .party      = "Democratic",
      .term       = { 19930120, 20010120 }
   },
   {  .givenname  = "George W.",
      .surname    = "Bush",
      .party      = "Republican",
      .term       = { 20010120, 20090120 }
   },
   {  .givenname  = "Barack",
      .surname    = "Obama",
      .party      = "Democratic",
      .term       = { 20090120, 20170120 }
   },
   {  .givenname  = "Donald",
      .surname    = "Trump",
      .party      = "Republican",
      .term       = { 20170120, 20210120 }
   },
   {  .givenname  = "Joe",
      .surname    = "Biden",
      .party      = "Democratic",
      .term       = { 20210120, 20250120 }
   },
   {  .givenname  = "Donald",
      .surname    = "Trump",
      .party      = "Republican",
      .term       = { 20250120, 20290120 }
   },
   {  .givenname  = NULL,
      .surname    = NULL,
      .term       = { 0, 0 }
   }

};


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
// MARK: - Prototypes

int
main(
         int                           argc,
         char *                        argv[] );


extern int
my_pres_cmp(
         const void *                  v1,
         const void *                  v2 );


static void
my_pres_free(
        us_president_t *               pres );


static void
my_pres_print(
        const us_president_t *         pres );


/////////////////
//             //
//  Functions  //
//             //
/////////////////
// MARK: - Functions

int
main(
         int                           argc,
         char *                        argv[] )
{
   size_t                     pos;
   const char *               prog_name;
   bindle_hash_t *            bh;
   us_president_t *           pres;
   const us_president_t *     res;
   const void *               ptr;
   us_term_t                  term;
   unsigned                   query;
   char *                     endptr;
   bindle_berval_t **         bvarray;

   prog_name = strrchr(argv[0], '/');
   prog_name = ((prog_name)) ? &prog_name[1] : argv[0];

   if (argc != 2)
   {
      fprintf(stderr, "Usage: %s YYYY[MM[DD]]\n", prog_name);
      return(1);
   };
   query = 0;
   if ((strcasecmp("all", argv[1])))
   {
      query = (unsigned)strtoul(argv[1], &endptr, 10);
      if (endptr[0] != '\0')
      {
         fprintf(stderr, "Usage: %s YYYY[MM[DD]]\n", prog_name);
         return(1);
      };
      if (query < 10000)
         query *= 10000;
      if (query < 1000000)
         query *= 100;
   };

   if (bindle_hash_initialize(&bh, BNDL_HKEY_BINARY|BNDL_HVAL_PTR) == -1)
   {
      perror("bindle_hash_initialize()");
      return(1);
   };

   bindle_hash_func_cmp(bh, my_pres_cmp);
   bindle_hash_func_ptr_free(bh, (void(*)(void *))my_pres_free);

   for(pos = 0; ((test_data[pos].givenname)); pos++)
   {
      if ((pres = malloc(sizeof(us_president_t))) == NULL)
      {
         perror("malloc()");
         bindle_hash_free(bh);
         return(1);
      };
      if ((pres->givenname = bindle_strdup(test_data[pos].givenname)) == NULL)
      {
         perror("bindle_strdup()");
         my_pres_free(pres);
         bindle_hash_free(bh);
         return(1);
      };
      if ((pres->surname = bindle_strdup(test_data[pos].surname)) == NULL)
      {
         perror("bindle_strdup()");
         my_pres_free(pres);
         bindle_hash_free(bh);
         return(1);
      };
      if ((pres->party = bindle_strdup(test_data[pos].party)) == NULL)
      {
         perror("bindle_strdup()");
         my_pres_free(pres);
         bindle_hash_free(bh);
         return(1);
      };
      pres->num         = ((int)pos) + 1;
      pres->term.start  = test_data[pos].term.start;
      pres->term.end    = test_data[pos].term.end;
      term.start        = test_data[pos].term.start;
      term.end          = test_data[pos].term.end;

      if (bindle_hash_set(bh, &term, sizeof(us_term_t), &pres, sizeof(us_president_t *)) == -1)
      {
         perror("bindle_hash_set()");
         bindle_hash_free(bh);
         return(1);
      };
   };

   if (query == 0)
   {
      if (bindle_hash_keys(bh, &bvarray) == -1)
      {
         perror("bindle_hash_keys()");
         bindle_hash_free(bh);
         return(1);
      };
      for(pos = 0; ((bvarray[pos])); pos++)
      {
         term.start  = ((us_term_t *)bvarray[pos]->bv_val)->start;
         term.end    = ((us_term_t *)bvarray[pos]->bv_val)->start;
         if (bindle_hash_get(bh, &term, sizeof(us_term_t), (const void **)&ptr, NULL) == -1)
         {
            perror("bindle_hash_get()");
            bindle_hash_free(bh);
            return(1);
         };
         res = *((const us_president_t * const *)ptr);
         my_pres_print(res);
      };
   };

   if (query > 0)
   {
      term.start  = query;
      term.end    = query;
      if (bindle_hash_get(bh, (void *)&term, sizeof(us_term_t), (const void **)&ptr, NULL) == -1)
      {
         perror("bindle_hash_get()");
         bindle_hash_free(bh);
         return(1);
      };
      res = *((const us_president_t * const *)ptr);
      my_pres_print(res);
   };

   bindle_hash_free(bh);
   bh = NULL;

   return(0);
}


int
my_pres_cmp(
         const void *                  v1,
         const void *                  v2 )
{
   const us_term_t *    t1;
   const us_term_t *    t2;

   assert( v1 != NULL );
   assert( v2 != NULL );
   assert( *((us_term_t * const *)v1) != NULL);
   assert( *((us_term_t * const *)v2) != NULL);

   t1 = bindle_hash_cmp_val(v1);
   t2 = bindle_hash_cmp_val(v2);

   if ( (t1->start <= t2->start) && (t2->start < t1->end) )
      if ( (t1->start <= t2->end) && (t2->end < t1->end) )
         return(0);
   if ( (t2->start <= t1->start) && (t1->start < t2->end) )
      if ( (t2->start <= t1->end) && (t1->end < t2->end) )
         return(0);

   if (t1->start < t2->start)
      return(-1);

   return(1);
}


void
my_pres_free(
        us_president_t *               pres )
{
   assert(pres != NULL);

   if ((pres->givenname))
      free(pres->givenname);
   if ((pres->surname))
      free(pres->surname);
   if ((pres->party))
      free(pres->party);
   memset(pres, 0, sizeof(us_president_t));

   free(pres);

   return;
}


void
my_pres_print(
        const us_president_t *         pres )
{
   char name[128];
   snprintf(name, sizeof(name), "%s %s", pres->givenname, pres->surname);
   name[sizeof(name)-1] = '\0';
   printf(
      "%3i. %-25s (term: %04i-%02i-%02i - %04i-%02i-%02i)\n",
      pres->num,
      name,
      (pres->term.start / 10000),
      ((pres->term.start % 10000) / 100),
      (pres->term.start % 100),
      (pres->term.end / 10000),
      ((pres->term.end % 10000) / 100),
      (pres->term.end % 100)
   );
   return;
}


/* end of source */

