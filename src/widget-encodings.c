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
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>


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

#define BNDLE_B16_CHUNK       2
#define BNDLE_B16_BUFF_LEN    (BNDLE_B16_CHUNK*32)
#define BNDLE_B16_RES_LEN     (BNDLE_B16_CHUNK*64)
#define BNDLE_B32_CHUNK       40
#define BNDLE_B32_BUFF_LEN    (BNDLE_B32_CHUNK*5)
#define BNDLE_B32_RES_LEN     (BNDLE_B32_CHUNK*8)
#define BNDLE_B64_CHUNK       12
#define BNDLE_B64_BUFF_LEN    (BNDLE_B64_CHUNK*18)
#define BNDLE_B64_RES_LEN     (BNDLE_B64_CHUNK*24)
#define BNDLE_PCTENC_CHUNK    2048
#define BNDLE_PCTENC_BUFF_LEN BNDLE_PCTENC_CHUNK
#define BNDLE_PCTENC_RES_LEN  BNDLE_PCTENC_CHUNK


//////////////////
//              //
//  Data Types  //
//              //
//////////////////
#pragma mark - Data Types

struct bindle_state
{
   size_t                     chunk_size;
   size_t                     res_size;
   size_t                     buff_size;
   size_t                     buff_len;
   char *                     buff;
   char *                     res;
   int                        fdin;
   int                        fdout;
};


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
   "  -i file, --input=file     use input file instead of stdin",
   "  -n, --newline             add newline to output",
   "  -o file, --output=file    use output file instead of stdout",
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
bindle_encodings_action_decode(
         bindle_conf_t *               cnf,
         int                           method );


static int
bindle_encodings_action_encode(
         bindle_conf_t *               cnf,
         int                           method );


static int
bindle_encodings_src_fileno(
         bindle_conf_t *               cnf,
         int                           method );


static int
bindle_encodings_src_string(
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
   bindle_state_t    state;
   char              res[BNDLE_B16_RES_LEN+1];
   char              buff[BNDLE_B16_BUFF_LEN+1];
   memset(&state, 0, sizeof(state));
   cnf->state        = &state;
   state.chunk_size  = BNDLE_B16_CHUNK;
   state.buff_size   = BNDLE_B16_BUFF_LEN;
   state.res_size    = BNDLE_B16_RES_LEN;
   state.buff        = buff;
   state.res         = res;
   return(bindle_widget_encodings(cnf, BNDL_HEX));
}


int
bindle_widget_base32(
         bindle_conf_t *               cnf )
{
   bindle_state_t    state;
   char              res[BNDLE_B32_RES_LEN+1];
   char              buff[BNDLE_B32_BUFF_LEN+1];
   memset(&state, 0, sizeof(state));
   cnf->state        = &state;
   state.chunk_size  = BNDLE_B32_CHUNK;
   state.buff_size   = BNDLE_B32_BUFF_LEN;
   state.res_size    = BNDLE_B32_RES_LEN;
   state.buff        = buff;
   state.res         = res;
   return(bindle_widget_encodings(cnf, BNDL_BASE32));
}


int
bindle_widget_base32hex(
         bindle_conf_t *               cnf )
{
   bindle_state_t    state;
   char              res[BNDLE_B32_RES_LEN+1];
   char              buff[BNDLE_B32_BUFF_LEN+1];
   memset(&state, 0, sizeof(state));
   cnf->state        = &state;
   state.chunk_size  = BNDLE_B32_CHUNK;
   state.buff_size   = BNDLE_B32_BUFF_LEN;
   state.res_size    = BNDLE_B32_RES_LEN;
   state.buff        = buff;
   state.res         = res;
   return(bindle_widget_encodings(cnf, BNDL_BASE32HEX));
}


int
bindle_widget_base64(
         bindle_conf_t *               cnf )
{
   bindle_state_t    state;
   char              res[BNDLE_B64_RES_LEN+1];
   char              buff[BNDLE_B64_BUFF_LEN+1];
   memset(&state, 0, sizeof(state));
   cnf->state        = &state;
   state.chunk_size  = BNDLE_B64_CHUNK;
   state.buff_size   = BNDLE_B64_BUFF_LEN;
   state.res_size    = BNDLE_B64_RES_LEN;
   state.buff        = buff;
   state.res         = res;
   return(bindle_widget_encodings(cnf, BNDL_BASE64));
}


int
bindle_widget_crockfordbase32(
         bindle_conf_t *               cnf )
{
   bindle_state_t    state;
   char              res[BNDLE_B32_RES_LEN+1];
   char              buff[BNDLE_B32_BUFF_LEN+1];
   memset(&state, 0, sizeof(state));
   cnf->state        = &state;
   state.chunk_size  = BNDLE_B32_CHUNK;
   state.buff_size   = BNDLE_B32_BUFF_LEN;
   state.res_size    = BNDLE_B32_RES_LEN;
   state.buff        = buff;
   state.res         = res;
   return(bindle_widget_encodings(cnf, BNDL_CROCKFORD));
}


int
bindle_widget_pctenc(
         bindle_conf_t *               cnf )
{
   bindle_state_t    state;
   char              res[BNDLE_PCTENC_RES_LEN+1];
   char              buff[BNDLE_PCTENC_BUFF_LEN+1];
   memset(&state, 0, sizeof(state));
   cnf->state        = &state;
   state.chunk_size  = BNDLE_PCTENC_CHUNK;
   state.buff_size   = BNDLE_PCTENC_BUFF_LEN;
   state.res_size    = BNDLE_PCTENC_RES_LEN;
   state.buff        = buff;
   state.res         = res;
   return(bindle_widget_encodings(cnf, BNDL_PCTENC));
}


int
bindle_widget_encodings(
         bindle_conf_t *               cnf,
         int                           method )
{
   int               c;
   int               rc;
   int               opt_index;
   const char *      str;

   // getopt options
   static const char *  short_opt = BINDLE_COMMON_SHORT "dei:no:ps:";
   static struct option long_opt[] =
   {
      BINDLE_COMMON_LONG,
      { "decode",          no_argument,         NULL, 'd' },
      { "encode",          no_argument,         NULL, 'e' },
      { "input",           optional_argument,   NULL, 'i' },
      { "newline",         no_argument,         NULL, 'n' },
      { "nopadding",       no_argument,         NULL, 'p' },
      { "output",          optional_argument,   NULL, 'o' },
      { NULL, 0, NULL, 0 }
   };

   optind            = 1;
   opt_index         = 0;
   str               = NULL;
   cnf->state->fdin  = STDIN_FILENO;
   cnf->state->fdout = STDOUT_FILENO;

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

         case 'i':
         if (cnf->state->fdin != STDIN_FILENO)
         {
            fprintf(stderr, "%s: duplicate option `--%c'\n", PROGRAM_NAME, c);
            fprintf(stderr, "Try `%s --help' for more information.\n", PROGRAM_NAME);
            return(1);
         };
         if ((cnf->state->fdin = open(optarg, O_RDONLY)) == -1)
         {
            fprintf(stderr, "%s: open(): %s: %s\n", cnf->prog_name, optarg, strerror(errno));
            return(1);
         };
         break;

         case 'n':
         cnf->widget_flags |= BINDLE_FLG_NEWLINE;
         break;

         case 'o':
         if (cnf->state->fdout != STDOUT_FILENO)
         {
            fprintf(stderr, "%s: duplicate option `--%c'\n", PROGRAM_NAME, c);
            fprintf(stderr, "Try `%s --help' for more information.\n", PROGRAM_NAME);
            return(1);
         };
         if ((cnf->state->fdout = open(optarg, (O_WRONLY|O_CREAT|O_TRUNC), 0644)) == -1)
         {
            fprintf(stderr, "%s: open(): %s: %s\n", cnf->prog_name, optarg, strerror(errno));
            return(1);
         };
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
      rc = bindle_encodings_src_string(cnf, method, str);
   else
      rc = bindle_encodings_src_fileno(cnf, method);

   if ((cnf->widget_flags & BINDLE_FLG_NEWLINE))
      write(cnf->state->fdout, "\n", strlen("\n"));

   return(rc);
}


int
bindle_encodings_action_decode(
         bindle_conf_t *               cnf,
         int                           method )
{
   ssize_t              len;
   bindle_state_t *     state;
   size_t               pos;
   size_t               off;
   size_t               chunk_size;
   int                  fullbuff;

   state = cnf->state;

   fullbuff = (state->buff_size == state->buff_len) ? BNDL_YES : BNDL_NO;

   // strip newlines
   for(pos = 0, off = 0; (pos < state->buff_len); pos++)
   {
      if ( (state->buff[pos] == '\n') || (state->buff[pos] == '\r') )
      {
         off++;
         continue;
      };
      if (off != 0)
         state->buff[pos-off] = state->buff[pos];
   };
   state->buff_len -= off;
   state->buff[state->buff_len] = '\0';

   // decode data
   chunk_size = state->buff_len;
   if (fullbuff == BNDL_YES)
   {
      chunk_size = (state->buff_len / state->chunk_size) * state->chunk_size;
      if ( (chunk_size > state->buff_len) || (chunk_size < state->chunk_size) )
         return(0);
   };
   if ((len = bindle_decode(method, state->res, state->res_size, state->buff, chunk_size)) == -1)
   {
      fprintf(stderr, "%s: bindle_decode(): %s\n", cnf->prog_name, strerror(errno));
      return(-1);
   };
   if (len == 0)
      return(0);
   write(state->fdout, state->res, (size_t)len);

   // shift unencoded data
   for(pos = chunk_size; (pos < state->buff_len); pos++)
      state->buff[pos-chunk_size] = state->buff[pos];
   state->buff_len -= chunk_size;

   return(0);
}


int
bindle_encodings_action_encode(
         bindle_conf_t *               cnf,
         int                           method )
{
   ssize_t              len;
   bindle_state_t *     state;

   state = cnf->state;

   if ((len = bindle_encode(method, state->res, state->res_size, state->buff, state->buff_len, (cnf->widget_flags & BINDLE_FLG_NOPAD))) == -1)
   {
      fprintf(stderr, "%s: %s\n", cnf->prog_name, strerror(errno));
      return(-1);
   };
   if (len > 0)
      write(cnf->state->fdout, cnf->state->res, (size_t)len);

   return(0);
}


int
bindle_encodings_src_fileno(
         bindle_conf_t *               cnf,
         int                           method )
{
   ssize_t           len;
   bindle_state_t *  state;
   char *            buff;
   char *            res;

   state             = cnf->state;
   state->buff_len    = 0;
   buff              = state->buff;
   res               = state->res;

   while ((len = read(cnf->state->fdin, &buff[state->buff_len], (state->buff_size-state->buff_len))) > 0)
   {
      state->buff_len += (size_t)len;
      buff[state->buff_len] = '\0';
      if ((cnf->widget_flags & BINDLE_FLG_DECODE) == BINDLE_FLG_DECODE)
      {
         if (bindle_encodings_action_decode(cnf, method) == -1)
            return(1);
      }
      else
      {
         if (bindle_encodings_action_encode(cnf, method) == -1)
            return(1);
      }
   };
   if (len == -1)
   {
      fprintf(stderr, "%s: read(): %s\n", cnf->prog_name, strerror(errno));
      return(1);
   };

   return(0);
}


int
bindle_encodings_src_string(
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
            write(cnf->state->fdout, res, (size_t)rc);
      };
   }
   else
   {
      for(off = 0; (off < len); off += BINDLE_BUFF_SIZE)
      {
         size = ((off+BINDLE_BUFF_SIZE) < len) ? BINDLE_BUFF_SIZE : (len-off);
         memcpy(chunk, &str[off], size);
         if ((rc = bindle_encode(method, res, sizeof(res), chunk, size, (cnf->widget_flags & BINDLE_FLG_NOPAD))) > 0)
            write(cnf->state->fdout, res, (size_t)rc);
      };
   };

   return(0);
}


/* end of source */
