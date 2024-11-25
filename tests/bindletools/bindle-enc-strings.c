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
#define _TESTS_BINDLE_ENC_STRINGS_C 1
#include "bindle-enc-strings.h"

///////////////
//           //
//  Headers  //
//           //
///////////////
// MARK: - Headers

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>


/////////////////
//             //
//  Variables  //
//             //
/////////////////
// MARK: - Variables

// MARK: base32_strings[]
struct test_data base32_strings[] =
{
   { .dec = "",             .enc = "",                         .nopad = 0 },
   { .dec = NULL,           .enc = "M=======",                 .nopad = 0 },
   { .dec = NULL,           .enc = "MY=====",                  .nopad = 0 },
   { .dec = "f",            .enc = "MY======",                 .nopad = 0 },
   { .dec = NULL,           .enc = "MZX=====",                 .nopad = 0 },
   { .dec = "fo",           .enc = "MZXQ====",                 .nopad = 0 },
   { .dec = "foo",          .enc = "MZXW6===",                 .nopad = 0 },
   { .dec = "foob",         .enc = "MZXW6YQ=",                 .nopad = 0 },
   { .dec = "fooba",        .enc = "MZXW6YTB",                 .nopad = 0 },
   { .dec = "foobar",       .enc = "MZXW6YTBOI======",         .nopad = 0 },
   { .dec = "foobarb",      .enc = "MZXW6YTBOJRA====",         .nopad = 0 },
   { .dec = "foobarba",     .enc = "MZXW6YTBOJRGC===",         .nopad = 0 },
   { .dec = "foobarbar",    .enc = "MZXW6YTBOJRGC4Q=",         .nopad = 0 },
   { .dec = "foobarbarf",   .enc = "MZXW6YTBOJRGC4TG",         .nopad = 0 },
   { .dec = "foobarbarfo",  .enc = "MZXW6YTBOJRGC4TGN4======", .nopad = 0 },
   { .dec = "foobarbarfoo", .enc = "MZXW6YTBOJRGC4TGN5XQ====", .nopad = 0 },
   { .dec = "",             .enc = "",                         .nopad = 1 },
   { .dec = "f",            .enc = "MY",                       .nopad = 1 },
   { .dec = "fo",           .enc = "MZXQ",                     .nopad = 1 },
   { .dec = "foo",          .enc = "MZXW6",                    .nopad = 1 },
   { .dec = "foob",         .enc = "MZXW6YQ",                  .nopad = 1 },
   { .dec = "fooba",        .enc = "MZXW6YTB",                 .nopad = 1 },
   { .dec = "foobar",       .enc = "MZXW6YTBOI",               .nopad = 1 },
   { .dec = "foobarb",      .enc = "MZXW6YTBOJRA",             .nopad = 1 },
   { .dec = "foobarba",     .enc = "MZXW6YTBOJRGC",            .nopad = 1 },
   { .dec = "foobarbar",    .enc = "MZXW6YTBOJRGC4Q",          .nopad = 1 },
   { .dec = "foobarbarf",   .enc = "MZXW6YTBOJRGC4TG",         .nopad = 1 },
   { .dec = "foobarbarfo",  .enc = "MZXW6YTBOJRGC4TGN4",       .nopad = 1 },
   { .dec = "foobarbarfoo", .enc = "MZXW6YTBOJRGC4TGN5XQ",     .nopad = 1 },
   { .dec = NULL,     .enc = NULL,                             .nopad = 0 }
};


// MARK: base32hex_strings[]
struct test_data base32hex_strings[] =
{
   { .dec = "",             .enc = "",                         .nopad = 0 },
   { .dec = "f",            .enc = "CO======",                 .nopad = 0 },
   { .dec = "fo",           .enc = "CPNG====",                 .nopad = 0 },
   { .dec = "foo",          .enc = "CPNMU===",                 .nopad = 0 },
   { .dec = "foob",         .enc = "CPNMUOG=",                 .nopad = 0 },
   { .dec = "fooba",        .enc = "CPNMUOJ1",                 .nopad = 0 },
   { .dec = "foobar",       .enc = "CPNMUOJ1E8======",         .nopad = 0 },
   { .dec = "foobarb",      .enc = "CPNMUOJ1E9H0====",         .nopad = 0 },
   { .dec = "foobarba",     .enc = "CPNMUOJ1E9H62===",         .nopad = 0 },
   { .dec = "foobarbar",    .enc = "CPNMUOJ1E9H62SG=",         .nopad = 0 },
   { .dec = "foobarbarf",   .enc = "CPNMUOJ1E9H62SJ6",         .nopad = 0 },
   { .dec = "foobarbarfo",  .enc = "CPNMUOJ1E9H62SJ6DS======", .nopad = 0 },
   { .dec = "foobarbarfoo", .enc = "CPNMUOJ1E9H62SJ6DTNG====", .nopad = 0 },
   { .dec = "",             .enc = "",                         .nopad = 1 },
   { .dec = "f",            .enc = "CO",                       .nopad = 1 },
   { .dec = "fo",           .enc = "CPNG",                     .nopad = 1 },
   { .dec = "foo",          .enc = "CPNMU",                    .nopad = 1 },
   { .dec = "foob",         .enc = "CPNMUOG",                  .nopad = 1 },
   { .dec = "fooba",        .enc = "CPNMUOJ1",                 .nopad = 1 },
   { .dec = "foobar",       .enc = "CPNMUOJ1E8",               .nopad = 1 },
   { .dec = "foobarb",      .enc = "CPNMUOJ1E9H0",             .nopad = 1 },
   { .dec = "foobarba",     .enc = "CPNMUOJ1E9H62",            .nopad = 1 },
   { .dec = "foobarbar",    .enc = "CPNMUOJ1E9H62SG",          .nopad = 1 },
   { .dec = "foobarbarf",   .enc = "CPNMUOJ1E9H62SJ6",         .nopad = 1 },
   { .dec = "foobarbarfo",  .enc = "CPNMUOJ1E9H62SJ6DS",       .nopad = 1 },
   { .dec = "foobarbarfoo", .enc = "CPNMUOJ1E9H62SJ6DTNG",     .nopad = 1 },
   { .dec = NULL,           .enc = NULL,                       .nopad = 0 }
};


// MARK: base64_strings[]
struct test_data base64_strings[] =
{
   { .dec = "",             .enc = "",                               .nopad = 0 },
   { .dec = "f",            .enc = "Zg==",                           .nopad = 0 },
   { .dec = "fo",           .enc = "Zm8=",                           .nopad = 0 },
   { .dec = "foo",          .enc = "Zm9v",                           .nopad = 0 },
   { .dec = "foob",         .enc = "Zm9vYg==",                       .nopad = 0 },
   { .dec = "fooba",        .enc = "Zm9vYmE=",                       .nopad = 0 },
   { .dec = "foobar",       .enc = "Zm9vYmFy",                       .nopad = 0 },
   { .dec = "foobarb",      .enc = "Zm9vYmFyYg==",                   .nopad = 0 },
   { .dec = "foobarba",     .enc = "Zm9vYmFyYmE=",                   .nopad = 0 },
   { .dec = "foobarbar",    .enc = "Zm9vYmFyYmFy",                   .nopad = 0 },
   { .dec = "foobarbarf",   .enc = "Zm9vYmFyYmFyZg==",               .nopad = 0 },
   { .dec = "foobarbarfo",  .enc = "Zm9vYmFyYmFyZm8=",               .nopad = 0 },
   { .dec = "foobarbarfoo", .enc = "Zm9vYmFyYmFyZm9v",               .nopad = 0 },
   { .dec = "",             .enc = "",                               .nopad = 1 },
   { .dec = "f",            .enc = "Zg",                             .nopad = 1 },
   { .dec = "fo",           .enc = "Zm8",                            .nopad = 1 },
   { .dec = "foo",          .enc = "Zm9v",                           .nopad = 1 },
   { .dec = "foob",         .enc = "Zm9vYg",                         .nopad = 1 },
   { .dec = "fooba",        .enc = "Zm9vYmE",                        .nopad = 1 },
   { .dec = "foobar",       .enc = "Zm9vYmFy",                       .nopad = 1 },
   { .dec = "foobarb",      .enc = "Zm9vYmFyYg",                     .nopad = 1 },
   { .dec = "foobarba",     .enc = "Zm9vYmFyYmE",                    .nopad = 1 },
   { .dec = "foobarbar",    .enc = "Zm9vYmFyYmFy",                   .nopad = 1 },
   { .dec = "foobarbarf",   .enc = "Zm9vYmFyYmFyZg",                 .nopad = 1 },
   { .dec = "foobarbarfo",  .enc = "Zm9vYmFyYmFyZm8",                .nopad = 1 },
   { .dec = "foobarbarfoo", .enc = "Zm9vYmFyYmFyZm9v",               .nopad = 1 },
   { .dec = "5nuvF7DY4/PY+APIzP", .enc = "NW51dkY3RFk0L1BZK0FQSXpQ", .nopad = 1 },
   { .dec = "wpq83IwsEv3ci0hf2S", .enc = "d3BxODNJd3NFdjNjaTBoZjJT", .nopad = 1 },
   { .dec = "4ltI0yv+ddKjyEN6IU", .enc = "NGx0STB5ditkZEtqeUVONklV", .nopad = 1 },
   { .dec = "sLo8FOEOQGzDtXIE72", .enc = "c0xvOEZPRU9RR3pEdFhJRTcy", .nopad = 1 },
   { .dec = "uQBsjzE1uZ287j0hIW", .enc = "dVFCc2p6RTF1WjI4N2owaElX", .nopad = 1 },
   { .dec = "5OcM4X4uibUIsf0KF/", .enc = "NU9jTTRYNHVpYlVJc2YwS0Yv", .nopad = 1 },
   { .dec = "2WGVwxQBvpMz6d/a+O", .enc = "MldHVnd4UUJ2cE16NmQvYStP", .nopad = 1 },
   { .dec = "u1OAtBFMPKCWnS7fw8", .enc = "dTFPQXRCRk1QS0NXblM3Znc4", .nopad = 1 },
   { .dec = "XsbP5uIZ1RMOLH8Tzo", .enc = "WHNiUDV1SVoxUk1PTEg4VHpv", .nopad = 1 },
   { .dec = "L8C0IUt45j42ic36+k", .enc = "TDhDMElVdDQ1ajQyaWMzNitr", .nopad = 1 },
   { .dec = "HqC2LnE2qxADXk3g7+", .enc = "SHFDMkxuRTJxeEFEWGszZzcr", .nopad = 1 },
   { .dec = "ko4QuyAGFQD2IUPZU8", .enc = "a280UXV5QUdGUUQySVVQWlU4", .nopad = 1 },
   { .dec = NULL,           .enc = NULL,                             .nopad = 0 }
};


// MARK: crockford_strings[]
struct test_data crockford_strings[] =
{
   { .dec = "",             .enc = "",                         .nopad = 0 },
   { .dec = "f",            .enc = "CR",                       .nopad = 0 },
   { .dec = "fo",           .enc = "CSQG",                     .nopad = 0 },
   { .dec = "foo",          .enc = "CSQPY",                    .nopad = 0 },
   { .dec = "foob",         .enc = "CSQPYRG",                  .nopad = 0 },
   { .dec = "fooba",        .enc = "CSQPYRK1",                 .nopad = 0 },
   { .dec = "foobar",       .enc = "CSQPYRK1E8",               .nopad = 0 },
   { .dec = "foobarb",      .enc = "CSQPYRK1E9H0",             .nopad = 0 },
   { .dec = "foobarba",     .enc = "CSQPYRK1E9H62",            .nopad = 0 },
   { .dec = "foobarbar",    .enc = "CSQPYRK1E9H62WG",          .nopad = 0 },
   { .dec = "foobarbarf",   .enc = "CSQPYRK1E9H62WK6",         .nopad = 0 },
   { .dec = "foobarbarfo",  .enc = "CSQPYRK1E9H62WK6DW",       .nopad = 0 },
   { .dec = "foobarbarfoo", .enc = "CSQPYRK1E9H62WK6DXQG",     .nopad = 0 },
   { .dec = NULL,           .enc = NULL,                       .nopad = 0 }
};


// MARK: hex_strings[]
struct test_data hex_strings[] =
{
   { .dec = "",             .enc = "",                         .nopad = 0 },
   { .dec = "f",            .enc = "66",                       .nopad = 0 },
   { .dec = "fo",           .enc = "666f",                     .nopad = 0 },
   { .dec = "foo",          .enc = "666f6f",                   .nopad = 0 },
   { .dec = "foob",         .enc = "666f6f62",                 .nopad = 0 },
   { .dec = "fooba",        .enc = "666f6f6261",               .nopad = 0 },
   { .dec = "foobar",       .enc = "666f6f626172",             .nopad = 0 },
   { .dec = "foobarb",      .enc = "666f6f62617262",           .nopad = 0 },
   { .dec = "foobarba",     .enc = "666f6f6261726261",         .nopad = 0 },
   { .dec = "foobarbar",    .enc = "666f6f626172626172",       .nopad = 0 },
   { .dec = "foobarbarf",   .enc = "666f6f62617262617266",     .nopad = 0 },
   { .dec = "foobarbarfo",  .enc = "666f6f626172626172666f",   .nopad = 0 },
   { .dec = "foobarbarfoo", .enc = "666f6f626172626172666f6f", .nopad = 0 },
   { .dec = NULL,           .enc = NULL,                       .nopad = 0 }
};


// MARK: pctenc_strings[]
struct test_data pctenc_strings[] =
{
   { .dec = "",               .enc = "",                       .nopad = 0 },
   { .dec = "foobar",         .enc = "foobar",                 .nopad = 0 },
   { .dec = "foo bar",        .enc = "foo+bar",                .nopad = 0 },
   { .dec = "foo&bar",        .enc = "foo%26bar",              .nopad = 0 },
   { .dec = "foo&bar=100%",   .enc = "foo%26bar%3d100%25",     .nopad = 0 },
   { .dec = NULL,             .enc = NULL,                     .nopad = 0 }
};


/////////////////
//             //
//  Functions  //
//             //
/////////////////
// MARK: - Functions

int
bindle_test_decode(
         const char *                  prog_name,
         int                           argc,
         char *                        argv[],
         int                           method,
         struct test_data *            data )
{
   int               opt;
   int               c;
   int               opt_index;
   unsigned          opts;
   size_t            pos;
   char              buff[24];
   ssize_t           len;
   const char *      dec;
   const char *      enc;
   int               exit_code;
   char              msg[128];

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

   bindle_tests_initialize(prog_name);

   opts = 0;
   exit_code = 0;

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
         printf("Usage: %s [OPTIONS]\n", prog_name);
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
         fprintf(stderr, "Try `%s --help' for more information.\n", prog_name);
         return(1);

         default:
         fprintf(stderr, "%s: unrecognized option `--%c'\n", prog_name, c);
         fprintf(stderr, "Try `%s --help' for more information.\n", prog_name);
         return(1);
      };
   };
   if (optind < argc)
   {
      fprintf(stderr, "%s: unknown argument `%s'\n", prog_name, argv[optind]);
      fprintf(stderr, "Try `%s --help' for more information.\n", prog_name);
      return(1);
   };

   for(pos = 0; (((data[pos].dec)) || ((data[pos].enc))); pos++)
   {
      dec = data[pos].dec;
      enc = data[pos].enc;

      if ( (!(dec)) || (!(enc)) )
         continue;

      snprintf(msg, sizeof(msg), "decoding \"%s\" ... ", enc);
      if (!(opts & BNDLTEST_OPT_QUIET))
         printf("%-40s", msg);

      len = bindle_decode(method, buff, sizeof(buff), enc, strlen(enc));
      if (len == -1)
      {
         if (!(opts & BNDLTEST_OPT_QUIET))
            printf("FAIL -- %s\n", strerror(errno));
         exit_code = 1;
      } else {
         buff[len] = '\0';
         if (!(strcmp(dec, buff)))
         {
            if (!(opts & BNDLTEST_OPT_QUIET))
               printf("PASS\n");
         } else {
            if (!(opts & BNDLTEST_OPT_QUIET))
               printf("FAIL \"%s\"\n", buff);
            exit_code = 1;
         };
      };
   };

   return(exit_code);
}


int
bindle_test_encode(
         const char *                  prog_name,
         int                           argc,
         char *                        argv[],
         int                           method,
         struct test_data *            data )
{
   int               opt;
   int               c;
   int               opt_index;
   unsigned          opts;
   size_t            pos;
   char              buff[32];
   ssize_t           len;
   const char *      dec;
   const char *      enc;
   int               exit_code;
   int               nopad;
   char              msg[128];

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

   bindle_tests_initialize(prog_name);

   opts = 0;
   exit_code = 0;

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
         printf("Usage: %s [OPTIONS]\n", prog_name);
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
         fprintf(stderr, "Try `%s --help' for more information.\n", prog_name);
         return(1);

         default:
         fprintf(stderr, "%s: unrecognized option `--%c'\n", prog_name, c);
         fprintf(stderr, "Try `%s --help' for more information.\n", prog_name);
         return(1);
      };
   };
   if (optind < argc)
   {
      fprintf(stderr, "%s: unknown argument `%s'\n", prog_name, argv[optind]);
      fprintf(stderr, "Try `%s --help' for more information.\n", prog_name);
      return(1);
   };
   
   for(pos = 0; (((data[pos].dec)) || ((data[pos].enc))); pos++)
   {
      dec   = data[pos].dec;
      enc   = data[pos].enc;
      nopad = (int)data[pos].nopad;

      if ( (!(dec)) || (!(enc)) )
         continue;

      snprintf(msg, sizeof(msg), "encoding \"%s\" ... ", dec);
      if (!(opts & BNDLTEST_OPT_QUIET))
         printf("%-35s", msg);

      len = bindle_encode(method, buff, sizeof(buff), dec, strlen(dec), nopad);
      if (len == -1)
      {
         if (!(opts & BNDLTEST_OPT_QUIET))
            printf("FAIL -- %s\n", strerror(errno));
         exit_code = 1;
      } else {
         buff[len] = '\0';
         if (!(strcmp(enc, buff)))
         {
            if (!(opts & BNDLTEST_OPT_QUIET))
               printf("PASS\n");
         } else {
            if (!(opts & BNDLTEST_OPT_QUIET))
               printf("FAIL expected: \"%s\"; received: \"%s\"\n", enc, buff);
            exit_code = 1;
         };
      };
   };

   return(exit_code);
}


int
bindle_test_validate(
         const char *                  prog_name,
         int                           argc,
         char *                        argv[],
         int                           method,
         struct test_data *            data )
{
   int               opt;
   int               c;
   int               opt_index;
   unsigned          opts;
   size_t            pos;
   ssize_t           rc;
   ssize_t           len;
   const char *      dec;
   const char *      enc;
   int               exit_code;
   char              msg[128];

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

   bindle_tests_initialize(prog_name);

   opts = 0;
   exit_code = 0;

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
         printf("Usage: %s [OPTIONS]\n", prog_name);
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
         fprintf(stderr, "Try `%s --help' for more information.\n", prog_name);
         return(1);

         default:
         fprintf(stderr, "%s: unrecognized option `--%c'\n", prog_name, c);
         fprintf(stderr, "Try `%s --help' for more information.\n", prog_name);
         return(1);
      };
   };
   if (optind < argc)
   {
      fprintf(stderr, "%s: unknown argument `%s'\n", prog_name, argv[optind]);
      fprintf(stderr, "Try `%s --help' for more information.\n", prog_name);
      return(1);
   };
   
   for(pos = 0; ((data[pos].enc)); pos++)
   {
      dec   = data[pos].dec;
      enc   = data[pos].enc;

      snprintf(msg, sizeof(msg), "validating \"%s\" ... ", enc);
      if (!(opts & BNDLTEST_OPT_QUIET))
         printf("%-45s", msg);

      rc  = bindle_encoding_verify(method, enc, strlen(enc));
      len = ((dec)) ? ((ssize_t)strlen(dec)) : -1;

      if ( (!(dec)) && (len >= 0) )
      {
         if (!(opts & BNDLTEST_OPT_QUIET))
            printf("FAIL -- validate passed bad string\n");
         exit_code = 1;
      } else if (len != rc)
      {
         if (!(opts & BNDLTEST_OPT_QUIET))
            printf("FAIL -- validate reported incorrect length\n");
         exit_code = 1;
      } else
      {
         if (!(opts & BNDLTEST_OPT_QUIET))
            printf(((dec)) ? "PASS\n" : "PASS (caught bad encoding)\n");
      };
   };

   return(exit_code);
}

/* end of source file */
