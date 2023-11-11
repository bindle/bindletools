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
#define _CHECKS_BINDLE_STR_CHOMP_C 1


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


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
// MARK: - Definitions

#undef PROGRAM_NAME
#define PROGRAM_NAME "bindle-str-dirname"


/////////////////
//             //
//  Datatypes  //
//             //
/////////////////
// MARK: - Datatypes

typedef struct my_test_strings
{
   const char *   origin;
   const char *   sep;
   const char *   result;
} my_test_strings_t;


/////////////////
//             //
//  Variables  //
//             //
/////////////////
// MARK: - Variables

static my_test_strings_t  test_strings[] =
{
   { .origin = "This is a test.",               .result = "This is a test.",           .sep = NULL },
   { .origin = "This is a test.\n",             .result = "This is a test.",           .sep = NULL },
   { .origin = "This is a test.\n\n\n\n",       .result = "This is a test.",           .sep = NULL },
   { .origin = "This is a test.",               .result = "This is a test.",           .sep = "\n" },
   { .origin = "This is a test.\n",             .result = "This is a test.",           .sep = "\n" },
   { .origin = "This is a test.\n\n\n\n",       .result = "This is a test.",           .sep = "\n" },
   { .origin = "This is a test.",               .result = "This is a test.",           .sep = "\r\n" },
   { .origin = "This is a test.\n",             .result = "This is a test.\n",         .sep = "\r\n" },
   { .origin = "This is a test.\n\n\n\n",       .result = "This is a test.\n\n\n\n",   .sep = "\r\n" },
   { .origin = "This is a test.",               .result = "This is a test.",           .sep = "\r\n" },
   { .origin = "This is a test.\r\n",           .result = "This is a test.",           .sep = "\r\n" },
   { .origin = "This is a test.\r\n\r\n\r\n",   .result = "This is a test.",           .sep = "\r\n" },
   { .origin = "This is a test.",               .result = "This",                      .sep = "4" },
   { .origin = "This is a test.",               .result = "This is",                   .sep = "7" },
   { .origin = "This is a test.",               .result = "This is a",                 .sep = "9" },
   { .origin = "This is a test.",               .result = "This is a t",               .sep = "11" },
   { .origin = NULL,                            .result = NULL,                        .sep = NULL }
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

int
main(
         int                           argc,
         char *                        argv[] )
{
   int                           opt;
   int                           c;
   int                           opt_index;
   unsigned                      opts;
   const char *                  separator;
   char                          buff[256];
   size_t                        delta;
   size_t                        pos;

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
         printf("       %s [OPTIONS] [ str1 [ separator ] ]\n", PROGRAM_NAME);
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
      fprintf(stderr, "%s: unknown argument `%s'\n", PROGRAM_NAME, argv[optind+2]);
      fprintf(stderr, "Try `%s --help' for more information.\n", PROGRAM_NAME);
      return(1);
   };

   // expand custom strings
   if (optind < argc)
   {
      bindle_strlcpy(buff, argv[optind], sizeof(buff));
      separator = ((optind+1) < argc) ? argv[optind+1] : NULL;
      bindle_tests_verbose(opts, "test string:    \"%s\"", buff);
      bindle_tests_verbose(opts, "separator:      \"%s\"", (((separator)) ? separator : "\n"));
      delta = bindle_strchomp(buff, separator);
      if (!(opts & BNDLTEST_OPT_VERBOSE))
         bindle_tests_print(opts, "%s", buff);
      bindle_tests_verbose(opts, "   trimmed ==>  %zu", delta);
      bindle_tests_verbose(opts, "   result ==>   \"%s\"", buff);
      bindle_tests_verbose(opts, "");
      return(0);
   };

   // expand and verify test strings
   for(pos = 0; ((test_strings[pos].origin)); pos++)
   {
      bindle_strlcpy(buff, test_strings[pos].origin, sizeof(buff));
      separator = test_strings[pos].sep;
      bindle_tests_verbose(opts, "test string:    \"%s\"", test_strings[pos].origin);
      bindle_tests_verbose(opts, "separator:      \"%s\"", (((separator)) ? separator : "\n"));
      delta = bindle_strchomp(buff, separator);
      bindle_tests_verbose(opts, "   trimmed ==>  %zu", delta);
      bindle_tests_verbose(opts, "   expected ==> \"%s\"", test_strings[pos].result);
      bindle_tests_verbose(opts, "   result   ==> \"%s\"", buff);
      if ((strcmp(test_strings[pos].result, buff)))
      {
         bindle_tests_error(opts, NULL, "chomp does not match expected result");
         return(1);
      };
      bindle_tests_verbose(opts, "");
   };

   return(0);
}


/* end of source */

