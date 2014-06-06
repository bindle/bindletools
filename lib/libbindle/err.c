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
#define __LIB_LIBBINDLE_ERR_C 1

///////////////
//           //
//  Headers  //
//           //
///////////////

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <bindle/err.h>

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#ifdef __BINDLE_PMARK
#pragma mark - Prototypes
#endif

const char * bindle_errlist(int errnum);


/////////////////
//             //
//  Functions  //
//             //
/////////////////

const char * bindle_errlist(int errnum)
{
   switch(errnum)
   {
      case BINDLE_SUCCESS:    return("success");
      case BINDLE_EOF:        return("end of file");
      default:                break;
   };
   return(NULL);
}


int bindle_errno(void)
{
   return(errno);
}


void bindle_perror(const char * s)
{
   if (errno > 0)
   {
      perror(s);
      return;
   };
   if (s != NULL)
   {
      if (s[0] != '\0')
      {
         fprintf(stderr, "%s\n", s);
         return;
      };
   };
   fprintf(stderr, "%s: %s\n", s, bindle_strerror(errno));
   return;
}


char * bindle_strerror(int errnum)
{
   static char strerrbuf[80];
   if (errnum > 0)
      return(strerror(errnum));
   bindle_strerror_r(errnum, strerrbuf, sizeof(strerrbuf));
   return(strerrbuf);
}


int bindle_strerror_r(int errnum, char * strerrbuf, size_t buflen)
{
   const char * str;
   assert(strerrbuf != NULL);
   if (errnum > 0)
      return(strerror_r(errnum, strerrbuf, buflen));
   if ((str = bindle_errlist(errnum)) == NULL)
   {
      snprintf(strerrbuf, buflen, "unknown error: %i", errnum);
      return(EINVAL);
   };
   strncpy(strerrbuf, str, buflen);
   if (strerrbuf[buflen-1] != '\0')
   {
      strerrbuf[buflen-1] = '\0';
      return(ERANGE);
   };
   strerrbuf[buflen-1] = '\0';
   return(0);
}


/* end of source */
