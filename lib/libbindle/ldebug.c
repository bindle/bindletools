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
#define _LIB_LIBBINDLE_LDEBUG_C 1
#include "ldebug.h"


///////////////
//           //
//  Headers  //
//           //
///////////////
#pragma mark - Headers

#undef NDEBUG

#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <syslog.h>
#include <stdarg.h>
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

// yes, global variables are evil...
const char *   bindle_debug_ident     = BNDL_DFLT_DEBUG_IDENT;
char *         bindle_debug_ident_ptr = NULL;
int            bindle_debug_level     = BNDL_DFLT_DEBUG_LEVEL;
int            bindle_debug_syslog    = BNDL_DFLT_DEBUG_SYSLOG;


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#pragma mark - Functions

void
bindle_debug(
         int                           level,
         const char *                  fmt,
         ... )
{
   va_list  args;

   if ( ((level & bindle_debug_level) == 0) || (!(fmt)) )
      return;

   if (!(bindle_debug_syslog))
      printf("%s: DEBUG: ", bindle_debug_ident);

   va_start(args, fmt);
   if ((bindle_debug_syslog))
      vsyslog(LOG_DEBUG, fmt, args);
   else
      vprintf(fmt, args);
   va_end(args);

   if (!(bindle_debug_syslog))
      printf("\n");

   return;
}


/* end of source */
