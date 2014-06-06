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
/**
 *   @file bindle/err.h
 *   @defgroup err Error Functions
 *   @brief  Bindle Tools functions for retrieving error messages.
 *   @{
 */
#ifndef __BINDLE_ERRNO_H
#define __BINDLE_ERRNO_H 1


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


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#ifdef __BINDLE_PMARK
#pragma mark - Definitions
#endif

#define BINDLE_SUCCESS     0        ///< success
#define BINDLE_EOF         -10      ///< end of file


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#ifdef __BINDLE_PMARK
#pragma mark - Prototypes
#endif

/**
 *  This function returns the error value set in errno.
 *  @return    The error value set in errno.
 *  @see       bindle_errno, bindle_perror, bindle_strerror, bindle_strerror_r
 */
_BINDLE_F int bindle_errno(void);


/**
 *  The bindle_perror() function finds the error message corresponding to the
 *  current value of the global variable errno and writes it, followed by a
 *  newline, to the standard error file descriptor.
 *  @param[in]    s           If the argument s is non-NULL and does not point
 *                            to the null character, this string is prepended
 *                            to the message string and separated from it by a
 *                            colon and space (``: ''); otherwise, only the
 *                            error message string is printed.
 *
 *  @return    Upon successful completetion, this function returns 0. If the
 *             error number was not recoginized, the function returns EINVAL.
 *  @see       bindle_errno, bindle_perror, bindle_strerror, bindle_strerror_r
 */
_BINDLE_F void bindle_perror(const char * s);


/**
 *  The bindle_strerror() function accepts an error number argument errnum and
 *  returns a pointer to the corresponding message string.
 *  @param[in]    errnum      error number argument
 *
 *  @return    If successful, the function returns a pointer to the
 *             corresponding message string. If the error number is not
 *             recognized, the function return an error message string
 *             containing ``Unknown error: '' followed by the error number in
 *             decimal.
 *  @see       bindle_errno, bindle_perror, bindle_strerror, bindle_strerror_r
 */
_BINDLE_F char * bindle_strerror(int errnum);


/**
 *  The bindle_strerror_r() function renders the same result into strerrbuf
 *  for a maximum of buflen characters and returns 0 upon success.
 *  @param[in]    errnum      error number argument
 *  @param[out]   strerrbuf   buffer to store rendered string
 *  @param[in]    buflen      length of buffer
 *
 *  @return    If successful, the function returns 0 and copies the
 *             corresponding message string into strerrbuf. If the error
 *             number is not recognized, the function returns EINVAL and
 *             copies error message string containing ``Unknown error: ''
 *             followed by the error number into the buffer. If insufficient
 *             storage is provided in strerrbuf (as specified in buflen) to
 *             contain the error string, bindle_strerror_r() returns ERANGE
 *             and strerrbuf will contain an error message that has been
 *             truncated and NUL terminated to fit the length specified by
 *             buflen.
 *  @see       bindle_errno, bindle_perror, bindle_strerror, bindle_strerror_r
 */
_BINDLE_F int bindle_strerror_r(int errnum, char * strerrbuf, size_t buflen);


/// @}
#endif
/* end of source */
