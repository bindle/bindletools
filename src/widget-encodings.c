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
#define BINDLE_FLG_IGNOREBAD  0x00010UL

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
   size_t                     line_wrap;
   size_t                     line_off;
   char *                     buff;
   char *                     res;
   FILE *                     fsin;
   FILE *                     fsout;
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
   "  -I, --ignore-garbage      ignore invalid characters when decoding",
   "  -i file, --input=file     use input file instead of stdin",
   "  -n, --newline             add newline to output",
   "  -o file, --output=file    use output file instead of stdout",
   "  -p, --nopadding           disable padding",
   "  -s string                 string to encode or decode",
   "  -w cols, --wrap=cols      wrap encoded lines to cols",
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
   char *            endptr;
   const char *      str;

   // getopt options
   static const char *  short_opt = BINDLE_COMMON_SHORT "b:deIi:no:ps:w:";
   static struct option long_opt[] =
   {
      BINDLE_COMMON_LONG,
      { "break",          no_argument,          NULL, 'b' },
      { "decode",          no_argument,         NULL, 'd' },
      { "encode",          no_argument,         NULL, 'e' },
      { "ignore-garbage",   no_argument,         NULL, 'I' },
      { "input",           optional_argument,   NULL, 'i' },
      { "newline",         no_argument,         NULL, 'n' },
      { "nopadding",       no_argument,         NULL, 'p' },
      { "output",          optional_argument,   NULL, 'o' },
      { "wrap",            optional_argument,   NULL, 'w' },
      { NULL, 0, NULL, 0 }
   };

   optind                  = 1;
   opt_index               = 0;
   str                     = NULL;
   cnf->state->fsin        = NULL;
   cnf->state->fsout       = NULL;

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

         case 'I':
         cnf->widget_flags |= BINDLE_FLG_IGNOREBAD;
         break;

         case 'i':
         if (cnf->state->fsin != NULL)
         {
            fprintf(stderr, "%s: duplicate option `--%c'\n", PROGRAM_NAME, c);
            fprintf(stderr, "Try `%s --help' for more information.\n", PROGRAM_NAME);
            return(1);
         };
         if ((cnf->state->fsin = fopen(optarg, "r")) == NULL)
         {
            fprintf(stderr, "%s: fopen(): %s: %s\n", cnf->prog_name, optarg, strerror(errno));
            return(1);
         };
         break;

         case 'n':
         cnf->widget_flags |= BINDLE_FLG_NEWLINE;
         break;

         case 'o':
         if (cnf->state->fsout != NULL)
         {
            fprintf(stderr, "%s: duplicate option `--%c'\n", PROGRAM_NAME, c);
            fprintf(stderr, "Try `%s --help' for more information.\n", PROGRAM_NAME);
            return(1);
         };
         if ((cnf->state->fsout = fopen(optarg, "w")) == NULL)
         {
            fprintf(stderr, "%s: fopen(): %s: %s\n", cnf->prog_name, optarg, strerror(errno));
            return(1);
         };
         break;

         case 'p':
         cnf->widget_flags |= BINDLE_FLG_NOPAD;
         break;

         case 's':
         str = optarg;
         break;

         case 'b':
         case 'w':
         cnf->widget_flags |= BINDLE_FLG_NEWLINE;
         cnf->state->line_wrap = strtoul(optarg, &endptr, 0);
         if ((endptr == optarg) || (endptr[0] != '\0'))
         {
            fprintf(stderr, "%s: invalid value for option `-%c'\n", PROGRAM_NAME, c);
            fprintf(stderr, "Try `%s --help' for more information.\n", PROGRAM_NAME);
            return(1);
         };
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
   cnf->state->fsout = (cnf->state->fsout == NULL) ? stdout : cnf->state->fsout;
   cnf->state->fsin  = (cnf->state->fsin  == NULL) ? stdin  : cnf->state->fsin;

   if ((str))
      rc = bindle_encodings_src_string(cnf, method, str);
   else
      rc = bindle_encodings_src_fileno(cnf, method);

   if ((cnf->widget_flags & BINDLE_FLG_NEWLINE))
      fprintf(cnf->state->fsout, "\n");

   fflush(cnf->state->fsout);

   if (cnf->state->fsout != stdout)
      fclose(cnf->state->fsout);
   if (cnf->state->fsin != stdin)
      fclose(cnf->state->fsin);

   return(rc);
}


int
bindle_encodings_action_decode(
         bindle_conf_t *               cnf,
         int                           method )
{
   char                 c;
   ssize_t              len;
   bindle_state_t *     state;
   size_t               pos;
   size_t               off;
   size_t               buff_len;

   state       = cnf->state;
   buff_len    = state->buff_len;

   // strip newlines
   for(pos = 0, off = 0; (pos < state->buff_len); pos++)
   {
      c = state->buff[pos];
      if ( (c == '\n') || (c == '\r') )
      {
         off++;
         continue;
      };
      if ( ((cnf->widget_flags & BINDLE_FLG_IGNOREBAD)) && (bindle_encoded_char(method, c) == -1) )
      {
         off++;
         continue;
      };
      if (off != 0)
         state->buff[pos-off] = state->buff[pos];
   };
   state->buff_len -= off;
   if (buff_len != state->buff_len)
      return(0);

   // decode data
   if ((len = bindle_decode(method, state->res, state->res_size, state->buff, buff_len)) == -1)
   {
      fprintf(stderr, "%s: bindle_decode(): %s\n", cnf->prog_name, strerror(errno));
      return(-1);
   };
   if (len == 0)
      return(0);
   fwrite(state->res, 1, (size_t)len, state->fsout);
   state->buff_len = 0;

   return(0);
}


int
bindle_encodings_action_encode(
         bindle_conf_t *               cnf,
         int                           method )
{
   ssize_t              len;
   ssize_t              chunk_len;
   ssize_t              buff_off;
   bindle_state_t *     state;

   state = cnf->state;

   if ((len = bindle_encode(method, state->res, state->res_size, state->buff, state->buff_len, (cnf->widget_flags & BINDLE_FLG_NOPAD))) < 0)
   {
      fprintf(stderr, "%s: %s\n", cnf->prog_name, strerror(errno));
      return(-1);
   };
   state->buff_len = 0;

   if (!(state->line_wrap))
   {
      if (fwrite(state->res, 1, (size_t)len, state->fsout) == 0)
      {
         fprintf(stderr, "%s: %s\n", cnf->prog_name, strerror(errno));
         return(-1);
      };
      return(0);
   };

   buff_off = 0;
   while(buff_off < len)
   {
      chunk_len = (ssize_t)(state->line_wrap - state->line_off);
      chunk_len = (len < (ssize_t)chunk_len)    ? len            : chunk_len;
      chunk_len = (chunk_len > (len-buff_off)) ? (len-buff_off) : chunk_len;
      if (fwrite(&cnf->state->res[buff_off], 1, (size_t)chunk_len, state->fsout) == 0)
      {
         fprintf(stderr, "%s: %s\n", cnf->prog_name, strerror(errno));
         return(-1);
      };
      buff_off += chunk_len;
      state->line_off += (size_t)chunk_len;
      if (state->line_off == state->line_wrap)
      {
         state->line_off = 0;
         fprintf(state->fsout, "\n");
      };
   };

   return(0);
}


int
bindle_encodings_src_fileno(
         bindle_conf_t *               cnf,
         int                           method )
{
   size_t            len;
   bindle_state_t *  state;
   char *            buff;

   state             = cnf->state;
   state->buff_len    = 0;
   state->line_off   = 0;
   buff              = state->buff;

   while ((len = fread(&buff[state->buff_len], 1, (state->buff_size-state->buff_len), state->fsin)) >= 0)
   {
      state->buff_len += (size_t)len;
      if (state->buff_len == 0)
         return(0);
      buff[state->buff_len] = '\0';
      if ((cnf->widget_flags & BINDLE_FLG_DECODE) == BINDLE_FLG_DECODE)
         if (bindle_encodings_action_decode(cnf, method) == -1)
            return(1);
      if ((cnf->widget_flags & BINDLE_FLG_DECODE) != BINDLE_FLG_DECODE)
         if (bindle_encodings_action_encode(cnf, method) == -1)
            return(1);
      if (len == 0)
         return(0);
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
            fwrite(res, 1, (size_t)rc, cnf->state->fsout);
      };
   }
   else
   {
      for(off = 0; (off < len); off += BINDLE_BUFF_SIZE)
      {
         size = ((off+BINDLE_BUFF_SIZE) < len) ? BINDLE_BUFF_SIZE : (len-off);
         memcpy(chunk, &str[off], size);
         if ((rc = bindle_encode(method, res, sizeof(res), chunk, size, (cnf->widget_flags & BINDLE_FLG_NOPAD))) > 0)
            fwrite(res, 1, (size_t)rc, cnf->state->fsout);
      };
   };

   return(0);
}


/* end of source */
