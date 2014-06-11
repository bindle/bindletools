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
#include <bindle/err.h>

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
   int              err;
   char           * buff;
   size_t           bsize;       ///< Buffer size
   size_t           blen;        ///< Buffer length (amount of data stored)
   size_t           boffset;     ///< Buffer offset of EOL
   size_t           lnum;        ///< Line count
   size_t           lcur;        ///< current line count
   char           * name;        ///< name of file
};


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#ifdef __BINDLE_PMARK
#pragma mark - Functions
#endif

void bindle_fdclose(bindlefd * bfd)
{
   if (bfd == NULL)
      return;

   if (bfd->fd != -1)
      close(bfd->fd);

   free(bfd->buff);
   free(bfd->name);
   free(bfd);

   return;
}


int bindle_fderrno(bindlefd * bfd)
{
   assert(bfd != NULL);
   return(bfd->err);
}


ssize_t bindle_fdgetline(bindlefd * bfd, const char ** linep,
   size_t * linenump, int opts)
{
   size_t      offset;
   size_t      pos;
   ssize_t     len;

   assert(bfd  != NULL);

   if (linep != NULL)
      *linep = NULL;

   // shift buffer to the left (erasing current line)
   if (bfd->boffset != 0)
   {
      offset = bfd->blen - bfd->boffset;
      for(pos = 0; pos < offset; pos++)
         bfd->buff[pos] = bfd->buff[pos + bfd->boffset];
   };
   bfd->blen    -= bfd->boffset;
   bfd->boffset  = 0;

   // attempt to fill buffer from file
   if ((len = read(bfd->fd, &bfd->buff[bfd->blen], (bfd->bsize - bfd->blen - 1))) == -1)
   {
      bfd->err = errno;
      return(-1);
   };
   bfd->blen += len;

   // loop through file
   for(bfd->boffset = 0; bfd->boffset < bfd->blen; bfd->boffset++)
   {
      // parse individual characters to find EOL
      switch(bfd->buff[bfd->boffset])
      {
         case '\n':
         bfd->buff[bfd->boffset] = '\0';
         bfd->boffset++;
         bfd->lnum++;
         if (linenump != NULL)
            *linenump = bfd->lnum;
         if (linep != NULL)
            *linep = bfd->buff;
         return(bfd->boffset);

         case '\r':
         if ((opts & BINDLE_FD_STRIP_CR) == 0)
            break;
         for(pos = bfd->boffset+1; pos < bfd->blen; pos++)
            bfd->buff[pos-1] = bfd->buff[pos];
         bfd->blen--;
         bfd->boffset--;
         break;

         default:
         break;
      };

      // attempt to fill buffer from file
      if ((bfd->bsize - 1) < bfd->blen)
      {
         if ((len = read(bfd->fd, &bfd->buff[bfd->blen], (bfd->bsize - bfd->blen - 1))) == -1)
         {
            bfd->err = errno;
            return(-1);
         };
         bfd->blen += len;
      };
   };

   if ((linenump))
      *linenump = bfd->lnum;

   if (bfd->blen == 0)
   {
      errno    = BINDLE_EOF;
      bfd->err = errno;
      return(0);
   };

   errno    = EFBIG;
   bfd->err = errno;

   return(-1);
}


size_t bindle_fdlinenumber(bindlefd * bfd)
{
   assert(bfd != NULL);
   return(bfd->lnum);
}


int bindle_fdopen(bindlefd ** pbfd, const char * filename)
{
   bindlefd * bfd;

   assert(pbfd     != NULL);
   assert(filename != NULL);

   if ((bfd = malloc(sizeof(bindlefd))) == NULL)
      return(errno);
   bzero(bfd, sizeof(bindlefd));
   bfd->fd = -1;

   if ((bfd->buff = calloc(1, BINDLE_FD_BUFF_SIZE)) == NULL)
   {
      bindle_fdclose(bfd);
      return(errno);
   };
   bfd->bsize = BINDLE_FD_BUFF_SIZE;

   if ((bfd->name = strdup(filename)) == NULL)
   {
      bindle_fdclose(bfd);
      return(errno);
   };

   if ((bfd->fd = open(filename, O_RDONLY)) == -1)
   {
      bindle_fdclose(bfd);
      return(errno);
   };

   *pbfd = bfd;

   return(BINDLE_SUCCESS);
}


int bindle_fdresize(bindlefd * bfd, size_t size, char ** pbuf)
{
   void * ptr;

   assert(bfd != NULL);

   if (pbuf != NULL)
      *pbuf = bfd->buff;

   if (size == bfd->bsize)
      return(BINDLE_SUCCESS);

   if ((ptr = realloc(bfd->buff, size)) == NULL)
      return(errno);
   bfd->buff      = ptr;
   bfd->bsize     = size;
   bfd->blen      = (bfd->blen > size)    ? size : bfd->blen;
   bfd->boffset   = (bfd->boffset > size) ? size : bfd->boffset;

   if (pbuf != NULL)
      *pbuf = bfd->buff;

   return(BINDLE_SUCCESS);
}


size_t bindle_fdsize(bindlefd * bfd)
{
   assert(bfd != NULL);
   return(bfd->bsize);
}


size_t bindle_fdstat(bindlefd * bfd, struct stat * buf)
{
   assert(bfd != NULL);
   assert(buf != NULL);
   return(fstat(bfd->fd, buf));
}


/* end of source */
