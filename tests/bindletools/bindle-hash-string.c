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
#define _TESTS_BINDLE_HASH_STRING_C 1


///////////////
//           //
//  Headers  //
//           //
///////////////
// MARK: - Headers

#include "bindle-tests.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <getopt.h>
#include <assert.h>
#include <errno.h>
#include <inttypes.h>

#include "../lib/libbindle/lhash.h"


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
// MARK: - Definitions

#undef PROGRAM_NAME
#define PROGRAM_NAME "bindle-hash-string"


//////////////////
//              //
//  Data Types  //
//              //
//////////////////
// MARK: - Data Types

typedef struct _mydata MyData;

struct _mydata
{
   const char *      key;
   const char *      value;
   int               status;
   int               dup;
};


/////////////////
//             //
//  Variables  //
//             //
/////////////////
// MARK: - Variables

static const MyData test_data[] =
{
   {
      .key        = "test981",
      .value      = "smile direct arm period however",
      .dup        = 0,
      .status     = 0,
   },
   {
      .key        = "test111",
      .value      = "gas all supply scene type",
      .dup        = 0,
      .status     = 0,
   },
   {
      .key        = "test000",
      .value      = "normal eye very power temperature",
      .dup        = 0,
      .status     = 0,
   },
   {
      .key        = "test222",
      .value      = "directly prepare individual fiscal best",
      .dup        = 0,
      .status     = 0,
   },
   {
      .key        = "dup01",
      .value      = "foreign latter same could maintain",
      .dup        = 1,
      .status     = 0,
   },
   {
      .key        = "test22",
      .value      = "individual manager compare measure situation",
      .dup        = 0,
      .status     = 0,
   },
   {
      .key        = "test",
      .value      = "win control until meet face",
      .dup        = 0,
      .status     = 0,
   },
   {
      .key        = "dup01",
      .value      = "patient enjoy conference especially different",
      .dup        = 1,
      .status     = 0,
   },
   {
      .key        = "test0000",
      .value      = "lose feed page husband law",
      .dup        = 0,
      .status     = 0,
   },
   {
      .key        = "DUP01",
      .value      = "tax image before figure prevent",
      .dup        = 1,
      .status     = 0,
   },
   {
      .key        = "Test1010101",
      .value      = "Recommended password: certain million game four press",
      .dup        = 0,
      .status     = 0,
   },
   {
      .key        = "Dup01",
      .value      = "Recommended password: maybe subject labor often accept",
      .dup        = 1,
      .status     = 0,
   },
   {
      .key        = "TestAgain",
      .value      = "Recommended password: body best nor job talk",
      .dup        = 0,
      .status     = 0,
   },
   {
      .key        = NULL,
      .value      = NULL,
      .dup        = 0,
      .status     = 0,
   },
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


/////////////////
//             //
//  Functions  //
//             //
/////////////////
// MARK: - Functions

int main( int argc, char * argv[] )
{
   int                  opt;
   int                  c;
   int                  opt_index;
   size_t               pos;
   size_t               hash_size;
   size_t               val_len;
   unsigned             opts;
   bindle_hash_t *      bh;
   const char *         key;
   const char *         val;
   const char *         res;
   size_t               res_len;
   char **              keys;
   size_t               x;

   // getopt options
   static char          short_opt[] = "dhVvq";
   static struct option long_opt[] =
   {
      {"debug",            no_argument,       NULL, 'd' },
      {"help",             no_argument,       NULL, 'h' },
      {"quiet",            no_argument,       NULL, 'q' },
      {"silent",           no_argument,       NULL, 'q' },
      {"version",          no_argument,       NULL, 'V' },
      {"verbose",          no_argument,       NULL, 'v' },
      { NULL, 0, NULL, 0 }
   };

   bindle_tests_initialize(PROGRAM_NAME);

   opts = 0;

   while((c = getopt_long(argc, argv, short_opt, long_opt, &opt_index)) != -1)
   {
      switch(c)
      {
         case -1:       /* no more arguments */
         case 0:        /* long options toggles */
         break;

         case 'd':
         opt = BNDL_DEBUG_ANY;
         bindle_set_option(NULL, BNDL_OPT_DEBUG_LEVEL,  &opt);
         break;

         case 'h':
         printf("Usage: %s [OPTIONS]\n", PROGRAM_NAME);
         printf("OPTIONS:\n");
         printf("  -d, --debug               print debug messages\n");
         printf("  -h, --help                print this help and exit\n");
         printf("  -q, --quiet, --silent     do not print messages\n");
         printf("  -V, --version             print version number and exit\n");
         printf("  -v, --verbose             print verbose messages\n");
         printf("\n");
         return(0);

         case 'q':
         opts |=  BNDLTEST_OPT_QUIET;
         opts &= ~BNDLTEST_OPT_VERBOSE;
         break;

         case 'V':
         bindle_tests_version();
         return(0);

         case 'v':
         opts |=  BNDLTEST_OPT_VERBOSE;
         opts &= ~BNDLTEST_OPT_QUIET;
         break;

         case '?':
         fprintf(stderr, "Try `%s --help' for more information.\n", PROGRAM_NAME);
         return(1);

         default:
         fprintf(stderr, "%s: unrecognized option `--%c'\n", PROGRAM_NAME, c);
         fprintf(stderr, "Try `%s --help' for more information.\n", PROGRAM_NAME);
         return(1);
      };
   };

   if (optind != argc)
   {
         fprintf(stderr, "%s: unknown argument `--%s'\n", PROGRAM_NAME, argv[optind]);
         fprintf(stderr, "Try `%s --help' for more information.\n", PROGRAM_NAME);
         return(1);
   };

   // initialize test data
   bindle_tests_verbose(opts, "initializing test data ...");
   if (bindle_hash_initialize(&bh, (BNDL_HASH_REPLACE|BNDL_HASH_CASE|BNDL_HKEY_STRING|BNDL_HVAL_STRING)) == -1)
   {
      bindle_tests_error(opts, NULL, "bindle_hash_initialize(): %s", strerror(errno));
      return(1);
   };
   hash_size = bindle_hash_len(bh);

   // add test data
   for(pos = 0; ((test_data[pos].key)); pos++)
   {
      key      = test_data[pos].key;
      val      = test_data[pos].value;
      val_len  = strlen(val) + 1;
      bindle_tests_verbose(opts, "   %zu. adding via bindle_hash_set: key: %-10s val: %s", hash_size, key, val);
      if (bindle_hash_sset(bh, key, val, val_len) == -1)
      {
         bindle_tests_error(opts, NULL, "bindle_hash_set(): %s: %s", key, strerror(errno));
         bindle_hash_free(bh);
         return(1);
      };
      if (bindle_hash_sget(bh, key, (const void **)&res, &res_len) == -1)
      {
         bindle_tests_error(opts, NULL, "bindle_hash_get(): %s: %s", key, strerror(errno));
         bindle_hash_free(bh);
         return(1);
      };
      if (res_len != val_len)
      {
         bindle_tests_error(opts, NULL, "value length of key does not match value length set");
         bindle_hash_free(bh);
         return(1);
      };
      if ((memcmp(res, val, val_len)))
      {
         bindle_tests_error(opts, NULL, "value of key does not match value set");
         bindle_hash_free(bh);
         return(1);
      };
      hash_size = bindle_hash_len(bh);
   };

   // retrieves hash keys
   if (bindle_hash_skeys(bh, &keys) == -1)
   {
      bindle_tests_error(opts, NULL, "bindle_hash_skeys(): %s", strerror(errno));
      bindle_hash_free(bh);
      return(1);
   };
   bindle_tests_verbose(opts, "%zu elements loaded in hash:", bindle_hash_len(bh));
   for(pos = 0; ((keys[pos])); pos++)
   {
      if (bindle_hash_sget(bh, keys[pos], (const void **)&res, &res_len) == -1)
      {
         bindle_tests_error(opts, NULL, "bindle_hash_get(): %s", strerror(errno));
         bindle_hash_free(bh);
         return(1);
      };
      bindle_tests_verbose(opts, "   %zu: %-10s %s", pos, keys[pos], res);
   };

   // print hash table
   bindle_tests_verbose(opts, "deleting elements:", bindle_hash_len(bh));
   for(pos = 0; ((keys[pos])); pos++)
   {
      bindle_tests_verbose(opts, "   deleting key: %s", (char *)keys[pos]);
      if (bindle_hash_sdel(bh, keys[pos]) == -1)
      {
         bindle_tests_error(opts, NULL, "bindle_hash_unset(): %s", strerror(errno));
         bindle_hash_free(bh);
         return(1);
      };
      for(x = 0; (x < bh->h_len); x++)
         bindle_tests_verbose(opts, "      %zu: %-10s %s", x, (char *)bh->h_elements[x]->e_key->bv_val, (char *)bh->h_elements[x]->e_val->bv_val);
   };
   bindle_tests_verbose(opts, "%zu elements loaded in hash:", bindle_hash_len(bh));

   bindle_strsfree(keys);
   keys = NULL;

   bindle_hash_free(bh);
   bh = NULL;

   return(0);
}

/* end of source */

