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
#define _CHECKS_BINDLE_URL_ENCODING_C 1


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


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#pragma mark - Definitions

#undef PROGRAM_NAME
#define PROGRAM_NAME "bindle-url-encoding"


//////////////////
//              //
//  Data Types  //
//              //
//////////////////
#pragma mark - Data Types

typedef struct my_test_pairs
{
   const char *   decoded;
   const char *   encoded;
} MyPairs;


/////////////////
//             //
//  Variables  //
//             //
/////////////////
#pragma mark - Variables

static MyPairs test_pairs[] =
{
   { "127.0.0.1", NULL },
   { "Hello World!!!", NULL },
   { "How-much_bits,can~a:bitchuck;chuck/if=@+bitchuck would?chuck'bits", NULL },
   { NULL, NULL }
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


static int
my_test_pair(
         unsigned                      opts,
         unsigned                      idx,
         const MyPairs *               pair );


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#pragma mark - Functions

int
main(
         int                           argc,
         char *                        argv[] )
{
   int               opt;
   int               c;
   int               opt_index;
   unsigned          opts;
   MyPairs           pair;
   unsigned          pos;


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

   opts   = 0;

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
         printf("       %s [OPTIONS] [ <decoded string> [ <encoded string ] ]\n", PROGRAM_NAME);
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
   if ((optind+2) < argc)
   {
      fprintf(stderr, "%s: unrecognized argument\n", PROGRAM_NAME);
      fprintf(stderr, "Try `%s --help' for more information.\n", PROGRAM_NAME);
      return(1);
   };
   if (optind < argc)
   {
      pair.decoded = argv[optind];
      pair.encoded = ((optind+1) < optind) ? argv[optind+1] : NULL;
      return(my_test_pair(opts, 1, &pair));
   };

   for(pos = 0; (((test_pairs[pos].decoded)) || ((test_pairs[pos].encoded))); pos++)
      if ((my_test_pair(opts, pos, &test_pairs[pos])))
         return(1);

   return(0);
}


int
my_test_pair(
         unsigned                      opts,
         unsigned                      idx,
         const MyPairs *               pair )
{
   int   rc;
   char  encoded[4096];
   char  decoded[4096];

   rc = 0;

   if ((pair->decoded))
   {
      bindle_tests_verbose(opts, "%u: encoding string \"%s\" ...", idx, pair->decoded);
      bindle_url_encode(pair->decoded, encoded, sizeof(encoded));
      bindle_tests_verbose(opts, "%u: encoded string  \"%s\"", idx, encoded);

      bindle_tests_verbose(opts, "%u: decoding string \"%s\" ...", idx, encoded);
      bindle_url_decode(encoded, decoded, sizeof(decoded));
      bindle_tests_verbose(opts, "%u: decoded string  \"%s\"", idx, decoded);

      if ( ((pair->encoded)) && ((strcmp(encoded, pair->encoded))) )
      {
         bindle_tests_error(opts, NULL, "%u: encoded string does not match expected string", idx);
         rc = 1;
      };
      if ((strcmp(decoded, pair->decoded)))
      {
         bindle_tests_error(opts, NULL, "%u: decoded string does not match original string", idx);
         rc = 1;
      };
      return(rc);
   }

   if ((pair->encoded))
   {
      bindle_tests_verbose(opts, "%u: decoding string \"%s\" ...", idx, pair->encoded);
      bindle_url_decode(pair->encoded, decoded, sizeof(decoded));
      bindle_tests_verbose(opts, "%u: decoded string  \"%s\"", idx, decoded);

      bindle_tests_verbose(opts, "%u: encoding string \"%s\" ...", idx, decoded);
      bindle_url_encode(decoded, encoded, sizeof(decoded));
      bindle_tests_verbose(opts, "%u: encoded string  \"%s\"", idx, encoded);

      if ((strcmp(encoded, pair->encoded)))
      {
         bindle_tests_error(opts, NULL, "%u: encoded string does not match original string", idx);
         return(1);
      };
      return(0);
   };

   return(0);
}

/* end of source */

