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
#ifndef _TESTS_BINDLE_ENC_STRINGS_H
#define _TESTS_BINDLE_ENC_STRINGS_H 1

///////////////
//           //
//  Headers  //
//           //
///////////////
// MARK: - Headers

#include "bindle-tests.h"

#include <string.h>
#include <strings.h>
#include <stdlib.h>


/////////////////
//             //
//  Datatypes  //
//             //
/////////////////
// MARK: - Datatypes

struct test_data
{
   const char * dec;
   const char * enc;
   intptr_t     nopad;
};


/////////////////
//             //
//  Variables  //
//             //
/////////////////
// MARK: - Variables

extern struct test_data base32_strings[];
extern struct test_data base32hex_strings[];
extern struct test_data base64_strings[];
extern struct test_data crockford_strings[];
extern struct test_data hex_strings[];
extern struct test_data pctenc_strings[];


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
// MARK: - Prototypes

int
bindle_test_decode(
         const char *                  prog_name,
         int                           argc,
         char *                        argv[],
         int                           method,
         struct test_data *            data );


int
bindle_test_encode(
         const char *                  prog_name,
         int                           argc,
         char *                        argv[],
         int                           method,
         struct test_data *            data );


int
bindle_test_validate(
         const char *                  prog_name,
         int                           argc,
         char *                        argv[],
         int                           method,
         struct test_data *            data );

#endif /* end of header file */
