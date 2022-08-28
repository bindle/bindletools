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
#define _TESTS_BINDLE_ARRAY_STACK_C 1


///////////////
//           //
//  Headers  //
//           //
///////////////
#pragma mark - Headers

#include "bindle-tests.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <getopt.h>
#include <assert.h>
#include <inttypes.h>


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#pragma mark - Definitions

#undef PROGRAM_NAME
#define PROGRAM_NAME "bindle-array-stack"

#define MY_LIST_LEN  256


//////////////////
//              //
//  Data Types  //
//              //
//////////////////
#pragma mark - Data Types

typedef struct _mydata MyData;

struct _mydata
{
   char *            name;
   int               value;
   int               opts;
};


/////////////////
//             //
//  Variables  //
//             //
/////////////////
#pragma mark - Variables

static const char * test_strs[] =
{
   "cccccccc",
   "bbbbbbbb",
   "aaaaaaaa",
   NULL
};


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#pragma mark - Prototypes

int
main(
         int                           argc,
         char *                        argv[] );


static char *
my_strrand(
         size_t                        min,
         size_t                        max );


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#pragma mark - Functions

int main( int argc, char * argv[] )
{
   int                  opt;
   int                  c;
   int                  opt_index;
   unsigned             opts;
   size_t               pos;
   size_t               len;
   size_t               list_len;
   size_t               x;
   size_t               y;
   MyData               data[MY_LIST_LEN];
   MyData *             src[MY_LIST_LEN];
   MyData **            list;
   MyData **            objp;

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
   memset(data, 0, sizeof(data));
   for(len = 0; ((test_strs[len])); len++);
   for(pos = 0; (pos < MY_LIST_LEN); pos++)
   {
      data[pos].value = (int)(~pos);
      data[pos].opts  = (int)pos;
      data[pos].name  = (pos < len) ? bindle_strdup(test_strs[pos]) : my_strrand(8, 8);
      if (!(data[pos].name))
      {
         for(; (pos > 0); pos--)
            free(data[pos-1].name);
         return(bindle_tests_error(opts, NULL, "out of virtual memory"));
      };
   };


   // populate src and test arrays
   for(pos = 0; (pos < MY_LIST_LEN); pos++)
      src[pos] = &data[pos];


   // loop through test data and add to stack
   bindle_tests_verbose(opts, "pushing to stack ...");
   list     = NULL;
   list_len = 0;
   for(x = 0; (x < MY_LIST_LEN); x++)
   {
      // periodically pop data
      if ((x % 4) == 3)
      {
         if ((objp = bindle_pop(list, &list_len, sizeof(MyData *))) == NULL)
            return(bindle_tests_error(opts, NULL, "unable to pop from stack"));
         if ((strcasecmp(src[x-1]->name, (*objp)->name)))
            return(bindle_tests_error(opts, NULL, "str of pop object is incorrect"));
      };

      // adds objects to stack
      if (bindle_push(&src[x], (void **)&list, &list_len, sizeof(MyData *), &realloc) == -1)
         return(bindle_tests_error(opts, NULL, "failed to push object to stack"));
   };


   // loop through stack and verify data
   bindle_tests_verbose(opts, "verifing stack ...");
   if (list_len != ((MY_LIST_LEN / 4) * 3))
      return(bindle_tests_error(opts, NULL, "stack length is incorrect"));
   for(x = 0, y = 0; (x < MY_LIST_LEN); x++)
   {
      if ((x % 4) == 2)
         continue;
      if ((strcmp(src[x]->name, list[y]->name)))
         return(bindle_tests_error(opts, NULL, "stack data does not match test data"));
      y++;
   };


   // pop data from stack
   bindle_tests_verbose(opts, "popping from stack ...");
   while (list_len > 0)
      if (bindle_pop(list, &list_len, sizeof(MyData *)) == NULL)
         return(bindle_tests_error(opts, NULL, "unable to pop from stack"));

   return(0);
}


char *
my_strrand(
         size_t                        min,
         size_t                        max )
{
   char *      str;
   size_t      len;
   size_t      pos;
   static int  count = 0;

   assert(min <= max);

   len = ((max - min)) ? ((unsigned)random() % (max - min)) : 0;
   len += min;
   if (len < sizeof(int))
      len = sizeof(int);

   if ((str = malloc(len+1)) == NULL)
      return(NULL);
   str[len] = '\0';

   for(pos = 0; (pos < (len-sizeof(int))); pos++)
      str[pos] = (random() % 26) + 'a';

   // ensure unique value for test
   snprintf(&str[pos], (len-pos+1), "%04xu", (~count)&0xffff);
   count++;

   return(str);
}

/* end of source */

