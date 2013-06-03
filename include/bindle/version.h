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
/**
 *  @file bindle/version.h
 *  Bindle Tools library and API versions.
 */
#ifndef __BINDLE_VERSION_H
#define __BINDLE_VERSION_H 1


///////////////
//           //
//  Headers  //
//           //
///////////////

#include <sys/types.h>


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////

/**
 *  @return Returns build string of Bindle Tools library.
 *  @see bindle_string
 *  @see bindle_version
 */
const char * bindle_build(void);


/**
 *  @return Returns the library version and build in dot notation.
 *  @see bindle_build
 *  @see bindle_version
 */
const char * bindle_string(void);


/**
 *  @return Returns the library version in dot notation.
 *  @see bindle_build
 *  @see bindle_string
 */
const char * bindle_version(void);


/**
 *  @return Returns the library API version.
 *  @see bindle_lib_version_revision
 *  @see bindle_lib_version_age
 */
int bindle_lib_version_current(void);


/**
 *  @return Returns the library API revision for current version.
 *  @see bindle_lib_version_current
 *  @see bindle_lib_version_age
 */
int bindle_lib_version_revision(void);


/**
 *  @return Returns the library API compatibility with past revisions.
 *  @see bindle_lib_version_current
 *  @see bindle_lib_version_revision
 */
int bindle_lib_version_age(void);


/**
 *  @return Returns the library libtool API string.
 *  @see bindle_lib_release_info
 */
const char * bindle_lib_version_info(void);


/**
 *  @return Returns the library DLL API string.
 *  @see bindle_lib_version_info
 */
const char * bindle_lib_release_info(void);


#endif
/* end of source */
