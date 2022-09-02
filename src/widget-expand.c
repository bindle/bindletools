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
#define _SRC_WIDGET_EXPAND_C 1
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
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#pragma mark - Definitions

#define BINDLE_BUFF_SIZE      4096


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#pragma mark - Functions

int
bindle_widget_expand(
         bindle_conf_t *               cnf )
{
   int               c;
   int               i;
   int               opt_index;
   const char *      str;
   char              buff[4096];

   // getopt options
   static const char *  short_opt = BINDLE_COMMON_SHORT "f";
   static struct option long_opt[] =
   {
      BINDLE_COMMON_LONG,
      { NULL, 0, NULL, 0 }
   };

   optind    = 1;
   opt_index = 0;
   str       = NULL;

   while((c = bindle_getopt(cnf, cnf->argc, cnf->argv, short_opt, long_opt, &opt_index)) != -1)
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
   if ((cnf->argc - optind) < 1)
   {
      fprintf(stderr, "%s: missing required argument\n", cnf->prog_name);
      fprintf(stderr, "Try `%s --help' for more information.\n", cnf->prog_name);
      return(1);
   };

   for(i = optind; (i < cnf->argc); i++)
   {
      if (!(bindle_strexpand(buff, cnf->argv[i], sizeof(buff), 1)))
      {
         fprintf(stderr, "%s: bindle_strexpand(): unknown error\n", cnf->prog_name);
         return(1);
      };
      printf("%s\n", buff);
   };

   return(0);
}


int
bindle_widget_expand_usage(
         bindle_conf_t *               cnf )
{
   assert(cnf != NULL);
   printf("TOKENS:\n");
   printf("  %%%%     a literal percent\n");
   printf("  %%D     domain name\n");
   printf("  %%d     home directory\n");
   printf("  %%G     group ID\n");
   printf("  %%g     group name\n");
   printf("  %%H     hostname with domain name\n");
   printf("  %%h     hostname without domain name\n");
   printf("  %%P     process ID\n");
   printf("  %%p     process name\n");
   printf("  %%U     user ID\n");
   printf("  %%u     username\n");
   printf("  \\\\    expands to backslash\n");
   printf("  \\'    expands to single quote\n");
   printf("  \\?    expands to question mark\n");
   printf("  \\n    expands to newline\n");
   printf("  \\r    expands to carriage return\n");
   printf("  \\t    expands to horizontal tab\n");
   printf("  \\v    expands to vertical tab\n");
   printf("  \\xnn  expands to a byte with a hexadecimal value xx\n");
   printf("  \\nnn  expands to a byte with an octal value nnn\n");
   return(0);
}

/* end of source */
