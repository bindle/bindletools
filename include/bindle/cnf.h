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
 *   @file bindle/cnf.h
 *   @defgroup cnf Configuration File Parsing Functions
 *   @brief  Bindletools configuration functions provide basic support for
 *   parsing ASCII/UTF-8 configuration files.
 *   @{
 */
#ifndef __BINDLE_CNF_H
#define __BINDLE_CNF_H 1
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


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#ifdef __BINDLE_PMARK
#pragma mark - Definitions
#endif

//#define BINDLE_CNF_INCLUDES         0x0001      ///< Process includes
#define BINDLE_CNF_ONESTR           0x0002      ///< do not splite line into arguments
#define BINDLE_CNF_ESCAPES          0x0004      ///< allow characters to be escaped with a backslash
#define BINDLE_CNF_DEBUG            0x0008      ///< enable debug output
#define BINDLE_CNF_UNQUOTED         0x0010      //   argument is unquoted
#define BINDLE_CNF_QUOTE_SINGLE     0x0020      ///< treat single quoted text as one argument
#define BINDLE_CNF_QUOTE_DOULE      0x0040      ///< treat double quoted text as one argument
//#define BINDLE_CNF_COMMENT_C        0x0100
#define BINDLE_CNF_COMMENT_CPP      0x0200
#define BINDLE_CNF_COMMENT_SH       0x0400      ///< Allow shell style comments
#define BINDLE_CNF_COMMENT_M4       (0x0800 | BINDLE_CNF_COMMENT_SH)
#define BINDLE_CNF_COMMENT_INI      0x1000      ///< Allow INI style comments

#define BINDLE_CNF_TYPE_UNQUOTED    0x74  ///< text was not quoted
#define BINDLE_CNF_TYPE_SQUOTE      0x27  ///< text was contained within single quotes
#define BINDLE_CNF_TYPE_DQUOTE      0x22  ///< text was contained within double quotes
#define BINDLE_CNF_TYPE_MQUOTE      0x60  ///< text was contained within M4 quotes

#define BINDLE_CNF_DEFAULT          (  BINDLE_CNF_ESCAPES | \
                                       BINDLE_CNF_COMMENT_SH | \
                                       BINDLE_CNF_QUOTE_SINGLE | \
                                       BINDLE_CNF_QUOTE_DOULE )

//////////////////
//              //
//  Data Types  //
//              //
//////////////////
#ifdef __BINDLE_PMARK
#pragma mark - Data Types
#endif

///
typedef struct bindle_cnf_struct  bindlecnf;


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#ifdef __BINDLE_PMARK
#pragma mark - Prototypes
#endif

/**
 *  Destroys an UTF-8/ASCII configuration file reference.
 *  @param[in]    bcnf        Reference to the configuration handle.
 *
 *  @see       bindle_cnfopen, bindle_cnfopen, bindle_cnfread,
 */
_BINDLE_F void bindle_cnfclose(bindlecnf * bcnf);


/**
 *  Parses a UTF-8/ASCII configuration file.
 *  @param[out]   pbcnf       Reference pointer used to store allocated bindlecnf struct.
 *  @param[in]    filename    Reference to path of file to append to the stack.
 *  @param[in]    opts        config file parsing options
 *
 *  @return    Upon successful completetion, this function returns the
 *             reference open file stack.
 *  @see       bindle_cnfopen, bindle_cnfopen, bindle_cnfread,
 */
_BINDLE_F int bindle_cnfopen(bindlecnf ** pbcnf, const char * filename,
   uint32_t opts);


/**
 *  Reads a line from the configuration
 *  @param[in]    bcnf        Reference to the configuration handle.
 *  @param[out]   pargv       to be written
 *  @param[out]   pargc       to be written
 *  @param[out]   plinec      to be written
 *
 *  @return    Upon successful completetion, this function returns 0,
 *             otherwise an error code is returned.
 *  @see       bindle_cnfopen, bindle_cnfopen, bindle_cnfread,
 */
int bindle_cnfread(bindlecnf * bcnf, int * pargc, const char *** pargv,
   const char ** ptypes, size_t * plinec);


/// @}
#endif
/* end of source */
