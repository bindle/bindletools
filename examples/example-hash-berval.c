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
#define _EXAMPLES_EXAMPLE_HASH_C 1


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
#include <bindle.h>
#include <assert.h>


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
// MARK: - Definitions

#undef PROGRAM_NAME
#define PROGRAM_NAME "example-hash-berval"


/////////////////
//             //
//  Variables  //
//             //
/////////////////
// MARK: - Variables

struct
{  const char *   name;
   const char *   party;
} test_data[] =
{  {  "George Washington",       "Unaffiliated" },
   {  "John Adams",              "Federalist" },
   {  "Thomas Jefferson",        "Democratic-Republican" },
   {  "James Madison",           "Democratic-Republican" },
   {  "James Monroe",            "Democratic-Republican" },
   {  "John Quincy Adams",       "Democratic-Republican / National Republican" },
   {  "Andrew Jackson",          "Democratic" },
   {  "Martin Van Buren",        "Democratic" },
   {  "William Henry Harrison",  "Whig" },
   {  "John Tyler",              "Whig" },
   {  "James K. Polk",           "Democratic" },
   {  "Zachary Taylor",          "Whig" },
   {  "Millard Fillmore",        "Whig" },
   {  "Franklin Pierce",         "Democratic" },
   {  "James Buchanan",          "Democratic" },
   {  "Abraham Lincoln",         "Republican / National Union" },
   {  NULL,                      NULL }
};


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
// MARK: - Prototypes

int
main(
         void );


/////////////////
//             //
//  Functions  //
//             //
/////////////////
// MARK: - Functions

int
main(
         void )
{
   int                        opts;
   size_t                     pos;
   size_t                     namelen;
   size_t                     partylen;
   bindle_hash_t *            bh;
   const char *               name;
   const char *               party;
   const void *               val;
   size_t                     vallen;
   bindle_berval_t *          key;
   bindle_berval_t **         keys;

   opts = BNDL_HKEY_STRING | BNDL_HVAL_STRING;
   if (bindle_hash_initialize(&bh, opts) == -1)
   {  perror("bindle_hash_initialize()");
      return(1);
   };

   // populate hash
   for(pos = 0; ((test_data[pos].name)); pos++)
   {  name     = test_data[pos].name;
      namelen  = strlen(name)+1;
      party    = test_data[pos].party;
      partylen = strlen(party)+1;
      if (bindle_hash_set(bh, name, namelen, party, partylen))
      {  perror("bindle_hash_set()");
         bindle_hash_free(bh);
         return(1);
      };
   };

   // retrieve list of keys
   if (bindle_hash_keys(bh, &keys) == -1)
   {  perror("bindle_hash_keys()");
      bindle_hash_free(bh);
      return(1);
   };

   // retrieve value for each key
   for(pos = 0; ((keys[pos])); pos++)
   {  key = keys[pos];
      if (bindle_hash_get(bh, key->bv_val, key->bv_len, &val, &vallen) == -1)
      {  perror("bindle_hash_get()");
         bindle_bvarray_free(keys);
         bindle_hash_free(bh);
         return(1);
      };
      printf("%-25s (%s)\n", (char *)key->bv_val, (const char *)val);
   };

   // delete elements from hash
   for(pos = 0; ((keys[pos])); pos++)
   {  key = keys[pos];
      if (bindle_hash_del(bh, key->bv_val, key->bv_len) == -1)
      {  perror("bindle_hash_del()");
         bindle_bvarray_free(keys);
         bindle_hash_free(bh);
         return(1);
      };
   };

   // frees key array
   bindle_bvarray_free(keys);
   keys = NULL;

   bindle_hash_free(bh);
   bh = NULL;

   return(0);
}


/* end of source */

