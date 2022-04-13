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
#define __LIB_LIBBINDLE_CNF_C 1

///////////////
//           //
//  Headers  //
//           //
///////////////

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <bindle/cnf.h>
#include <bindle/fd.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <bindle/err.h>


///////////////
//           //
//  Structs  //
//           //
///////////////
#ifdef __BINDLE_PMARK
#pragma mark - Structs
#endif

struct bindle_cnf_struct
{
   int              opts;
   bindlefd       * fd;          ///< pointer to bindlefd
   char           * filename;    ///< name of config file
   char           * buff;
   char          ** argv;
   int              argc;
   int              args;
   char           * argt;
};


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#ifdef __BINDLE_PMARK
#pragma mark - Prototypes
#endif

int bindle_cnfargvadd(bindlecnf * bcnf, char * line, int type);
ssize_t bindle_cnfcomments(char * line, ssize_t * plen, int opts);
ssize_t bindle_cnfwhitespace(char ** pline, ssize_t * plen);


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#ifdef __BINDLE_PMARK
#pragma mark - Functions
#endif

int bindle_cnfargvadd(bindlecnf * bcnf, char * line, int type)
{
   size_t   size;
   void   * ptr;

   assert(bcnf != NULL);
   assert(line != NULL);
   assert(type != 0);

   if ((bcnf->argc + 2) > bcnf->args)
   {
      // increases argv buffer size
      size = sizeof(char *) * (bcnf->argc + 2);
      if ((ptr = realloc(bcnf->argv, size)) == NULL)
         return(errno = ENOMEM);
      bcnf->argv = ptr;

      // increases argv buffer size
      size = sizeof(char) * (bcnf->argc + 2);
      if ((ptr = realloc(bcnf->argt, size)) == NULL)
         return(errno = ENOMEM);
      bcnf->argt = ptr;

      bcnf->args = (bcnf->argc + 2);
   };

   bcnf->argv[bcnf->argc] = line;
   bcnf->argt[bcnf->argc] = type;
   bcnf->argc++;
   bcnf->argv[bcnf->argc] = NULL;
   bcnf->argt[bcnf->argc] = '\0';

   return(BINDLE_SUCCESS);
}


void bindle_cnfclose(bindlecnf * bcnf)
{
   if (bcnf == NULL)
      return;

   bindle_fdclose(bcnf->fd);
   free(bcnf->filename);
   free(bcnf);

   return;
}


ssize_t bindle_cnfcomments(char * line, ssize_t * plen, int opts)
{
   ssize_t pos;

   assert(line != NULL);
   assert(plen != NULL);

   for(pos = 0; line[pos] != '\0'; pos++)
   {
      switch(line[pos])
      {
         // BINDLE_CNF_COMMENT_CPP
         case '/':
         if ((opts & BINDLE_CNF_COMMENT_CPP) == 0)
            break;
         if (line[pos+1] != '/')
            break;
         line[pos] = '\0';
         *plen = pos;
         return(0);

         // BINDLE_CNF_ESCAPES
         case '\\':
         if ((opts & BINDLE_CNF_ESCAPES) == 0)
            break;
         pos++;
         break;

         // BINDLE_CNF_COMMENT_INI
         case ';':
         if ((opts & BINDLE_CNF_COMMENT_INI) == 0)
            break;
         line[pos] = '\0';
         *plen = pos;
         return(0);

         // BINDLE_CNF_COMMENT_M4
         case 'd':
         if ((opts & BINDLE_CNF_COMMENT_M4) == 0)
            break;
         if (strncasecmp(&line[pos], "dnl", 3) != 0)
            break;
         line[pos] = '\0';
         *plen = pos;
         return(0);

         // BINDLE_CNF_COMMENT_SH
         case '#':
         if ((opts & BINDLE_CNF_COMMENT_SH) == 0)
            break;
         line[pos] = '\0';
         *plen = pos;
         return(0);

         // groups single quotes
         case '\'':
         pos++;
         while((line[pos] != '\'') && (line[pos] != '\0'))
            pos++;
         break;

         default:
         break;
      };
   };

   *plen = pos;

   return(0);
}


int bindle_cnfopen(bindlecnf ** pbcnf, const char * filename, uint32_t opts)
{
   int         err;
   bindlecnf * bcnf;
   size_t      size;

   assert(pbcnf != NULL);
   assert(filename != NULL);

   *pbcnf = NULL;

   if (opts == 0)
      opts = BINDLE_CNF_DEFAULT;

   if ((bcnf = malloc(sizeof(bindlecnf))) == NULL)
      return(errno);
   memset(bcnf, 0, sizeof(bindlecnf));
   bcnf->opts = opts;

   bcnf->args = 2;

   size = sizeof(char *) * (bcnf->args);
   if ((bcnf->argv = malloc(size)) == NULL)
   {
      bindle_cnfclose(bcnf);
      return(errno);
   };
   bzero(bcnf->argv, size);

   size = sizeof(char) * (bcnf->args);
   if ((bcnf->argt = malloc(size)) == NULL)
   {
      bindle_cnfclose(bcnf);
      return(errno);
   };
   bzero(bcnf->argt, size);

   if ((bcnf->filename = strdup(filename)) == NULL)
   {
      bindle_cnfclose(bcnf);
      return(errno);
   };

   if ((bcnf->buff = calloc(1, 512)) == NULL)
   {
      bindle_cnfclose(bcnf);
      return(errno);
   };

   if ((err = bindle_fdopen(&bcnf->fd, filename)) != BINDLE_SUCCESS)
   {
      bindle_cnfclose(bcnf);
      return(err);
   };

   if ((err = bindle_fdsetbuffer(bcnf->fd, bcnf->buff, 512)) != BINDLE_SUCCESS)
   {
      bindle_cnfclose(bcnf);
      return(errno);
   };

   *pbcnf = bcnf;

   return(BINDLE_SUCCESS);
}


int bindle_cnfread(bindlecnf * bcnf, int * pargc, const char *** pargv,
   const char ** ptypes, size_t * plinec)
{
   ssize_t      len;
   char       * line;
   ssize_t      pos;
   char         quote;
   ssize_t      x;
   int          err;

   assert(bcnf    != NULL);
   assert(pargv   != NULL);

   line = NULL;

   *pargv = NULL;
   if (pargc != NULL)
      *pargc = 0;
   if (ptypes != NULL)
      *ptypes = NULL;

   // saves initial node
   bcnf->argv[0] = NULL;
   bcnf->argc    = 0;

   // loops through looking for lines
   while((len = bindle_fdgetline(bcnf->fd, NULL, plinec, BINDLE_FD_STRIP_CR)) > 0)
   {
      // reset line position
      line = bcnf->buff;

      if ((bcnf->opts & BINDLE_CNF_DEBUG) != 0)
         printf("Line %zu: %s\n", *plinec, line);

      // strip comments
      bindle_cnfcomments(line, &len, bcnf->opts);

      // strip leading and tailing white space
      bindle_cnfwhitespace(&line, &len);
      if (len <= 0)
         continue;

      if ((bcnf->opts & BINDLE_CNF_ONESTR) != 0)
         if ((err = bindle_cnfargvadd(bcnf, line, BINDLE_CNF_TYPE_UNQUOTED)) != 0)
            return(err);

      // interprets escapes and comments
      quote = '\0';
      for(pos = 0; pos < len; pos++)
      {
         switch(line[pos])
         {
            // processes escaped characters
            case '\\':
            if (  (quote == '\'') ||
                  ((bcnf->opts & BINDLE_CNF_ESCAPES) == 0) )
               break;
            for(x = pos; x < len; x++)
               line[x] = line[x+1];
            len--;
            if (  ((bcnf->opts & BINDLE_CNF_ONESTR) != 0) ||
                  (quote != '\0') )
               break;
            quote = BINDLE_CNF_TYPE_UNQUOTED;
            if ((err = bindle_cnfargvadd(bcnf, &line[pos], quote)) != 0)
               return(err);
            break;

            // processes quoted strings
            case '"':
            case '\'':
            if ((bcnf->opts & BINDLE_CNF_ONESTR) != 0)
               break;
            if (  ((line[pos] == '\'') && ((bcnf->opts & BINDLE_CNF_QUOTE_SINGLE) == 0)) ||
                  ((line[pos] == '"') && ((bcnf->opts & BINDLE_CNF_QUOTE_DOULE) == 0)) )
            {
               if (quote == '\0')
               {
                  quote = BINDLE_CNF_TYPE_UNQUOTED;
                  if ((err = bindle_cnfargvadd(bcnf, &line[pos], quote)) != 0)
                     return(err);
               };
               break;
            };
            if (quote == BINDLE_CNF_TYPE_UNQUOTED)
               quote = '\0';
            if (quote != '\0')
            {
               if (line[pos] == quote)
               {
                  line[pos]   = '\0';
                  quote       = '\0';
               };
            }
            else
            {
               quote       = line[pos];
               line[pos]   = '\0';
               if ((err = bindle_cnfargvadd(bcnf, &line[pos+1], quote)) != 0)
                  return(err);
            };
            break;

            // processes spaces
            case ' ':
            case '\t':
            if ((bcnf->opts & BINDLE_CNF_ONESTR) != 0)
               break;
            if ((quote != '\0') && (quote != BINDLE_CNF_TYPE_UNQUOTED))
               break;
            quote     = '\0';
            line[pos] = '\0';
            while((line[pos+1] == ' ') || (line[pos+1] == '\t'))
               pos++;
            break;

            // normal characters
            default:
            if ((bcnf->opts & BINDLE_CNF_ONESTR) != 0)
               break;
            if (quote != '\0')
               break;
            quote = BINDLE_CNF_TYPE_UNQUOTED;
            if ((err = bindle_cnfargvadd(bcnf, &line[pos], quote)) != 0)
               return(err);
            break;
         };
      };

      if ((quote != '\0') && (quote != BINDLE_CNF_TYPE_UNQUOTED))
         return(errno = BINDLE_UNTERM);

      *pargv = (const char **)bcnf->argv;
      if (pargc != NULL)
         *pargc = bcnf->argc;
      if (ptypes != NULL)
         *ptypes = bcnf->argt;

      return(0);
   };

   if (len  == -1)
      if (bindle_fderrno(bcnf->fd) != BINDLE_EOF)
         return(bindle_fderrno(bcnf->fd));

   return(errno = BINDLE_EOF);
}


ssize_t bindle_cnfwhitespace(char ** pline, ssize_t * plen)
{
   char      * line;
   ssize_t     len;

   assert(pline != NULL);
   assert(plen  != NULL);

   line = *pline;
   len  = *plen;

   // strip leading white space
   while( ((line[0] == ' ') || (line[0] == '\t')) && (len > 0) )
   {
      line++;
      len--;
   };

   // strip tailing white space
   while ( ((line[len-1] == ' ') || (line[len-1] == '\t')) && (len > 0) )
   {
      line[len-1] = '\0';
      len--;
   };

   *plen = len;

   return(0);
}


/* end of source */
