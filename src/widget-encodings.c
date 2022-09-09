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
#define _SRC_WIDGET_BASE32_C 1
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
#include <errno.h>


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#pragma mark - Definitions

#define BINDLE_BUFF_SIZE      (40*3)

#define BINDLE_FLG_ENCODE     0x00001UL
#define BINDLE_FLG_DECODE     0x00002UL
#define BINDLE_FLG_NOPAD      0x00004UL
#define BINDLE_FLG_NEWLINE    0x00008UL


/////////////////
//             //
//  Variables  //
//             //
/////////////////
#pragma mark - Variables

#pragma mark bindle_widget_encodings_options[]
const char * const bindle_widget_encodings_options[] =
{
   "  -d, --decode              decode string",
   "  -e, --encode              encode string",
   "  -n, --newline             add newline to output",
   "  -p, --nopadding           disable padding",
   "  -s string                 string to encode or decode",
   NULL
};


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#pragma mark - Prototypes

static int
bindle_widget_encodings(
         bindle_conf_t *               cnf,
         int                           method );


static int
bindle_widget_encodings_stdin(
         bindle_conf_t *               cnf,
         int                           method );


static int
bindle_widget_encodings_string(
         bindle_conf_t *               cnf,
         int                           method,
         const char *                  str );


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#pragma mark - Functions

int
bindle_widget_base16(
         bindle_conf_t *               cnf )
{
   return(bindle_widget_encodings(cnf, BNDL_HEX));
}


int
bindle_widget_base32(
         bindle_conf_t *               cnf )
{
   return(bindle_widget_encodings(cnf, BNDL_BASE32));
}


int
bindle_widget_base32hex(
         bindle_conf_t *               cnf )
{
   return(bindle_widget_encodings(cnf, BNDL_BASE32HEX));
}


int
bindle_widget_base64(
         bindle_conf_t *               cnf )
{
   return(bindle_widget_encodings(cnf, BNDL_BASE64));
}


int
bindle_widget_crockfordbase32(
         bindle_conf_t *               cnf )
{
   return(bindle_widget_encodings(cnf, BNDL_CROCKFORD));
}


int
bindle_widget_pctenc(
         bindle_conf_t *               cnf )
{
   return(bindle_widget_encodings(cnf, BNDL_PCTENC));
}


int
bindle_widget_encodings(
         bindle_conf_t *               cnf,
         int                           method )
{
   int               c;
   int               opt_index;
   const char *      str;

   // getopt options
   static const char *  short_opt = BINDLE_COMMON_SHORT "denps:";
   static struct option long_opt[] =
   {
      BINDLE_COMMON_LONG,
      { "decode",          no_argument,      NULL, 'd' },
      { "encode",          no_argument,      NULL, 'e' },
      { "newline",         no_argument,      NULL, 'n' },
      { "nopadding",       no_argument,      NULL, 'p' },
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

         case 'd':
         cnf->widget_flags |=  BINDLE_FLG_DECODE;
         cnf->widget_flags &= ~BINDLE_FLG_ENCODE;
         break;

         case 'e':
         cnf->widget_flags |=  BINDLE_FLG_ENCODE;
         cnf->widget_flags &= ~BINDLE_FLG_DECODE;
         break;

         case 'n':
         cnf->widget_flags |= BINDLE_FLG_NEWLINE;
         break;

         case 'p':
         cnf->widget_flags |= BINDLE_FLG_NOPAD;
         break;

         case 's':
         str = optarg;
         break;

         case 'u':
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
   if ((cnf->argc - optind) > 0)
   {
      fprintf(stderr, "%s: unknown argument -- `%s'\n", cnf->prog_name, cnf->argv[optind]);
      fprintf(stderr, "Try `%s --help' for more information.\n", cnf->prog_name);
      return(1);
   };

   fflush(stdout);

   if ((str))
      return(bindle_widget_encodings_string(cnf, method, str));
   return(bindle_widget_encodings_stdin(cnf, method));
}


int
bindle_widget_encodings_stdin(
         bindle_conf_t *               cnf,
         int                           method )
{
   ssize_t     rc;
   ssize_t     len;
   char        chunk[BINDLE_BUFF_SIZE+1];
   char        res[(BINDLE_BUFF_SIZE*3)+1];

   while ((rc = read(STDIN_FILENO, chunk, BINDLE_BUFF_SIZE)) > 0)
   {
      chunk[rc] = '\0';
      if ((cnf->widget_flags & BINDLE_FLG_DECODE) == BINDLE_FLG_DECODE)
         len = bindle_decode(method, res, sizeof(res), chunk, (size_t)rc);
      else
         len = bindle_encode(method, res, sizeof(res), chunk, (size_t)rc, (cnf->widget_flags & BINDLE_FLG_NOPAD));
      if (len > 0)
         write(STDOUT_FILENO, res, (size_t)len);
   };
   if (rc == -1)
   {
      fprintf(stderr, "%s: read(): %s\n", cnf->prog_name, strerror(errno));
      return(1);
   };

   if ((cnf->widget_flags & BINDLE_FLG_NEWLINE))
   {
      bindle_strlcpy(res, "\n", sizeof(res));
      write(STDOUT_FILENO, res, strlen(res));
   };

   return(0);
}


int
bindle_widget_encodings_string(
         bindle_conf_t *               cnf,
         int                           method,
         const char *                  str )
{
   ssize_t     rc;
   size_t      len;
   size_t      off;
   size_t      size;
   char        chunk[BINDLE_BUFF_SIZE+1];
   char        res[(BINDLE_BUFF_SIZE*3)+1];

   len = strlen(str);

   if ((cnf->widget_flags & BINDLE_FLG_DECODE) == BINDLE_FLG_DECODE)
   {
      for(off = 0; (off < len); off += BINDLE_BUFF_SIZE)
      {
         size = ((off+BINDLE_BUFF_SIZE) < len) ? BINDLE_BUFF_SIZE : (len-off);
         memcpy(chunk, &str[off], size);
         if ((rc = bindle_decode(method, res, sizeof(res), chunk, size)) > 0)
            write(STDOUT_FILENO, res, (size_t)rc);
      };
   }
   else
   {
      for(off = 0; (off < len); off += BINDLE_BUFF_SIZE)
      {
         size = ((off+BINDLE_BUFF_SIZE) < len) ? BINDLE_BUFF_SIZE : (len-off);
         memcpy(chunk, &str[off], size);
         if ((rc = bindle_encode(method, res, sizeof(res), chunk, size, (cnf->widget_flags & BINDLE_FLG_NOPAD))) > 0)
            write(STDOUT_FILENO, res, (size_t)rc);
      };
   };

   printf("\n");

   return(0);
}


/* end of source */
