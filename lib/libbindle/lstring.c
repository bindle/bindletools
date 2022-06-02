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
#define _LIB_LIBBINDLE_LSTRING_C 1
#include "lstring.h"


///////////////
//           //
//  Headers  //
//           //
///////////////
#pragma mark - Headers

#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <time.h>
#include <ctype.h>
#include <pwd.h>
#include <grp.h>
#include <sys/utsname.h>
#include <sys/errno.h>
#include <assert.h>


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#pragma mark - Definitions


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#pragma mark - Prototypes


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#pragma mark - Functions

char *
bindle_basename(
         const char *                  path )
{
   static char bname[512];
   return(bindle_basename_r(path, bname, sizeof(bname)));
}


char *
bindle_basename_r(
         const char *                  path,
         char *                        bname,
         size_t                        bnamelen )
{
   size_t pos;
   size_t slash;
   size_t len;

   assert(bname != NULL);
   assert(bnamelen > 2);

   // handle empty strings
   if ( (!(path)) || (path[0] == '\0') )
   {
      bindle_strlcpy(bname, ".", bnamelen);
      return(bname);
   };

   // skip leading '/' chars
   while (path[0] == '/')
      path = &path[1];
   if (path[0] == '\0')
   {
      bindle_strlcpy(bname, "/", bnamelen);
      return(bname);
   };

   // find start of trailing slashes, if any
   for(pos = strlen(path) - 1; (path[pos] == '/'); pos--);
   slash = pos + 1;

   // find last slash before basename
   for(pos = slash - 1; ((pos > 0) && (path[pos] != '/')); pos--);
   if (path[pos] == '/')
      pos++;

   // copy string
   len = slash - pos + 1;
   bnamelen = (len > bnamelen) ? bnamelen : len;
   bindle_strlcpy(bname, &path[pos], bnamelen);


   return(bname);
}


char *
bindle_dirname(
         const char *                  path )
{
   static char dname[512];
   return(bindle_dirname_r(path, dname, sizeof(dname)));
}


char *
bindle_dirname_r(
         const char *                  path,
         char *                        dname,
         size_t                        dnamelen )
{
   size_t pos;
   size_t off;

   assert(dname != NULL);
   assert(dnamelen > 2);

   // handle empty strings
   if ( (!(path)) || (path[0] == '\0') )
   {
      bindle_strlcpy(dname, ".", dnamelen);
      return(dname);
   };
   if (!(strchr(path, '/')))
   {
      bindle_strlcpy(dname, ".", dnamelen);
      return(dname);
   };

   // skip leading '/' chars
   if (path[0] == '/')
   {
      while(path[1] == '/')
         path = &path[1];
      if (path[1] == '\0')
      {
         bindle_strlcpy(dname, "/", dnamelen);
         return(dname);
      };
   } else if (!(strchr(path, '/')))
   {
      bindle_strlcpy(dname, ".", dnamelen);
      return(dname);
   };

   // strip tailing '/' chars
   for(pos = strlen(path) - 1; (path[pos] == '/'); pos--);

   // strip filename
   for(; ((pos > 0) && (path[pos] != '/')); pos--);
   if (pos == 0)
   {
      if (path[0] == '/')
         bindle_strlcpy(dname, "/", dnamelen);
      else
         bindle_strlcpy(dname, ".", dnamelen);
      return(dname);
   };

   // strip trailing '/' in dirname
   for(; ((pos > 1) && (path[pos-1] == '/')); pos--);


   pos++;
   dnamelen = (pos > dnamelen) ? dnamelen : pos;
   bindle_strlcpy(dname, path, dnamelen);

   // remove double '/'
   for(pos = 0, off = 0; (dname[pos] != '\0'); pos++)
   {
      dname[off++] = dname[pos];
      if (dname[pos] == '/')
         while(dname[pos+1] == '/')
            pos++;
   };
   dname[off] = '\0';

   return(dname);
}


char
bindle_strchop(
         char *                        str )
{
   size_t   pos;
   char     c;

   BindleDebugTrace();

   if ( (!(str)) || (!(str[0])) )
      return(0);
   for(pos = 0; ((str[pos+1])); pos++);
   c        = str[pos];
   str[pos] = '\0';

   return(c);
}


char *
bindle_strdup(
         const char *                  s1 )
{
   char *      ptr;
   size_t      len;
   BindleDebugTrace();
   if (!(s1))
      return(NULL);
   len = strlen(s1);
   if ((ptr = malloc(len+2)) == NULL)
      return(NULL);
   memcpy(ptr, s1, len);
   ptr[len+0] = '\0';
   ptr[len+1] = '\0';
   return(ptr);
}


char *
bindle_strexpand(
         char *                        dst,
         const char * restrict         src,
         size_t                        len,
         int                           force_expansion )
{
   size_t            pos;
   size_t            offset;
   char              buff[4096];
   char *            ptr;
   char              quote;
   struct utsname    unam;
   struct passwd     pwd;
   struct passwd *   pwres;
   struct group      grp;
   struct group *    grres;

   BindleDebugTrace();

   assert(dst != NULL);
   assert(len  > 0);

   if (!(src))
      return(NULL);

   // expand escapes in buffer
   offset = 0;
   quote  = ((src[0] == '\'')||(src[0] == '"')) ? src[0] : 0;
   for(pos = (((quote)) ? 1 : 0); ( ((src[pos])) && (offset < (len-1)) ); pos++)
   {
      if ( (quote != '"') && (force_expansion != BNDL_YES) )
      {
         dst[offset++] = src[pos];
         continue;
      };
      switch(src[pos])
      {
         // tokens
         case '%':
         // %% - a literal percent
         // %D - domain name
         // %d - home directory
         // %G - group ID
         // %g - group name
         // %H - hostname with domain name
         // %h - hostname without domain name
         // %P - process ID
         // %p - process name
         // %U - user ID
         // %u - username
         pos++;
         switch(src[pos])
         {
            case 'D': // domain name
            if (uname(&unam) == -1)
               return(NULL);
            strncpy(buff, unam.nodename, sizeof(buff));
            if ((ptr = strchr(buff, '.')) == NULL)
               buff[0] = '\0';
            ptr = ((ptr)) ? &ptr[1] : buff;
            dst[offset] = '\0';
            bindle_strlcat(dst, ptr, len);
            offset += strlen(ptr);
            break;

            case 'd': // home directory
            getpwuid_r(getuid(), &pwd, buff, sizeof(buff), &pwres);
            if (!(pwres))
               break;
            dst[offset] = '\0';
            bindle_strlcat(dst, pwres->pw_dir, len);
            offset += strlen(pwres->pw_dir);
            break;

            case 'G': // gid
            snprintf(buff, sizeof(buff), "%u", getgid());
            dst[offset] = '\0';
            bindle_strlcat(dst, buff, len);
            offset += strlen(buff);
            break;

            case 'g': // group name
            getgrgid_r(getgid(), &grp, buff, sizeof(buff), &grres);
            if (!(grres))
               break;
            dst[offset] = '\0';
            bindle_strlcat(dst, grres->gr_name, len);
            offset += strlen(grres->gr_name);
            break;

            case 'H': // fully qualified hostname
            if (uname(&unam) == -1)
               break;
            dst[offset] = '\0';
            bindle_strlcat(dst, unam.nodename, len);
            offset += strlen(unam.nodename);
            break;

            case 'h': // short hostname
            if (uname(&unam) == -1)
               break;
            strncpy(buff, unam.nodename, sizeof(buff));
            if ((ptr = strchr(buff, '.')) != NULL)
               ptr[0] = '\0';
            dst[offset] = '\0';
            bindle_strlcat(dst, buff, len);
            offset += strlen(buff);
            break;

            case 'P': // process ID
            snprintf(buff, sizeof(buff), "%u", getppid());
            dst[offset] = '\0';
            bindle_strlcat(dst, buff, len);
            offset += strlen(buff);
            break;

            case 'p': // process name/ident
            dst[offset] = '\0';
            bindle_strlcat(dst, bindle_debug_ident, len);
            offset += strlen(bindle_debug_ident);
            break;

            case 'U': // uid
            snprintf(buff, sizeof(buff), "%u", getgid());
            dst[offset] = '\0';
            bindle_strlcat(dst, buff, len);
            offset += strlen(buff);
            break;

            case 'u': // username
            getpwuid_r(getuid(), &pwd, buff, sizeof(buff), &pwres);
            if (!(pwres))
               break;
            dst[offset] = '\0';
            bindle_strlcat(dst, pwres->pw_name, len);
            offset += strlen(pwres->pw_name);
            break;

            case '%': // '%' character
            dst[offset++] = src[pos];
            break;

            default:
            dst[offset++] = src[pos];
         };
         break;

         // character escape sequences
         case '\\':
         pos++;
         switch(src[pos])
         {
            case '"':
            case '\'':
            case '\\':
            case '\?':
            dst[offset++] = src[pos];
            break;

            case 'n': dst[offset++] = '\n'; break;
            case 'r': dst[offset++] = '\r'; break;
            case 't': dst[offset++] = '\t'; break;
            case 'v': dst[offset++] = '\v'; break;

            // \xnn - treat nn as hex value
            case 'x':
            buff[0] = src[++pos];
            buff[1] = src[++pos];
            buff[2] = '\0';
            dst[offset++] = (char)strtoul(buff, &ptr, 16);
            if (ptr != &buff[2])
               return(NULL);
            break;

            // \nnn - treat nnn as octal value
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            buff[0] = src[pos++];
            buff[1] = src[pos++];
            buff[2] = src[pos];
            buff[3] = '\0';
            dst[offset++] = (char)strtoul(buff, &ptr, 8);
            if (ptr != &buff[3])
               return(NULL);
            break;

            default:
            dst[offset++] = src[pos];
         };
         break;

         default:
         dst[offset++] = src[pos];
         break;
      };
   };
   if ((quote))
      offset--;
   if (offset >= len)
   {
      dst[len-1] = '\0';
      return(NULL);
   };
   dst[offset] = '\0';

   return(dst);
}


size_t
bindle_strlcpy(
         char * restrict               dst,
         const char * restrict         src,
         size_t                        dstsize )
{
   size_t len;
   BindleDebugTrace();
   assert(dst     != NULL);
   assert(src     != NULL);
   assert(dstsize  > 0);
   for(len = 0; ((src[len])); len++)
      if (len < dstsize)
         dst[len] = src[len];
   dst[((len < dstsize) ? len : (dstsize-1))] = '\0';
   return(len);
}


size_t
bindle_strlcat(
         char * restrict               dst,
         const char * restrict         src,
         size_t                        dstsize )
{
   size_t      pos;
   size_t      offset;
   size_t      len;
   BindleDebugTrace();
   assert(dst     != NULL);
   assert(src     != NULL);
   assert(dstsize  > 0);
   for(pos = 0; ((dst[pos])); pos++);
   for(offset = 0; ((src[offset])); offset++)
      if ((pos + offset) < dstsize)
         dst[pos+offset] = src[offset];
   len = offset + pos;
   dst[((len < dstsize) ? len : (dstsize-1))] = '\0';
   return(len);
}


char *
bindle_strndup(
         const char *                  s1,
         size_t                        n )
{
   char *      ptr;
   size_t      len;
   BindleDebugTrace();
   if (!(s1))
      return(NULL);
   len = strlen(s1);
   len = (len < n) ? len : n;
   if ((ptr = malloc(len+2)) == NULL)
      return(NULL);
   memcpy(ptr, s1, len);
   ptr[len+0] = '\0';
   ptr[len+1] = '\0';
   return(ptr);
}


int
bindle_strtoargs(
         char *                        str,
         char ***                      argvp,
         int *                         argcp )
{
   char           quote;
   char *         bol;
   char **        argv;
   size_t         pos;
   int            rc;
   int            eol;

   BindleDebugTrace();

   assert(str != NULL);

   argv = NULL;
   eol  = 0;

   for(pos = 0; ( ((str[pos])) && (!(eol)) ); pos++)
   {
      switch(str[pos])
      {
         // skip white space
         case ' ':
         case '\t':
         break;

         // parse quoted argument
         case '"':
         case '\'':
         quote = str[pos];
         bol   = &str[pos];
         for(pos += 1; ((str[pos] != '\0') && (str[pos] != quote)); pos++)
            if ( (quote == '"') && (str[pos] == '\\') && (str[pos+1] == '"') )
               pos++;
         if (str[pos] != quote)
         {
            bindle_strsfree(argv);
            return(EINVAL);
         };
         pos++;
         switch(str[pos])
         {
            case '\0':
            case '#':
            eol = 1;
            str[pos] = '\0';
            break;

            case ' ':
            case '\t':
            str[pos] = '\0';
            break;

            default:
            bindle_strsfree(argv);
            return(EINVAL);
         };
         if ((rc = bindle_strsadd(&argv, bol)) != BNDL_SUCCESS)
         {
            bindle_strsfree(argv);
            return(rc);
         };
         break;

         // parse unquoted argument
         default:
         bol = &str[pos];
         while( (str[pos] != ' ') && (str[pos] != '\t') && (str[pos] != '\0') && (str[pos] != '#') )
         {
            if ( (str[pos] < '!') || (str[pos] > '~') || (str[pos] == '"') || (str[pos] == '\'') )
            {
               bindle_strsfree(argv);
               return(EINVAL);
            };
            pos++;
         };
         if ((str[pos] == '#') || (str[pos] == '\0'))
            eol = 1;
         str[pos] = '\0';
         if ((bol[0]))
         {
            if ((rc = bindle_strsadd(&argv, bol)) != BNDL_SUCCESS)
            {
               bindle_strsfree(argv);
               return(rc);
            };
         };
         break;
      };
   };

   if ((argcp))
      *argcp = (int)bindle_strscount(argv);
   if ((argvp))
      *argvp = argv;
   if (!(argvp))
      bindle_strsfree(argv);

   return(BNDL_SUCCESS);
}


int
bindle_strtobool(
         const char *                  str )
{
   if      (!(str))                       return(-1);
   else if (!(strcasecmp(str, "1")))      return(BNDL_YES);
   else if (!(strcasecmp(str, "true")))   return(BNDL_YES);
   else if (!(strcasecmp(str, "yes")))    return(BNDL_YES);
   else if (!(strcasecmp(str, "0")))      return(BNDL_NO);
   else if (!(strcasecmp(str, "false")))  return(BNDL_NO);
   else if (!(strcasecmp(str, "no")))     return(BNDL_NO);
   return(-1);
}


char *
bindle_strtrim(
         char *                        str )
{
   ssize_t offset;
   ssize_t pos;

   BindleDebugTrace();

   if (!(str))
      return(str);

   // find end of whitespace
   for(offset = 0; ( ((str[offset])) && ((isspace(str[offset]))) ); offset++);

   // shift string by offset
   for(pos = offset; ((str[pos])); pos++)
      str[pos-offset] = str[pos];
   if (!(pos))
      return(str);

   // trim end of line
   for(pos = pos - 1 - offset; ( ((isspace(str[pos]))) && (pos > 0) ); pos--);
   str[pos+1] = '\0';

   return(str);
}


/* end of source */
