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
#define __LIB_LIBBINDLE_FD_C 1

///////////////
//           //
//  Headers  //
//           //
///////////////

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <bindle/fd.h>

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>


///////////////
//           //
//  Structs  //
//           //
///////////////
#ifdef __BINDLE_PMARK
#pragma mark - Structs
#endif

struct bindle_fd_struct
{
   int              fd;
   char             buff[BINDLE_FD_BUFF_SIZE];
   size_t           bsize;       ///< Buffer size
   size_t           blen;        ///< Buffer length (amount of data stored)
   size_t           boffset;     ///< Buffer offset of EOL
   size_t           lnum;        ///< Line count
   size_t           lcur;        ///< current line count
   struct stat      sb;          ///< stat() buffer of file
   bindlefd       * ptr;         ///< pointer to next item in circular stack
};


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#ifdef __BINDLE_PMARK
#pragma mark - Prototypes
#endif

bindlefd * bindle_fdalloc(void);
void bindle_fdfree(bindlefd * bfd);
bindlefd * bindle_fdpop(bindlefd * bfd);


/////////////////
//             //
//  Functions  //
//             //
/////////////////

int bindle_fd(bindlefd * bfd)
{
   assert(bfd != NULL);
   if (bfd->ptr == NULL)
      return(bfd->fd);
   return(bfd->ptr->fd);
}


inline bindlefd * bindle_fdalloc(void)
{
   bindlefd * bfd;

   if ((bfd = malloc(sizeof(bindlefd))) == NULL)
      return(NULL);
   memset(bfd, 0, sizeof(bindlefd));

   bfd->fd     = -1;
   bfd->ptr    = bfd;
   bfd->bsize  = sizeof(bfd->buff);

   return(bfd);
}


void bindle_fdclose(bindlefd * bfd)
{
   assert(bfd);
   while(bindle_fdpopclose(bfd) > 0);
   bindle_fdfree(bfd);
   return;
}


inline void bindle_fdfree(bindlefd * bfd)
{
   assert(bfd != NULL);

   if (bfd->fd != -1)
      close(bfd->fd);
   bfd->fd = -1;

   free(bfd);

   return;
}


ssize_t bindle_fdgetline(bindlefd * bfd, const char ** linep,
   size_t * linenump, int opts)
{
   bindlefd  * cur;
   size_t      offset;
   size_t      pos;
   ssize_t     len;

   assert(bfd  != NULL);

   errno = 0;

   cur = bfd->ptr;

   // shift buffer to the left (erasing current line)
   if (cur->boffset != 0)
   {
      offset = cur->blen - cur->boffset;
      for(pos = 0; pos < offset; pos++)
         cur->buff[pos] = cur->buff[pos + cur->boffset];
   };
   cur->blen    -= cur->boffset;
   cur->boffset  = 0;

   // attempt to fill buffer from file
   if ((len = read(cur->fd, &cur->buff[cur->blen], (cur->bsize - cur->blen - 1))) == -1)
   {
      *linep = NULL;
      return(-1);
   };
   cur->blen += len;

   // loop through file
   for(cur->boffset = 0; cur->boffset < cur->blen; cur->boffset++)
   {
      // parse individual characters to find EOL
      switch(cur->buff[cur->boffset])
      {
         case '\\':
         if (  ((opts & BINDLE_FD_ESC_NEWLINE) == 0) ||
               ((cur->boffset + 1) >= cur->blen) )
            break;
         if (cur->buff[cur->boffset+1] != '\n')
            break;
         for(pos = cur->boffset+2; pos < cur->blen; pos++)
            cur->buff[pos-2] = cur->buff[pos];
         cur->blen    -= 2;
         cur->boffset -= 2;
         cur->lnum++;
         break;

         case '\n':
         cur->buff[cur->boffset] = '\0';
         cur->boffset++;
         cur->lnum++;
         if ((linenump))
            *linenump = cur->lnum;
         *linep = cur->buff;
         return(cur->boffset);

         case '\r':
         if ((opts & BINDLE_FD_STRIP_CR) == 0)
            break;
         for(pos = cur->boffset+1; pos < cur->blen; pos++)
            cur->buff[pos-1] = cur->buff[pos];
         cur->blen--;
         cur->boffset--;
         break;

         default:
         break;
      };

      // attempt to fill buffer from file
      if ((cur->bsize - 1) < cur->blen)
      {
         if ((len = read(cur->fd, &cur->buff[cur->blen], (cur->bsize - cur->blen - 1))) == -1)
         {
            *linep = NULL;
            return(-1);
         };
         cur->blen += len;
      };
   };

   if ((linenump))
      *linenump = cur->lnum;

   if (cur->blen == 0)
   {
      *linep = NULL;
      return(0);
   };

   *linep = NULL;
   errno  = EFBIG;

   return(-1);
}


bindlefd * bindle_fdopen(bindlefd * stck, const char * filename)
{
   bindlefd * bfd;
   bindlefd * cur;

   assert(filename != NULL);

   if ((bfd = bindle_fdalloc()) == NULL)
      return(NULL);

   if (stat(filename, &bfd->sb) == -1)
   {
      bindle_fdfree(bfd);
      return(NULL);
   };

   if ((bfd->fd = open(filename, O_RDONLY)) == -1)
   {
      bindle_fdfree(bfd);
      return(NULL);
   };

   if (stck == NULL)
      return(bfd);

   bfd->ptr = stck->ptr;
   cur = bfd;
   while(cur != stck)
   {
      cur = cur->ptr;
      if (  (cur->sb.st_ino == bfd->sb.st_ino) &&
            (cur->sb.st_dev == bfd->sb.st_dev) )
      {
         errno = ELOOP;
         bindle_fdfree(bfd);
         return(NULL);
      };
   };

   stck->ptr = bfd;

   return(bfd);
}


inline bindlefd * bindle_fdpop(bindlefd * bfd)
{
   bindlefd * cur;
   assert(bfd != NULL);
   if (bfd->ptr == bfd)
      return(NULL);
   cur       = bfd->ptr;
   bfd->ptr = cur->ptr;
   return(cur);
}


int bindle_fdpopclose(bindlefd * bfd)
{
   bindlefd * cur;
   assert(bfd != NULL);
   if ((cur = bindle_fdpop(bfd)) == NULL)
      return(0);
   bindle_fdfree(cur);
   return(1);
}


/* end of source */
