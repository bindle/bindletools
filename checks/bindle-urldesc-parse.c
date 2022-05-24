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
#define _CHECKS_BINDLE_URLDESC_PARSE_C 1


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
#define PROGRAM_NAME "bindle-urldesc-parse"


/////////////////
//             //
//  Variables  //
//             //
/////////////////
#pragma mark - Variables

static const char * test_urls_good[] =
{
   "127.0.0.1",
   NULL
};

static const char * good_sc[] =
{
   "",
   "http://",
   "https://",
   NULL
};

static const char * good_us[] =
{
   "",
   "jdoe@",
   "jdoe:pass@",
   NULL
};

static const char * good_ho[] =
{
   "",
   "127.0.0.1",
   "[::1]",
   "example.com",
   NULL
};

static const char * good_se[] =
{
   "",
   ":http",
   ":https",
   ":80",
   NULL
};

static const char * good_pa[] =
{
   "",
   "/this/is/a/path",
   "/",
   NULL
};

static const char * good_qu[] =
{
   "",
   "?",
   "?this-is?asd=qe",
   NULL
};

static const char * good_fr[] =
{
   "",
   "#",
   "#testaasd",
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


static int
my_test_url(
         unsigned                      opts,
         const char *                  url );


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
   size_t            pos;
   size_t            pos_sc;
   size_t            pos_us;
   size_t            pos_ho;
   size_t            pos_se;
   size_t            pos_pa;
   size_t            pos_qu;
   size_t            pos_fr;
   char              url[512];

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
         printf("       %s [OPTIONS] url1 [ url2 [ ... [ urlN ] ] ]\n", PROGRAM_NAME);
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

   if (optind < argc)
   {
      while (optind < argc)
      {
         if ((my_test_url(opts, argv[optind])))
            return(1);
         optind++;
      };
      return(0);
   };

   // test known good URLs
   for(pos = 0; ((test_urls_good[pos])); pos++)
      if ((my_test_url(opts, test_urls_good[pos])))
         return(1);

   // test generated good URLs
   for(pos_sc = 0; ((good_sc[pos_sc])); pos_sc++)
   {
      for(pos_us = 0; ((good_us[pos_us])); pos_us++)
      {
         for(pos_ho = 0; ((good_ho[pos_ho])); pos_ho++)
         {
            for(pos_se = 0; ((good_se[pos_se])); pos_se++)
            {
               for(pos_pa = 0; ((good_pa[pos_pa])); pos_pa++)
               {
                  for(pos_qu = 0; ((good_qu[pos_qu])); pos_qu++)
                  {
                     for(pos_fr = 0; ((good_fr[pos_fr])); pos_fr++)
                     {
                        bindle_strlcpy(url, good_sc[pos_sc], sizeof(url));
                        bindle_strlcat(url, good_us[pos_us], sizeof(url));
                        bindle_strlcat(url, good_ho[pos_ho], sizeof(url));
                        bindle_strlcat(url, good_se[pos_se], sizeof(url));
                        bindle_strlcat(url, good_pa[pos_pa], sizeof(url));
                        bindle_strlcat(url, good_qu[pos_qu], sizeof(url));
                        bindle_strlcat(url, good_fr[pos_fr], sizeof(url));
                        if ((url[0]))
                           if ((my_test_url(opts, url)))
                              return(1);
                     };
                  };
               };
            };
         };
      };
   };

   return(0);
}


int
my_test_url(
         unsigned                      opts,
         const char *                  url )
{
   int               rc;
   BindleURLDesc *   budp;

   bindle_tests_print(opts, "testing \"%s\" ...", url);

   if (!(bindle_is_url(url)))
   {
      bindle_tests_error(opts, NULL, "bindle_is_url(): not a URL\n");
      return(1);
   };

   if ((rc = bindle_urldesc_parse(url, &budp)) != 0)
   {
      bindle_tests_error(opts, NULL, "bindle_urldesc_parse(): %s\n", strerror(rc));
      return(1);
   };

   if ((budp->bud_scheme))       bindle_tests_verbose(opts, "   scheme:    %s", budp->bud_scheme);
   if ((budp->bud_userinfo))     bindle_tests_verbose(opts, "   userinfo:  %s", budp->bud_userinfo);
   if ((budp->bud_host))         bindle_tests_verbose(opts, "   host:      %s", budp->bud_host);
   if ((budp->bud_port))         bindle_tests_verbose(opts, "   port:      %u", budp->bud_port);
   else if ((budp->bud_service)) bindle_tests_verbose(opts, "   service:   %s", budp->bud_service);
   if ((budp->bud_path))         bindle_tests_verbose(opts, "   path:      %s", budp->bud_path);
   if ((budp->bud_query))        bindle_tests_verbose(opts, "   query:     %s", budp->bud_query);
   if ((budp->bud_fragment))     bindle_tests_verbose(opts, "   fragment:  %s", budp->bud_fragment);

   bindle_urldesc_free(budp);

   return(0);
}

/* end of source */

