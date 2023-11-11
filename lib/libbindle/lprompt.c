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
#define _LIB_LIBBINDLE_LPROMPT_C 1
#include "lprompt.h"


///////////////
//           //
//  Headers  //
//           //
///////////////
// MARK: - Headers

#include <stdio.h>
#include <signal.h>
#include <assert.h>

#ifdef HAVE_TERMIOS_H
#include <termios.h>
#endif
#ifdef HAVE_SGTTY_H
#include <sgtty.h>
#endif


/////////////////
//             //
//  Functions  //
//             //
/////////////////
// MARK: - Functions

char *
bindle_getpass(
         const char *                  prompt )
{
   static char    buff[BNDL_PASSWORD_LEN+1];
   return(bindle_getpass_r(prompt, buff, sizeof(buff)));
}


char *
bindle_getpass_r(
         const char *                  prompt,
         char *                        pass,
         size_t                        passlen )
{
   FILE         * fs;
   int            c;
   size_t         pos;
#if defined(HAVE_TERMIOS_H) || defined(HAVE_SGTTY_H)
   struct termios old;
   struct termios new;
   void          (*sig)( int sig );
#endif

   // prompts for password
   prompt = ((prompt)) ? prompt : "Password: ";
   if (prompt)
      fprintf(stderr, "%s", prompt);
   fflush(stdout);

   // disables ECHO
#if defined(HAVE_TERMIOS_H) || defined(HAVE_SGTTY_H)
   if ((fs = fopen("/dev/tty", "r")) == NULL)
      fs = stdin;
   if(tcgetattr(fileno(fs), &old) == -1)
      return(NULL);
   sig          = signal(SIGINT, SIG_IGN);
   new          = old;
   new.c_lflag &= ~((unsigned)ECHO);
   if(tcsetattr(fileno(fs), TCSANOW, &new))
      return(NULL);
#else
   fs = stdin;
#endif

   // reads buffer
   pos = 0;
   while ( ((c = getc(fs)) != EOF) && (c != '\n') && (c != '\r') )
      if (pos < (passlen-1))
         pass[pos++] = (char)c;
   pass[pos] = '\0';

   // restores previous terminal
   fprintf(stderr, "\n");
#if defined(HAVE_TERMIOS_H) || defined(HAVE_SGTTY_H)
   fflush(stderr);
   if(tcsetattr(fileno(fs), TCSANOW, &old))
      return(NULL);
   signal(SIGINT, sig);
   if (fs != stdin)
      fclose(fs);
#endif

   return(pass);
}


/* end of source */
