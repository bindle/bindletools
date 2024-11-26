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
#define _EXAMPLES_EXAMPLE_ENCODE_C 1


///////////////
//           //
//  Headers  //
//           //
///////////////
// MARK: - Headers

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <bindle.h>


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
// MARK: - Definitions

#undef PROGRAM_NAME
#define PROGRAM_NAME "example-encode"


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
// MARK: - Prototypes

int
main(
         int                           argc,
         char *                        argv[] );


/////////////////
//             //
//  Functions  //
//             //
/////////////////
// MARK: - Functions

int
main(
         int                           argc,
         char *                        argv[] )
{
   int         i;
   ssize_t     rc;
   char        buff[1024];

   for(i = 1; (i < argc); i++)
   {
      if ((rc = bindle_encode_size(BNDL_BASE32, strlen(argv[i]))) == -1)
      {
         fprintf(stderr, "bindle_encode_size(): %s\n", strerror(errno));
         return(1);
      };
      if ((size_t)rc > (sizeof(buff) - 1))
      {
         fprintf(stderr, "error: buffer is too small for encoded string\n");
         return(1);
      };
      if (bindle_encode(BNDL_BASE32, buff, sizeof(buff), argv[i], strlen(argv[i])) == -1)
      {
         fprintf(stderr, "bindle_encode(): %s\n", strerror(errno));
         return(1);
      };
      printf("%i: %s\n", i, buff);
   };

   return(0);
}

/* end of source */

