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
#define _LIB_LIBBINDLE_LOPTIONS_C 1
#include "loptions.h"


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
#include <sys/errno.h>
#include <assert.h>

#include "ldebug.h"


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#pragma mark - Definitions


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#pragma mark - Prototypes


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#pragma mark - Functions

int
bindle_get_option(
         void *                        ptr,
         int                           option,
         void *                        outvalue )
{
   BindleDebugTrace();

   assert(outvalue != NULL);

   // get global options
   switch(option)
   {
      case BNDL_OPT_DEBUG_IDENT:
      BindleDebug(BNDL_DEBUG_ARGS, "   == %s( tr, BNDL_OPT_DEBUG_IDENT, outvalue )", __func__);
      BindleDebug(BNDL_DEBUG_ARGS, "   <= outvalue: %s", bindle_debug_ident);
      if ((*((char **)outvalue) = bindle_strdup(bindle_debug_ident)) == NULL)
         return(ENOMEM);
      return(BNDL_SUCCESS);

      case BNDL_OPT_DEBUG_LEVEL:
      BindleDebug(BNDL_DEBUG_ARGS, "   == %s( tr, BNDL_OPT_DEBUG_LEVEL, outvalue )", __func__);
      BindleDebug(BNDL_DEBUG_ARGS, "   <= outvalue: 0x%08x", bindle_debug_level);
      *((int *)outvalue) = bindle_debug_level;
      return(BNDL_SUCCESS);

      case BNDL_OPT_DEBUG_SYSLOG:
      BindleDebug(BNDL_DEBUG_ARGS, "   == %s( tr, BNDL_OPT_DEBUG_SYSLOG, outvalue )", __func__);
      BindleDebug(BNDL_DEBUG_ARGS, "   <= outvalue: %s", ((bindle_debug_syslog)) ? "BNDL_ON" : "BNDL_OFF");
      *((int *)outvalue) = bindle_debug_syslog;
      return(BNDL_SUCCESS);

      default:
      break;
   };

   if ((ptr))
      return(BNDL_SUCCESS);

   return(BNDL_SUCCESS);
}


int
bindle_set_option(
         void *                        ptr,
         int                           option,
         const void *                  invalue )
{
   BindleDebugTrace();

   assert(invalue != NULL);

   // set global options
   switch(option)
   {
      case BNDL_OPT_DEBUG_IDENT:
      BindleDebug(BNDL_DEBUG_ARGS, "   == %s( tr, BNDL_OPT_DEBUG_IDENT, \"%s\" )", __func__, ((!((const char *)invalue)) ? "(NULL)" : (const char *)invalue));
      if (!((const char *)invalue))
         invalue = BNDL_DFLT_DEBUG_IDENT;
      bindle_strlcpy(bindle_debug_ident_buff, (const char *)invalue, sizeof(bindle_debug_ident_buff));
      bindle_debug_ident = bindle_debug_ident_buff;
      return(BNDL_SUCCESS);

      case BNDL_OPT_DEBUG_LEVEL:
      BindleDebug(BNDL_DEBUG_ARGS, "   == %s( tr, BNDL_OPT_DEBUG_LEVEL, 0x%08x )", __func__, *((const int *)invalue));
      bindle_debug_level = *((const int *)invalue);
      return(BNDL_SUCCESS);

      case BNDL_OPT_DEBUG_SYSLOG:
      BindleDebug(BNDL_DEBUG_ARGS, "   == %s( tr, BNDL_OPT_DEBUG_SYSLOG, %i )", __func__, *((const int *)invalue));
      bindle_debug_syslog = ((*((const int *)invalue)) == BNDL_OFF) ? BNDL_OFF : BNDL_ON;
      return(BNDL_SUCCESS);

      default:
      break;
   };

   if (!(ptr))
      return(BNDL_SUCCESS);

   return(BNDL_SUCCESS);
}



/* end of source */
