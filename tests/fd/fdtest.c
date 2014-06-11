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
#define __TESTS_FD_FDTEST_C 1

///////////////
//           //
//  Headers  //
//           //
///////////////

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <bindle/fd.h>
#include <bindle/err.h>

#include <stdio.h>
#include <getopt.h>


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#ifdef __BINDLE_PMARK
#pragma mark - Definitions
#endif

#define FDTEST_LOG_REAL       0x01
#define FDTEST_LOG_LOGICAL    0x02
#define FDTEST_LOG_FILE       0x04
#define FDTEST_LOG_CHARS      0x08

#define FDTEST_DEFAULT_LOG    FDTEST_LOG_REAL


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
   int            fdopts;
   int            logopts;
   int            err;
   bindlefd     * bfd;
   size_t         lline;
   size_t         rline;
   size_t         chars;
   ssize_t        len;
   const char   * line;

   static char   short_opt[] = "CfhLRrVw";
   static struct option long_opt[] =
   {
      {"help",          no_argument, 0, 'h'},
      {"version",       no_argument, 0, 'V'},
      {NULL,            0,           0, 0  }
   };

   fdopts  = 0;
   logopts = 0;
   rline   = 0;
   lline   = 0;

   while((c = getopt_long(argc, argv, short_opt, long_opt, &opt_index)) != -1)
   {
      switch(c)
      {
         case -1:	/* no more arguments */
         case 0:	/* long options toggles */
         break;

         case 'C':
         logopts |= FDTEST_LOG_CHARS;
         break;

         case 'f':
         logopts |= FDTEST_LOG_FILE;
         break;

         case 'h':
         my_usage(argv[0]);
         return(0);

         case 'L':
         logopts |= FDTEST_LOG_LOGICAL;
         break;

         case 'R':
         logopts |= FDTEST_LOG_REAL;
         break;

         case 'r':
         fdopts |= BINDLE_FD_STRIP_CR;
         break;

         case 'V':
         printf("%s no version\n", argv[0]);
         return(0);

         case '?':
         fprintf(stderr, "Try `%s --help' for more information.\n", argv[0]);
         return(1);

         default:
         fprintf(stderr, "%s: unrecognized option `--%c'\n",  argv[0], c);
         fprintf(stderr, "Try `%s --help' for more information.\n", argv[0]);
         return(1);
      };
   };
   if (optind == argc)
   {
      fprintf(stderr, "%s: missing required argument\n", argv[0]);
      fprintf(stderr, "Try `%s --help' for more information\n", argv[0]);
      return(1);
   };
   logopts = (logopts == 0) ? FDTEST_LOG_REAL : logopts;

   if ((err = bindle_fdopen(&bfd, argv[optind])) != BINDLE_SUCCESS)
   {
      perror("bindle_fdopen()");
      return(1);
   };

   if (bindle_fdresize(bfd, 1024) != BINDLE_SUCCESS)
   {
      perror("bindle_fdresize()");
      return(1);
   };

   len   = bindle_fdgetline(bfd, &line, &rline, fdopts);
   chars = len;
   while (line != NULL)
   {
      lline++;
      if ((logopts & FDTEST_LOG_FILE) != 0)
         printf("%zu:%s\n", rline, line);
      len    = bindle_fdgetline(bfd, &line, &rline, fdopts);
      chars += len;
   };
   if (len == -1)
   {
      perror("bindle_fdgetline()");
      bindle_fdclose(bfd);
      return(1);
   };

   if ((logopts & FDTEST_LOG_CHARS) != 0)
      printf("chars:   %zu\n", chars);
   if ((logopts & FDTEST_LOG_REAL) != 0)
      printf("real:    %zu\n", rline);
   if ((logopts & FDTEST_LOG_LOGICAL) != 0)
      printf("logical: %zu\n", lline);

   bindle_fdclose(bfd);

   return(0);
}


void my_usage(const char * prog_name)
{
   printf("Usage: %s [OPTIONS] file\n"
         "  -C                        print parsed characters\n"
         "  -f                        print file with line numbers\n"
         "  -h, --help                print this help and exit\n"
         "  -L                        print number of logical lines in file\n"
         "  -R                        print number of real lines in file\n"
         "  _r                        strip carriage returns\n"
         "  -V, --version             print version number and exit\n"
         "  -w                        print number of wrapped lines in file\n"
         "\n",
      prog_name
   );
   return;
}


/* end of source */
