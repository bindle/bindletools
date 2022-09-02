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
#define _SRC_BINDLETOOLS_C 1
#include "bindletools.h"


///////////////
//           //
//  Headers  //
//           //
///////////////
#pragma mark - Headers

#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>


//////////////
//          //
//  Macros  //
//          //
//////////////
#pragma mark - Macros


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#pragma mark - Definitions


//////////////////
//              //
//  Data Types  //
//              //
//////////////////
#pragma mark - Data Types


/////////////////
//             //
//  Variables  //
//             //
/////////////////
#pragma mark - Variables

#pragma mark bindle_options[]
const char * const bindle_options[] =
{
   "  -D level, --debug=level   set debug level",
   "  -h, --help                print this help and exit",
   "  -q, --quiet, --silent     do not print messages",
   "  -V, --version             print version number and exit",
   "  -v, --verbose             print verbose messages",
   NULL
};


#pragma mark bindle_widget_map[]
const bindle_widget_t bindle_widget_map[] =
{
   {
      .name       = "base16",
      .flags      = 0,
      .desc       = "encode/decode base16 string (RFC4648)",
      .usage      = NULL,
      .options    = bindle_widget_encodings_options,
      .aliases    = (const char * const[]) { "b16", "hex", NULL },
      .func_exec  = &bindle_widget_base16,
      .func_usage = NULL,
   },
   {
      .name       = "base32",
      .flags      = 0,
      .desc       = "encode/decode base32 string (RFC4648)",
      .usage      = NULL,
      .options    = bindle_widget_encodings_options,
      .aliases    = (const char * const[]) { "b32", NULL },
      .func_exec  = &bindle_widget_base32,
      .func_usage = NULL,
   },
   {
      .name       = "base32hex",
      .flags      = 0,
      .desc       = "encode/decode base32hex string (RFC4648)",
      .usage      = NULL,
      .options    = bindle_widget_encodings_options,
      .aliases    = (const char * const[]) { "b32hex", NULL },
      .func_exec  = &bindle_widget_base32hex,
      .func_usage = NULL,
   },
   {
      .name       = "base64",
      .flags      = 0,
      .desc       = "encode/decode base64 string (RFC4648)",
      .usage      = NULL,
      .options    = bindle_widget_encodings_options,
      .aliases    = (const char * const[]) { "b64", NULL },
      .func_exec  = &bindle_widget_base64,
      .func_usage = NULL,
   },
   {
      .name       = "crockfordbase32",
      .flags      = 0,
      .desc       = "encode/decode Crockford's base32 string",
      .usage      = NULL,
      .options    = bindle_widget_encodings_options,
      .aliases    = (const char * const[]) { "cb32", "crockford32", NULL },
      .func_exec  = &bindle_widget_crockfordbase32,
      .func_usage = NULL,
   },
   {
      .name       = "expand",
      .flags      = 0,
      .desc       = "expand tokens within string",
      .usage      = " str1 str2 ... strN",
      .options    = NULL,
      .aliases    = NULL,
      .func_exec  = &bindle_widget_expand,
      .func_usage = &bindle_widget_expand_usage,
   },
   {
      .name       = "help",
      .flags      = 0,
      .desc       = "displays usage",
      .usage      = NULL,
      .options    = NULL,
      .aliases    = (const char * const[]) { "usage", NULL },
      .func_exec  = &bindle_usage,
      .func_usage = NULL,
   },
   {
      .name       = "urldesc",
      .flags      = 0,
      .desc       = "parses URL",
      .usage      = " url",
      .options    = bindle_widget_urldesc_options,
      .aliases    = (const char * const[]) { "url", NULL },
      .func_exec  = &bindle_widget_urldesc,
      .func_usage = NULL,
   },
   {
      .name       = "urlencoding",
      .flags      = 0,
      .desc       = "encode/decode string for URL",
      .usage      = NULL,
      .options    = bindle_widget_urlencoding_options,
      .aliases    = (const char * const[]) { "percent-encoding", NULL },
      .func_exec  = &bindle_widget_urlencoding,
      .func_usage = NULL,
   },
   {
      .name       = "version",
      .flags      = 0,
      .desc       = "displays version",
      .usage      = NULL,
      .options    = NULL,
      .aliases    = NULL,
      .func_exec  = &bindle_version,
      .func_usage = NULL,
   },
   {
      .name       = NULL,
      .desc       = NULL,
      .usage      = NULL,
      .options    = NULL,
      .aliases    = NULL,
      .func_exec  = NULL,
      .func_usage = NULL,
   }
};


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#pragma mark - Prototypes

static int
bindle_usage_cmp(
         const void *                  a,
         const void *                  b );


static const bindle_widget_t *
bindle_widget(
         const char *                     str,
         int                              exact );


extern int
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
bindle_getopt(
         bindle_conf_t *               cnf,
         int                           argc,
         char * const *                argv,
         const char *                  optstring,
         const struct option *         longopts,
         int *                         longindex )
{
   int      c;
   int      i;
   char *   end;

   switch(c = getopt_long(argc, argv, optstring, longopts, longindex))
   {
      case -1:       /* no more arguments */
      case 0:        /* long options toggles */
      break;

      case 'D':
      end = NULL;
      i = (int)strtoll(optarg, &end, 0);
      if ( (!(end[0])) || (!(optarg[0])) )
      {  fprintf(stderr, "%s: invalid debug value `%s'\n", cnf->prog_name, optarg);
         fprintf(stderr, "Try `%s --help' for more information.\n", cnf->prog_name);
         return(1);
      };
      bindle_set_option(NULL, BNDL_OPT_DEBUG_LEVEL, &i);
      return(0);

      case 'h':
      bindle_usage(cnf);
      return(-2);

      case 'q':
      cnf->opts |=  BINDLE_OPT_QUIET;
      cnf->opts &= ~BINDLE_OPT_VERBOSE;
      return(0);

      case 'V':
      bindle_version(cnf);
      return(-2);

      case 'v':
      cnf->opts |=  BINDLE_OPT_VERBOSE;
      cnf->opts &= ~BINDLE_OPT_QUIET;
      return(0);

      default:
      break;
   };
   return(c);
}


int
bindle_usage(
         bindle_conf_t *               cnf )
{
   size_t                     pos;
   size_t                     x;
   size_t                     option_len;
   const bindle_widget_t *    widget;
   const char *               widget_help;
   const char *               widget_name;
   const char *               options[128];

   option_len   = 0;

   if ((cnf->widget))
   {
      // add widget options
      if ((cnf->widget->options))
         for(pos = 0; ( ((cnf->widget->options[pos])) && (option_len < 128) ); pos++)
            options[option_len++] = cnf->widget->options[pos];

      // unlink reserved widgets
      if (!(strcasecmp(cnf->widget->name, "help")))
         cnf->widget = NULL;
   };

   // add common options and sort all options
   for(pos = 0; ( ((bindle_options[pos])) && (option_len < 128) ); pos++)
      options[option_len++] = bindle_options[pos];
   qsort(options, option_len, sizeof(char *), bindle_usage_cmp);

   widget_name = ((cnf->widget)) ? cnf->widget->name  : "widget";
   widget_help = ((cnf->widget)) ? cnf->widget->usage : "";
   widget_help = ((widget_help)) ? widget_help        : "";

   printf("Usage: %s [OPTIONS] %s [OPTIONS]%s\n", PROGRAM_NAME, widget_name, widget_help);
   printf("       %s-%s [OPTIONS]%s\n", PROGRAM_NAME, widget_name, widget_help);
   printf("       %s%s [OPTIONS]%s\n", PROGRAM_NAME, widget_name, widget_help);
   printf("OPTIONS:\n");
   for(pos = 0; (pos < option_len); pos++)
      printf("%s\n", options[pos]);

   if (!(cnf->widget))
   {
      printf("WIDGETS:\n");
      for(pos = 0; bindle_widget_map[pos].name != NULL; pos++)
      {
         widget = &bindle_widget_map[pos];
         if ((widget->desc))
            printf("  %-25s %s\n", widget->name, widget->desc);
      };

      if ((cnf->opts & BINDLE_OPT_VERBOSE))
      {
         printf("WIDGET ALIASES:\n");
         for(pos = 0; bindle_widget_map[pos].name != NULL; pos++)
         {
            widget = &bindle_widget_map[pos];
            if ( (!(widget->aliases)) || (!(widget->desc)) )
               continue;
            for(x = 0; ((widget->aliases[x])); x++)
               printf("  %-25s alias for %s\n", widget->aliases[x], widget->name);
         };
      };
   };

   if ((cnf->widget))
      if ((cnf->widget->func_usage))
         cnf->widget->func_usage(cnf);

   printf("\n");

   return(0);
}


int
bindle_usage_cmp(
         const void *                  a,
         const void *                  b )
{
   int          rc;
   char * const * str1 = a;
   char * const * str2 = b;
   if ( ((*str1)[3] == '-') && ((*str2)[3] != '-') )
      return(1);
   if ( ((*str1)[3] != '-') && ((*str2)[3] == '-') )
      return(-1);
   if ((rc = strcasecmp(*str1, *str2)) != 0)
      return(rc);
   return(strcmp(*str1, *str2));
}


int
bindle_version(
         bindle_conf_t *               cnf )
{
   const char * prog_name;
   prog_name = ((cnf)) ? cnf->prog_name : PROGRAM_NAME;
   printf("%s (%s) %s\n", prog_name, PACKAGE_NAME, PACKAGE_VERSION);
   return(0);
}


const bindle_widget_t *
bindle_widget(
         const char *                     str,
         int                              exact )
{
   size_t                     len;
   size_t                     x;
   size_t                     y;
   size_t                     matches;
   const char *               alias;
   const bindle_widget_t *    match;
   const bindle_widget_t *    widget;

   match   = NULL;
   matches = 0;

   // trim prefix from string
   len = strlen(PROGRAM_NAME);
   if (!(strncasecmp(str, PROGRAM_NAME, len)))
      str = &str[len];
   if (str[0] == '-')
      str = &str[1];
   if (!(str[0]))
      return(NULL);
   len = strlen(str);

   // loop through widget map
   for(x = 0; ((bindle_widget_map[x].name)); x++)
   {
      widget = &bindle_widget_map[x];
      if (widget->func_exec == NULL)
         continue;

      // check widget name for string
      if (!(strncasecmp(widget->name, str, len)))
      {
         if (widget->name[len] == '\0')
            return(widget);
         if (!(exact))
         {
            matches++;
            match = widget;
         };
      };

      // check widget aliases for string
      if (widget->aliases == NULL)
         continue;
      for(y = 0; ((widget->aliases[y])); y++)
      {
         alias = widget->aliases[y];
         if (!(strncmp(alias, str, len)))
         {
            if (alias[len] == '\0')
               return(widget);
            if (!(exact))
            {
               matches++;
               match = widget;
            };
         };
      };
   };

   return((matches > 1) ? NULL : match);
}


int
main(
         int                           argc,
         char *                        argv[] )
{
   int               c;
   int               opt_index;
   char              prog_name[512];
   bindle_conf_t     conf;
   bindle_conf_t *   cnf;

   // getopt options
   static const char *  short_opt = "+" BINDLE_COMMON_SHORT;
   static struct option long_opt[] =
   {
      BINDLE_COMMON_LONG,
      { NULL, 0, NULL, 0 }
   };

   memset(&conf, 0, sizeof(bindle_conf_t));
   cnf            = &conf;
   cnf->prog_name = bindle_basename_r(argv[0], prog_name, sizeof(prog_name));
   cnf->argc      = argc;
   cnf->argv      = argv;

   // skip argument processing if called via alias
   if ((cnf->widget = bindle_widget(prog_name, 1)) == NULL)
   {
      opt_index = 0;
      while((c = bindle_getopt(cnf, argc, argv, short_opt, long_opt, &opt_index)) != -1)
      {
         switch(c)
         {
            case -2:       /* --help or --version */
            return(0);

            case -1:       /* no more arguments */
            case 0:        /* long options toggles */
            break;

            case 1:        /* error processing argument */
            return(1);

            case '?':
            fprintf(stderr, "Try `%s --help' for more information.\n", PROGRAM_NAME);
            return(1);

            default:
            fprintf(stderr, "%s: unrecognized option `--%c'\n", PROGRAM_NAME, c);
            fprintf(stderr, "Try `%s --help' for more information.\n", PROGRAM_NAME);
            return(1);
         };
      };
      if ((argc - optind) < 1)
      {
         fprintf(stderr, "%s: missing required argument\n", cnf->prog_name);
         fprintf(stderr, "Try `%s --help' for more information.\n", cnf->prog_name);
         return(1);
      };
      cnf->argc        = (argc - optind);
      cnf->argv        = &argv[optind];
      optind           = 1;
      if ((cnf->widget = bindle_widget(argv[optind], 0)) == NULL)
      {
         fprintf(stderr, "%s: unknown or ambiguous widget -- \"%s\"\n", cnf->prog_name, cnf->argv[0]);
         fprintf(stderr, "Try `%s --help' for more information.\n", cnf->prog_name);
         return(1);
      };
   };

   return(cnf->widget->func_exec(cnf));
}


/* end of source */
