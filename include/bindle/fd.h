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
 *  Return file descriptor for file in top of stack
 *  @param  bfd       Reference to the stack.
 *  @return Returns the file descriptor of the top file in the stack.
 *  @see bindle_fdopen, bindle_fdname
 */
_BINDLE_F int bindle_fd(bindlefd * bfd);


/**
 *  Destroys an UTF-8/ASCII file stack and frees the stack's resources.
 *  @param  bfd       Reference to the stack.
 *  @return This function does not return any value.
 *  @see bindle_fdopen, bindle_fd
 */
_BINDLE_F void bindle_fdclose(bindlefd * bfd);


/**
 *  Opens a UTF-8/ASCII file for parsing.
 *  @param  filename  Reference to path of file to append to the stack.
 *  @return Upon successful completetion, this function returns the reference
 *          to the file stack and the file reference at the top of the stack.
 *          If an error occurs, NULL is returned and errno is set.
 *  @see bindle_fdclose, bindle_fd, bindle_fdpopclose, bindle_errno, bindle_fdname
 */
_BINDLE_F bindlefd * bindle_fdopen(const char * filename);


/**
 *  Reads next line from file.
 *  @param  bfd       Reference to the stack.
 *  @param  linep     Reference to the stack.
 *  @param  linenump  Reference to the stack.
 *  @param  opts      Reference to the stack.
 *  @return Returns length of line on success, returns -1 on error and sets
 *          errno.
 *  @warning This function is not optimized. It is intended to be used at
 *           the startup of an application/daemon. This function would needs
 *           to be refactored before it would be advisable to use in a
 *           frequently used loop (such as the event loop for a daemon).
 *  @see bindle_fdopen
 */
_BINDLE_F ssize_t bindle_fdgetline(bindlefd * bfd, const char ** linep,
   size_t * linenump, int opts);


/**
 *  Opens a UTF-8/ASCII file for parsing.
 *  @param  stck      Pointer existing file stack.
 *  @return Upon successful completetion, this function returns a reference
 *          to the name of the top file on the stack.
 *  @see bindle_fdopen, bindle_fd, bindle_errno
 */
_BINDLE_F const char * bindle_fdname(bindlefd * stck);


/**
 *  Reads next line from file.
 *  @param  bfd       Reference to the stack.
 *  @param  size      Change buffer to size.
 *  @return Returns length of buffer on success, returns -1 on error and sets
 *          errno.
 *  @see bindle_fdopen, bindle_fdsize
 */
_BINDLE_F ssize_t bindle_fdresize(bindlefd * bfd, size_t size);


/**
 *  Retrieves current buffer size.
 *  @param  bfd       Reference to the stack.
 *  @return Returns the size of the buffer in bytes.
 *  @see bindle_fdopen, bindle_fdresize
 */
_BINDLE_F size_t bindle_fdsize(bindlefd * bfd);


#endif
/* end of source */
