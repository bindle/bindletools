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
#define _TESTS_TEST_STR_EXPAND_C 1


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
#define PROGRAM_NAME "test-str-expand"

#define TEST_OPT_FORCE     0x0040UL


/////////////////
//             //
//  Datatypes  //
//             //
/////////////////
// MARK: - Datatypes

typedef struct my_test_strings
{
   const char *   origin;
   const char *   expand;
   intptr_t       opts;
} MyTestStr;


/////////////////
//             //
//  Variables  //
//             //
/////////////////
// MARK: - Variables

static const MyTestStr  test_strings[] =
{
   { "asdfghj",                              "asdfghj",                       0 },
   { "'asdfghj'",                            "asdfghj",                       0 },
   { "\"asdfghj\"",                          "asdfghj",                       0 },
   { "\\141\\163\\144\\146\\147\\150\\152",  "asdfghj",                       BNDL_EXP_FORCE },
   { "\\x61\\x73\\x64\\x66\\x67\\x68\\x6a",  "asdfghj",                       BNDL_EXP_FORCE },
   { "a\\163\\x64f\\147\\x68j",              "asdfghj",                       BNDL_EXP_FORCE },
   { "a%%sdfghj",                            "a%sdfghj",                      BNDL_EXP_FORCE },
   { "zaq%%-%p-%%qaz",                       "zaq%-" PROGRAM_NAME "-%qaz",    BNDL_EXP_FORCE },
   { "%u:x:%U:%G:test user:%d:/bin/false",   NULL,                            BNDL_EXP_FORCE },
   { "%g:x:%G:%u,root",                      NULL,                            BNDL_EXP_FORCE },
   { "Is %p running successfully?",          NULL,                            BNDL_EXP_FORCE },
   { "Is your email address %u@%D\?",        NULL,                            BNDL_EXP_FORCE },
   { "is your email address %u@%H\?",        NULL,                            BNDL_EXP_FORCE },
   { "Is your email address %u@%h.%D\?",     NULL,                            BNDL_EXP_FORCE },
   { NULL, NULL, 0 }
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
   size_t                        pos;
   unsigned                      opts;
   char                          buff[BNDL_LINE_MAX_LEN];
   const MyTestStr *             test_str;

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
      {"force",            no_argument,       NULL, 1   },
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

         case 1:
         opts |= TEST_OPT_FORCE;
         break;

         case 'd':
         opt = BNDL_DEBUG_ANY;
         bindle_set_option(NULL, BNDL_OPT_DEBUG_LEVEL,  &opt);
         break;

         case 'h':
         printf("Usage: %s [OPTIONS]\n", PROGRAM_NAME);
         printf("       %s [OPTIONS] str1 [ str2 [ ... [ strN ] ] ]\n", PROGRAM_NAME);
         printf("OPTIONS:\n");
         printf("  -d, --debug               print debug messages\n");
         printf("  -h, --help                print this help and exit\n");
         printf("  -q, --quiet, --silent     do not print messages\n");
         printf("  -V, --version             print version number and exit\n");
         printf("  -v, --verbose             print verbose messages\n");
         printf("  --force                   force expansion of user strings\n");
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

   // expand custom strings
   if (optind < argc)
   {
      while (optind < argc)
      {
         bindle_tests_verbose(opts, "test string:  %s", argv[optind]);
         if (!(bindle_strexpand(buff,  argv[optind], sizeof(buff), (((opts&TEST_OPT_FORCE)) ? BNDL_EXP_FORCE : 0))))
         {
            bindle_tests_error(opts, NULL, "bindle_strexpand(): error occurred");
            return(1);
         };
         buff[sizeof(buff)-1] = '\0';
         if (!(opts & BNDLTEST_OPT_VERBOSE))
            bindle_tests_print(opts, "%s", buff);
         bindle_tests_verbose(opts, "         ==>  %s", buff);
         optind++;
         if (optind < argc)
            bindle_tests_verbose(opts, "");
     };
      return(0);
   };

   // expand and verify test strings
   for(pos = 0; ((test_strings[pos].origin)); pos++)
   {
      test_str = &test_strings[pos];
      bindle_tests_verbose(opts, "test string:  %s", test_strings[pos].origin);
      if (!(bindle_strexpand(buff, test_str->origin, sizeof(buff), (int)test_str->opts)))
      {
         bindle_tests_error(opts, NULL, "bindle_strexpand(): error occurred");
         return(1);
      };
      buff[sizeof(buff)-1] = '\0';
      bindle_tests_verbose(opts, "         ==>  %s", buff);
      if ((test_strings[pos].expand))
      {
         if ((strcmp(buff, test_strings[pos].expand)))
         {
            bindle_tests_error(opts, NULL, "expected: %s", test_strings[pos].expand);
            return(1);
         };
      };
      bindle_tests_verbose(opts, "");
   };

   return(0);
}


/* end of source */

