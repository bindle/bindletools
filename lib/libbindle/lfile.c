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
#define _LIB_LIBBINDLE_LFILE_C 1
#include "lfile.h"


///////////////
//           //
//  Headers  //
//           //
///////////////
#pragma mark - Headers

#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <syslog.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <assert.h>


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#pragma mark - Definitions


/////////////////
//             //
//  Variables  //
//             //
/////////////////
#pragma mark - Variables


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#pragma mark - Functions

int
bindle_filetostr(
         char *                        dst,
         const char *                  filename,
         size_t                        size )
{
   int            fd;
   struct stat    sb;
   size_t         len;
   ssize_t        rlen;

   assert(dst  != NULL);
   assert(size  > 1);

   if (stat(filename, &sb) == -1)
      return(-1);
   if ((sb.st_mode & S_IFMT) != S_IFREG)
   {
      errno = EINVAL;
      return(-1);
   };

   len = (size > (size_t)(sb.st_size+1)) ? (size_t)sb.st_size : (size - 1);

   if ((fd = open(filename, O_RDONLY)) == -1)
      return(-1);

   if ((rlen = read(fd, dst, len)) == -1)
   {
      close(fd);
      return(-1);
   };
   dst[rlen] = '\0';

   close(fd);

   return((int)rlen);
}


int
bindle_readline(
         int                           fd,
         char *                        str,
         size_t                        size )
{
   ssize_t           rc;
   struct stat       sb;
   size_t            pos;

   // check file descriptor
   if (fd == -1)
   {
      errno = EINVAL;
      return(-1);
   };
   if (fstat(fd, &sb) == -1)
      return(-1);

   // process input (slow, but works on TCP sockets, PIPE, FIFO, and  regular files)
   for(pos = 0; (pos < (size-1)); pos++)
   {
      // read data
      if ((rc = read(fd, &str[pos], 1)) == -1)
         return(-1);

      // process end of file
      if (rc == 0)
      {
         str[pos] = '\0';
         return((int)pos);
      };

      // continue if not end of file
      if (str[pos] != '\n')
         continue;

      // process end of line
      str[pos] = '\0';
      if ((pos > 0) && (str[pos-1] == '\r'))
         str[pos-1] = '\0';
      return((int)(pos+1));
   };

   str[pos] = '\0';

   return((int)(pos+1));
}

/* end of source */
