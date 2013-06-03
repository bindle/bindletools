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


///////////////
//           //
//  Headers  //
//           //
///////////////

#include <bindle/version.h>
#include <string.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#include "git-package-version.h"
#endif

#ifndef LIB_VERSION_CURRENT
#define LIB_VERSION_CURRENT -1
#endif
#ifndef LIB_VERSION_REVISION
#define LIB_VERSION_REVISION -1
#endif
#ifndef LIB_VERSION_AGE
#define LIB_VERSION_AGE -1
#endif
#ifndef LIB_VERSION_INFO
#define LIB_VERSION_INFO NULL
#endif
#ifndef LIB_RELEASE_INFO
#define LIB_RELEASE_INFO NULL
#endif


/////////////////
//             //
//  Functions  //
//             //
/////////////////

const char * bindle_build(void)
{
   return(GIT_PACKAGE_BUILD);
}


int bindle_lib_version_current(void)
{
   return(LIB_VERSION_CURRENT);
}


int bindle_lib_version_revision(void)
{
   return(LIB_VERSION_REVISION);
}


int bindle_lib_version_age(void)
{
   return(LIB_VERSION_AGE);
}


const char * bindle_lib_version_info(void)
{
   return(LIB_VERSION_INFO);
}


const char * bindle_lib_release_info(void)
{
   return(LIB_RELEASE_INFO);
}


const char * bindle_string(void)
{
   return(GIT_PACKAGE_STRING);
}


const char * bindle_version(void)
{
   return(GIT_PACKAGE_VERSION);
}

/* end of source */
