/*
 *  Bindle Binaries Tools
 *  Copyright (C) 2014 Bindle Binaries <syzdek@bindlebinaries.com>.
 *
 *  @BINDLE_BINARIES_BSD_LICENSE_START@
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
 *     * Neither the name of Bindle Binaries nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 *  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL BINDLE BINARIES BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 *  SUCH DAMAGE.
 *
 *  @BINDLE_BINARIES_BSD_LICENSE_END@
 */
#define __TESTS_CNF_CNFTEST_C 1

///////////////
//           //
//  Headers  //
//           //
///////////////

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <bindle/cnf.h>
#include <bindle/err.h>
#include <bindle/log.h>

#include <stdio.h>
#include <getopt.h>


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#ifdef __BINDLE_PMARK
#pragma mark - Prototypes
#endif

int main(int argc, char * argv[]);
void my_usage(const char * prog_name);


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#ifdef __BINDLE_PMARK
#pragma mark - Functions
#endif

int main(int argc, char * argv[])
{
   int            c;
   int            opt_index;
   int            cnfopts;
   int            err;
   size_t         line;
   int            x;
   const char  ** args;
   bindlecnf    * bcnf;
   char         * prog_name;
   const char   * types;

   static char   short_opt[] = "DdehimopQSsV";
   static struct option long_opt[] =
   {
      {"help",          no_argument, 0, 'h'},
      {"version",       no_argument, 0, 'V'},
      {NULL,            0,           0, 0  }
   };

   prog_name = bindle_basename(argv[0], NULL);
   cnfopts  = 0;

   while((c = getopt_long(argc, argv, short_opt, long_opt, &opt_index)) != -1)
   {
      switch(c)
      {
         case -1:	/* no more arguments */
         case 0:	/* long options toggles */
         break;

         case 'D':
         cnfopts |= BINDLE_CNF_QUOTE_DOULE;
         break;

         case 'd':
         cnfopts |= BINDLE_CNF_DEBUG;
         break;

         case 'e':
         cnfopts |= BINDLE_CNF_ESCAPES;
         break;

         case 'h':
         my_usage(prog_name);
         return(0);

         case 'i':
         cnfopts |= BINDLE_CNF_COMMENT_INI;
         break;

         case 'm':
         cnfopts |= BINDLE_CNF_COMMENT_M4;
         break;

         case 'o':
         cnfopts |= BINDLE_CNF_ONESTR;
         break;

         case 'p':
         cnfopts |= BINDLE_CNF_COMMENT_CPP;
         break;

         case 'Q':
         cnfopts |= BINDLE_CNF_QUOTE_DOULE;
         cnfopts |= BINDLE_CNF_QUOTE_SINGLE;
         break;

         case 'S':
         cnfopts |= BINDLE_CNF_QUOTE_SINGLE;
         break;

         case 's':
         cnfopts |= BINDLE_CNF_COMMENT_SH;
         break;

         case 'V':
         printf("%s no version\n", prog_name);
         return(0);

         case '?':
         fprintf(stderr, "Try `%s --help' for more information.\n", prog_name);
         return(1);

         default:
         fprintf(stderr, "%s: unrecognized option `--%c'\n",  prog_name, c);
         fprintf(stderr, "Try `%s --help' for more information.\n", prog_name);
         return(1);
      };
   };

   if (optind == argc)
   {
      fprintf(stderr, "%s: missing required argument\n", prog_name);
      fprintf(stderr, "Try `%s --help' for more information\n", prog_name);
      return(1);
   };

   if ((err = bindle_cnfopen(&bcnf, argv[optind], cnfopts)) != BINDLE_SUCCESS)
   {
      printf("%s: bindle_cnfopen(%s): %s\n", prog_name, argv[optind], bindle_strerror(err));
      return(1);
   };

   while ((err = bindle_cnfread(bcnf, &argc, &args, &types, &line)) == 0)
   {
      printf("line %zu (%u args) (%s)\n", line, argc, types);
      for(x = 0; x < argc; x++)
         printf("   arg %i: %s\n", x, args[x]);
      printf("\n");
   };

   if (err == -1)
   {
      printf("%s: bindle_cnfread(): %s\n", prog_name, bindle_strerror(err));
      bindle_cnfclose(bcnf);
      return(1);
   };

   bindle_cnfclose(bcnf);

   return(0);
}


void my_usage(const char * prog_name)
{
   printf("Usage: %s [OPTIONS] file\n"
         "  -D                        honor double quotes\n"
         "  -d                        enable debug output\n"
         "  -e                        allow backslashes to escape characters\n"
         "  -h, --help                print this help and exit\n"
         "  -i                        allow INI file style comment characters\n"
         "  -m                        allow M4 file style comment characters\n"
         "  -o                        do not split line\n"
         "  -p                        allow C++ style comment characters\n"
         "  -Q                        honor single and double quotes\n"
         "  -S                        honor single quotes\n"
         "  -s                        allow shell style comment characters\n"
         "  -V, --version             print version number and exit\n"
         "\n",
      prog_name
   );
   return;
}


/* end of source */
