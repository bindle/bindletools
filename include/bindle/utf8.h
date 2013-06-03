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
 *   @file bindle/utf8.h
 *   Bindletools UTF-8 provides basic support for manipulating UTF-8 strings.
 */
#ifndef __BINDLE_UTF8_H
#define __BINDLE_UTF8_H 1


///////////////
//           //
//  Headers  //
//           //
///////////////
#ifdef PMARK
#pragma mark - Headers
#endif

#include <sys/types.h>
#include <inttypes.h>


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#ifdef PMARK
#pragma mark - Prototypes
#endif

/**
 *  Determines the length of a string contained within a buffer of unknown
 *  buffer length.  The UTF-8 string must me NULL terminated.
 *  @param  s  Pointer to buffer containing UTF-8 string.
 *  @return This function returns the number of UTF-8 characters found within
 *    the buffer.
 *  @warning If the buffer is not NULL terminated, this function will overrun
 *    the buffer.
 */
size_t utf8len(const char * s);


/**
 *  Determines the length of a string contained within a buffer of unknown
 *  buffer length.  The UTF-8 string must me NULL terminated.
 *  @param  s       Pointer to buffer containing UTF-8 string.
 *  @param  maxlen  Length of buffer in bytes.
 *  @return This function returns the number of UTF-8 characters found within
 *    the buffer.
 */
size_t utf8nlen(const char * s, size_t maxlen);


/**
 *  Determines if the buffer contains a valid NULL terminated UTF-8 string.
 *  @param  s  Pointer to buffer containing UTF-8 string.
 *  @return This function returns the number of UTF-8 characters found within
 *    the buffer if the buffer contains a valid UTF-8 string.
 */
ssize_t utf8test(const char * s, size_t maxlen);


#endif
/* end of source */
