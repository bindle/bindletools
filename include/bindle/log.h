/*
 *  Xen Herder
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
/**
 *   @file bindle/log.h
 *   @defgroup log Logging Functions
 *   @brief  Bindle Tools logging functions.
 *   @{
 */
#ifndef __BINDLE_LOG_H
#define __BINDLE_LOG_H 1
#undef __BINDLE_PMARK

///////////////
//           //
//  Headers  //
//           //
///////////////
#ifdef __BINDLE_PMARK
#pragma mark - Headers
#endif

#include <sys/types.h>
#include <inttypes.h>

#include <bindle/cdefs.h>


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#ifdef __BINDLE_PMARK
#pragma mark - Prototypes
#endif

/**
 *  returns a file name with the path and suffix removed
 *  @param[in]    str         string to manipulate
 *  @param[in]    suffix      substring to strip from end of str
 *
 *  @return    Returns the modified string.  The result must be freed with
 *             free().
 *  @see       bindle_basename, bindle_dirname
 */
_BINDLE_F char * bindle_basename(const char * str, const char * suffix);


/**
 *  returns a path with file name removed
 *  @param[in]    str         string to manipulate
 *
 *  @return    Returns the modified string.  The result must be freed with
 *             free().
 *  @see       bindle_basename, bindle_dirname
 */
_BINDLE_F char * bindle_dirname(const char * str);


/// @}
#endif
/* end of source */
