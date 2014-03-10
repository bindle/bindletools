/*
 *  Bindle Binaries Tools
 *  Copyright (C) 2013 Bindle Binaries <syzdek@bindlebinaries.com>.
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

#include <stdio.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

extern int errno;

#include <bindle.h>


int main (int argc, char * argv[])
{
   struct stat   sb;
   int           fd;
   ssize_t       len;
   ssize_t       buff_len;
   char          buff[4096];
   char        * bol;
   ssize_t       expected;
   ssize_t       diff;
   ssize_t       result;

   // checks arguments
   if (argc != 2)
   {
      fprintf(stderr, "Usage: %s <file>\n", argv[0]);
      return(1);
   }

   // checks for file
   if (stat(argv[1], &sb) == -1)
   {
      fprintf(stderr, "%s: stat(%s): %s\n", argv[0], argv[1], strerror(errno));
      return(1);
   };

   // opens file for reading
   if ((fd = open(argv[1], O_RDONLY)) == -1)
   {
      fprintf(stderr, "%s: open(%s): %s\n", argv[0], argv[1], strerror(errno));
      return(1);
   };

   // loads file into buffer
   if ((len = read(fd, buff, sb.st_size)) == -1)
   {
      fprintf(stderr, "%s: read(%s): %s\n", argv[0], argv[1], strerror(errno));
      close(fd);
      return(1);
   };
   buff[len] = '\0';

   // closes file
   close(fd);

   // verifies length
   if (len == 0)
   {
      fprintf(stderr, "%s: %s: invalid test file\n", argv[0], argv[1]);
      return(1);
   };

   // calculates expected string size and buffer size
   expected = strtoll(buff, &bol, 0);
   if (!(bol))
   {
      fprintf(stderr, "%s: %s: unable to parse test file\n", argv[0], argv[1]);
      return(1);
   };

   // finds new line
   if ((bol = index(bol, '\n')) == NULL)
   {
      fprintf(stderr, "%s: %s: invalid test file\n", argv[0], argv[1]);
      return(1);
   };
   bol++;

   diff     = bol - buff;
   buff_len = sb.st_size - diff + 1;

   result = utf8test(bol, buff_len);
   fprintf(stderr, "%s: buff length: %lu\n", argv[1],    buff_len);
   fprintf(stderr, "%s: buff diff:   %lu\n", argv[1],    diff);
   fprintf(stderr, "%s: expected:    %li\n", argv[1],    expected);
   fprintf(stderr, "%s: result:      %li\n", argv[1],    result);
   fprintf(stderr, "%s: string:      \"%s\"\n", argv[1], bol);

   // tests string
   if ((result = utf8test(bol, buff_len)) != expected)
   {
      return(1);
   };

   return(0);
}
