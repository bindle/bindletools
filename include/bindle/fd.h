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
 *   @file bindle/fd.h
 *   Bindletools file functions provide basic support for parsing ASCII/UTF-8
 *   text files.
 *
 *   @defgroup fd File Parsing Functions
 *   @brief  Bindletools file functions provide basic support for parsing
 *   ASCII/UTF-8 text files.
 *   @{
 */
#ifndef __BINDLE_FD_H
#define __BINDLE_FD_H 1


///////////////
//           //
//  Headers  //
//           //
///////////////
#ifdef __BINDLE_PMARK
#pragma mark - Headers
#endif

#include <sys/types.h>
#include <sys/stat.h>
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

#define BINDLE_FD_STRIP_CR       0x04     ///< Strip carriage returns from line
#define BINDLE_FD_BUFF_SIZE      256ULL   ///< default buffer size used to parse lines


//////////////////
//              //
//  Data Types  //
//              //
//////////////////
#ifdef __BINDLE_PMARK
#pragma mark - Data Types
#endif

/// Bindle file descriptor state.
typedef struct bindle_fd_struct  bindlefd;


///////////////
//           //
//  Structs  //
//           //
///////////////
#ifdef __BINDLE_PMARK
#pragma mark - Structs
#endif


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#ifdef __BINDLE_PMARK
#pragma mark - Prototypes
#endif

/**
 *  Destroys an UTF-8/ASCII file stack and frees the stack's resources.
 *  @param[in]    bfd         Reference to the stack.
 *
 *  @return    This function does not return any value.
 *  @see       bindle_fdclose, bindle_fdgetline, bindle_fdlinenumber,
 *             bindle_fdname, bindle_fdopen, bindle_fdresize, bindle_fdsize,
 *             bindle_fdstat
 */
_BINDLE_F void bindle_fdclose(bindlefd * bfd);


/**
 *  Opens a UTF-8/ASCII file for parsing.
 *  @param[in]    filename    Reference to path of file to append to the stack.
 *
 *  @return    Upon successful completetion, this function returns the
 *             reference to the file stack and the file reference at the top
 *             of the stack. If an error occurs, NULL is returned and errno is
 *             set.
 *  @see       bindle_fdclose, bindle_fdgetline, bindle_fdlinenumber,
 *             bindle_fdname, bindle_fdopen, bindle_fdresize, bindle_fdsize,
 *             bindle_fdstat
 */
_BINDLE_F bindlefd * bindle_fdopen(const char * filename);


/**
 *  Reads next line from file.
 *  @param[in]    bfd         Reference to the stack.
 *  @param[out]   linep       Reference to the stack.
 *  @param[out]   linenump    Reference to the stack.
 *  @param[in]    opts        Reference to the stack.
 *
 *  @return    Returns length of line on success, returns -1 on error and
 *             sets errno.
 *  @warning   This function is not optimized. It is intended to be used at
 *             the startup of an application/daemon. This function would needs
 *             to be refactored before it would be advisable to use in a
 *             frequently used loop (such as the event loop for a daemon).
 *  @see       bindle_fdclose, bindle_fdgetline, bindle_fdlinenumber,
 *             bindle_fdname, bindle_fdopen, bindle_fdresize, bindle_fdsize,
 *             bindle_fdstat
 */
_BINDLE_F ssize_t bindle_fdgetline(bindlefd * bfd, const char ** linep,
   size_t * linenump, int opts);


/**
 *  Returns the number of the current line within the file.
 *  @param[in]    bfd         Pointer existing file stack.
 *
 *  @return    Returns the number of the current line within the file.
 *  @see       bindle_fdclose, bindle_fdgetline, bindle_fdlinenumber,
 *             bindle_fdname, bindle_fdopen, bindle_fdresize, bindle_fdsize,
 *             bindle_fdstat
 */
_BINDLE_F size_t bindle_fdlinenumber(bindlefd * bfd);


/**
 *  Opens a UTF-8/ASCII file for parsing.
 *  @param[in]    bfd         Pointer existing file stack.
 *
 *  @return    Upon successful completetion, this function returns a reference
 *             to the name of the top file on the stack.
 *  @see       bindle_fdclose, bindle_fdgetline, bindle_fdlinenumber,
 *             bindle_fdname, bindle_fdopen, bindle_fdresize, bindle_fdsize,
 *             bindle_fdstat
 */
_BINDLE_F const char * bindle_fdname(bindlefd * bfd);


/**
 *  Changes the size of the working buffer.
 *  @param[in]    bfd         Reference to the stack.
 *  @param[in]    size        Change buffer to size.
 *  @param[out]   pbuf        will be populated with pointer to internal buffer
 *
 *  @return    Returns length of buffer on success, returns -1 on error and
 *             sets errno.
 *  @see       bindle_fdclose, bindle_fdgetline, bindle_fdlinenumber,
 *             bindle_fdname, bindle_fdopen, bindle_fdresize, bindle_fdsize,
 *             bindle_fdstat
 */
_BINDLE_F ssize_t bindle_fdresize(bindlefd * bfd, size_t size, char ** pbuf);


/**
 *  Retrieves current buffer size.
 *  @param[in]    bfd         Reference to the stack.
 *
 *  @return    Returns the size of the buffer in bytes.
 *  @see       bindle_fdclose, bindle_fdgetline, bindle_fdlinenumber,
 *             bindle_fdname, bindle_fdopen, bindle_fdresize, bindle_fdsize,
 *             bindle_fdstat
 */
_BINDLE_F size_t bindle_fdsize(bindlefd * bfd);


/**
 *  Obtains information about the file
 *  @param[in]    bfd         Pointer existing file stack.
 *  @param[out]   buf         pointer to a stat structure as defined
 *
 *  @return    Upon successful completion a value of 0 is returned. Otherwise,
 *             a value of -1 is returned and errno is set to indicate the error.
 *  @see       bindle_fdclose, bindle_fdgetline, bindle_fdlinenumber,
 *             bindle_fdname, bindle_fdopen, bindle_fdresize, bindle_fdsize,
 *             bindle_fdstat
 */
_BINDLE_F size_t bindle_fdstat(bindlefd * bfd, struct stat * buf);


/// @}
#endif
/* end of source */
