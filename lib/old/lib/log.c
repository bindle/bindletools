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
#define __LIB_LIBBINDLE_LOG_C 1

///////////////
//           //
//  Headers  //
//           //
///////////////

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <bindle/log.h>

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#ifdef __BINDLE_PMARK
#pragma mark - Prototypes
#endif


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#ifdef __BINDLE_PMARK
#pragma mark - Functions
#endif

char * bindle_basename(const char * str, const char * suffix)
{
   char   * file;
   char   * ptr;
   size_t   len;
   size_t   slen;
   size_t   pos;
   size_t   off;

   assert(str != NULL);

   if ((file = strdup(str)) == NULL)
      return(NULL);

   len = strlen(file);

   if (len <= 1)
      return(file);
   while (file[len-1] == '/')
   {
      file[len-1] = '\0';
      len--;
   };
   if (len <= 1)
      return(file);

   if (suffix != NULL)
   {
      slen = strlen(suffix);
      if (slen < len)
         if (strcmp(&file[len-slen], suffix) == 0)
            file[len-slen] = '\0';
   };

   if ((ptr = rindex(file, '/')) != NULL)
   {
      off = ptr - file + 1;
      for(pos = 0; file[pos+off] != '\0'; pos++)
      {
         file[pos] = file[pos+off];
      };
      file[pos] = '\0';
   };

   return(file);
}


char * bindle_dirname(const char * str)
{
   char   * path;
   char   * ptr;
   size_t   len;

   assert(str != NULL);

   if (str[0] == '\0')
      return(strdup("."));

   // strip excess leading slashes
   while((strncmp(str, "//", 2)) == 0)
      str++;

   // copy string
   if ((path = strdup(str)) == NULL)
      return(NULL);

   // strip tailing slash
   len = strlen(path);
   while (path[len-1] == '/')
   {
      if (len == 1)
         return(path);
      path[len-1] = '\0';
      len--;
   };

   // find last slash in string
   if ((ptr = rindex(path, '/')) == NULL)
   {
      strncpy(path, ".", 2);
      return(path);
   }
   if (ptr == path)
   {
      path[1] = '\0';
      return(path);
   };
   ptr[0] = '\0';

   // strip tailing slash from modified path
   len = strlen(path);
   while (path[len-1] == '/')
   {
      if (len == 1)
         return(path);
      path[len-1] = '\0';
      len--;
   };

   return(path);
}



/* end of source */
