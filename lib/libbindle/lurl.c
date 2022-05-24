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
#define _LIB_LIBBINDLE_LURL_C 1
#include "lurl.h"


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
#include <syslog.h>
#include <stdatomic.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/errno.h>
#include <ctype.h>
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

static char
bindle_url_encode_char(
         char                          c );


static void
bindle_urldesc_free_sockaddrs(
         BindleURLDesc *               budp );


/////////////////
//             //
//  Variables  //
//             //
/////////////////
#pragma mark - Variables


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#pragma mark - Functions

int
bindle_is_url(
         const char *                  str )
{
   return(((bindle_urldesc_parse(str, NULL))) ? 0 : 1);
}


ssize_t
bindle_url_decode(
         const char *                  src,
         char * restrict               dst,
         size_t                        size )
{
   size_t      pos;
   size_t      off;
   char        hex[3];

   assert(src != NULL);
   assert(dst != NULL);

   // adjust for NULL termination
   size--;

   for(pos = 0, off = 0; (((src[pos])) && (off < size)); pos++, off++)
   {
      if (src[pos] == '+')
      {
         dst[off] = ' ';
         continue;
      };
      if (src[pos] != '%')
      {
         dst[off] = src[pos];
         continue;
      };
      if ( (!(isxdigit(src[pos+1]))) && (!(isxdigit(src[pos+2]))) )
      {
         dst[off] = '\0';
         return(-1);
      };
      hex[0]   = src[pos+1];
      hex[1]   = src[pos+2];
      hex[2]   = '\0';
      dst[off] = strtol(hex, NULL, 16);
      pos += 2;
   };
   dst[off] = '\0';

   return((ssize_t)off);
}


ssize_t
bindle_url_encode(
         const char *                  src,
         char * restrict               dst,
         size_t                        size )
{
   size_t      pos;
   size_t      off;

   assert(src != NULL);
   assert(dst != NULL);

   // adjust for NULL termination
   size--;

   for(pos = 0, off = 0; (((src[pos])) && (off < size)); pos++, off++)
   {
      if ((dst[off] = bindle_url_encode_char(src[pos])) != 0)
         continue;
      if ((off+3) >= size)
      {
         dst[off] = '\0';
         return(-1);
      };
      snprintf(&dst[off], 4, "%%%02x", src[pos]);
      off += 2;
   };
   dst[off] = '\0';

   return((ssize_t)off);
}


char
bindle_url_encode_char(
         char                          c )
{
   if ((isalnum(c)))
      return(c);
   switch(c)
   {
      case '.':
      case ',':
      case ';':
      case '_':
      case '-':
      case '~':
      case ':':
      case '@':
      case '/':
      return(c);

      case ' ':
      return('+');

      default:
      break;
   };
   return(0);
}


size_t
bindle_url_encode_len(
         const char *                  str )
{
   size_t      size;
   size_t      pos;

   if (!(str))
      return(0);

   for(pos = 0, size = 0; ((str[pos])); pos++, size++)
      if (!(bindle_url_encode_char(str[pos])))
         size += 2;

   return(size);
}


char *
bindle_urldesc2str(
         BindleURLDesc *               budp )
{
   size_t                  size;
   struct in6_addr         addr6;
   int                     is_ipv6addr;
   char *                  url;

   if (budp == NULL)
      return(NULL);

   // determine if host is an IPv6 address
   is_ipv6addr = 0;
   if ((budp->bud_host))
      if (inet_pton(AF_INET6, budp->bud_host, &addr6) == 1)
         is_ipv6addr = 1;

   // determine length of string
   size  = 1;
   size += ((budp->bud_scheme))   ? strlen(budp->bud_scheme)   + 3 : 0;
   size += ((budp->bud_userinfo)) ? strlen(budp->bud_userinfo) + 1 : 0;
   size += ((budp->bud_host))     ? strlen(budp->bud_host)     + 0 : 0;
   size += ((is_ipv6addr))        ? 2                              : 0;
   size += ((budp->bud_service))  ? strlen(budp->bud_service)  + 1 : 0;
   size += ((budp->bud_path))     ? strlen(budp->bud_path)     + 0 : 0;
   size += ((budp->bud_query))    ? strlen(budp->bud_query)    + 1 : 0;
   size += ((budp->bud_fragment)) ? strlen(budp->bud_fragment) + 1 : 0;

   // allocates memory for string
   if ((url = malloc(size)) == NULL)
      return(NULL);
   memset(url, 0, size);

   // copies URL components to string
   if ((budp->bud_scheme))
   {
      bindle_strlcat(url, budp->bud_scheme, size);
      bindle_strlcat(url, "://", size);
   };
   if ((budp->bud_userinfo))
   {
      bindle_strlcat(url, budp->bud_userinfo, size);
      bindle_strlcat(url, "@", size);
   };
   if ((budp->bud_host))
   {
      bindle_strlcat(url, (((is_ipv6addr)) ? "[" : ""), size);
      bindle_strlcat(url, budp->bud_host, size);
      bindle_strlcat(url, (((is_ipv6addr)) ? "]" : ""), size);
   };
   if ((budp->bud_service))
   {
      bindle_strlcat(url, ":", size);
      bindle_strlcat(url, budp->bud_service, size);
   };
   if ((budp->bud_path))
      bindle_strlcat(url, budp->bud_path, size);
   if ((budp->bud_query))
   {
      bindle_strlcat(url, "?", size);
      bindle_strlcat(url, budp->bud_query, size);
   };
   if ((budp->bud_fragment))
   {
      bindle_strlcat(url, "#", size);
      bindle_strlcat(url, budp->bud_fragment, size);
   };

   return(url);
}


void
bindle_urldesc_free(
         BindleURLDesc *               budp )
{
   if (!(budp))
      return;

   if ((budp->bud_scheme))    free(budp->bud_scheme);
   if ((budp->bud_userinfo))  free(budp->bud_userinfo);
   if ((budp->bud_host))      free(budp->bud_host);
   if ((budp->bud_service))   free(budp->bud_service);
   if ((budp->bud_path))      free(budp->bud_path);
   if ((budp->bud_query))     free(budp->bud_query);
   if ((budp->bud_fragment))  free(budp->bud_fragment);

   bindle_urldesc_free_sockaddrs(budp);

   memset(budp, 0, sizeof(BindleURLDesc));
   free(budp);

   return;
}


void
bindle_urldesc_free_sockaddrs(
         BindleURLDesc *               budp )
{
   struct sockaddr **      sas;
   unsigned                pos;

   if (!(budp))
      return;
   if (!(budp->bud_sockaddrs))
      return;

   sas = (struct sockaddr **)budp->bud_sockaddrs;
   for(pos = 0; ((sas[pos])); pos++)
      free(sas[pos]);

   free(budp->bud_sockaddrs);
   budp->bud_sockaddrs = NULL;

   return;
}


int
bindle_urldesc_parse(
         const char *                  url,
         BindleURLDesc **              budpp )
{
   size_t            pos;
   size_t            len;
   char *            buff;
   char *            str;
   char *            chr;
   char *            endptr;
   unsigned          port;
   BindleURLDesc *   budp;
   struct in6_addr   addr6;
   char              addrstr[INET_ADDRSTRLEN+INET6_ADDRSTRLEN];

   if (!(url))
      return(EINVAL);

   budp = NULL;
   if ((budpp))
   {
      if ((budp = malloc(sizeof(BindleURLDesc))) == NULL)
         return(ENOMEM);
      memset(budp, 0, sizeof(BindleURLDesc));
   };
   if ((buff = bindle_strdup(url)) == NULL)
   {
      bindle_urldesc_free(budp);
      return(ENOMEM);
   };
   str = buff;

   // process fragment
   if ((chr = strchr(str, '#')) != NULL)
   {
      for(pos = 1; ((chr[pos])); pos++)
      {
         switch(chr[pos])
         {
            case '#':
            case '[':
            case ']':
            free(buff);
            bindle_urldesc_free(budp);
            return(EINVAL);

            case '%':
            if ( (!(isxdigit(chr[pos+1]))) || (!(isxdigit(chr[pos+2]))) )
            {
               free(buff);
               bindle_urldesc_free(budp);
               return(EINVAL);
            };
            break;

            default:
            break;
         };
      };
      if ((budp))
      {
         if ((budp->bud_fragment = bindle_strdup(&chr[1])) == NULL)
         {
            free(buff);
            bindle_urldesc_free(budp);
            return(ENOMEM);
         };
      };
      chr[0] = '\0';
   };

   // process query
   if ((chr = strchr(str, '?')) != NULL)
   {
      for(pos = 1; ((chr[pos])); pos++)
      {
         switch(chr[pos])
         {
            case '#':
            case '[':
            case ']':
            free(buff);
            bindle_urldesc_free(budp);
            return(EINVAL);

            case '%':
            if ( (!(isxdigit(chr[pos+1]))) || (!(isxdigit(chr[pos+2]))) )
            {
               free(buff);
               bindle_urldesc_free(budp);
               return(EINVAL);
            };
            break;

            default:
            break;
         };
      };
      if ((budp))
      {
         if ((budp->bud_query = bindle_strdup(&chr[1])) == NULL)
         {
            free(buff);
            bindle_urldesc_free(budp);
            return(ENOMEM);
         };
      };
      chr[0] = '\0';
   };

   // process scheme
   //    Valid schemes include:
   //       scheme:/path (no hostname)
   //       scheme:///path (empty hostname)
   //       scheme://hostname/path
   if ( ((chr = strchr(str, ':')) != NULL) && (chr[1] == '/') )
   {
      len = (size_t)(chr - str);
      // checks syntax
      if (!(isalpha(str[0])))
      {
         free(buff);
         bindle_urldesc_free(budp);
         return(EINVAL);
      };
      for(pos = 1; (pos < len); pos++)
      {
         if ( (!(isalnum(str[0]))) && (str[pos] != '+') && (str[pos] != '.') && (str[pos] != '-') )
         {
            free(buff);
            bindle_urldesc_free(budp);
            return(EINVAL);
         };
      };
      // copies scheme
      if ((budp))
      {
         if ((budp->bud_scheme = bindle_strndup(str, len)) == NULL)
         {
            free(buff);
            bindle_urldesc_free(budp);
            return(ENOMEM);
         };
      };
      // shift string
      str = (chr[2] == '/') ? &chr[3]: &chr[1];
   };

   // process path
   if ((chr = strchr(str, '/')) != NULL)
   {
      for(pos = 1; ((chr[pos])); pos++)
      {
         switch(chr[pos])
         {
            case '#':
            case '[':
            case ']':
            case '?':
            free(buff);
            bindle_urldesc_free(budp);
            return(EINVAL);

            case '%':
            if ( (!(isxdigit(chr[pos+1]))) || (!(isxdigit(chr[pos+2]))) )
            {
               free(buff);
               bindle_urldesc_free(budp);
               return(EINVAL);
            };
            break;

            default:
            break;
         };
      };
      if ((budp))
      {
         if ((budp->bud_path = bindle_strdup(chr)) == NULL)
         {
            free(buff);
            bindle_urldesc_free(budp);
            return(ENOMEM);
         };
      };
      chr[0] = '\0';
   };

   // process userinfo
   if ((chr = strchr(str, '@')) != NULL)
   {
      for(pos = 0; (str[pos] != '@'); pos++)
      {
         switch(str[pos])
         {
            case '/':
            case '?':
            case '#':
            case '[':
            case ']':
            free(buff);
            bindle_urldesc_free(budp);
            return(EINVAL);

            case '%':
            if ( (!(isxdigit(str[pos+1]))) || (!(isxdigit(str[pos+2]))) )
            {
               free(buff);
               bindle_urldesc_free(budp);
               return(EINVAL);
            };
            break;

            default:
            break;
         };
      };
      len = (size_t)(chr - str);
      if ((budp))
      {
         if ((budp->bud_userinfo = bindle_strndup(str, len)) == NULL)
         {
            free(buff);
            bindle_urldesc_free(budp);
            return(ENOMEM);
         };
      };
      str = &chr[1];
   };

   // process host
   if (str[0] == '[')
   {
      // process IPv6 addresses
      str = &str[1];
      if ((chr = strchr(str, ']')) == NULL)
      {
         free(buff);
         bindle_urldesc_free(budp);
         return(EINVAL);
      };
      len = (size_t)(chr - str);
      if ((len < 2) || (len >= sizeof(addrstr)))
      {
         free(buff);
         bindle_urldesc_free(budp);
         return(EINVAL);
      };
      bindle_strlcpy(addrstr, str, (len+1));
      if ((budp))
      {
         if ((budp->bud_host = bindle_strndup(str, len)) == NULL)
         {
            free(buff);
            bindle_urldesc_free(budp);
            return(ENOMEM);
         };
      };
      if (inet_pton(AF_INET6, addrstr, &addr6) != 1)
      {
         free(buff);
         bindle_urldesc_free(budp);
         return(EINVAL);
      };
      str = &chr[1];
   } else {
      // process hostnames and IPv4 addresses
      for(len = 0; (((isalnum(str[len]))) || (str[len] == '.') || (str[len] == '-')); len++)
      {
         if (!(len))
            continue;
         if ((isalnum(str[len-1])))
            continue;
         if ((str[len] == '.') || (str[len] == '-'))
         {
            free(buff);
            bindle_urldesc_free(budp);
            return(EINVAL);
         };
      };
      if ( ((budp)) && (len > 0) )
      {
         if ((budp->bud_host = bindle_strndup(str, len)) == NULL)
         {
            free(buff);
            bindle_urldesc_free(budp);
            return(ENOMEM);
         };
      };
      str = &str[len];
   };

   // process port
   if (str[0] == ':')
   {
      str = &str[1];
      // checks syntax
      for(len = 0; ((str[len])); len++)
      {
         if ( (!(isalnum(str[len]))) && (str[len] != '.') && (str[len] != '-') && (str[len] != '_') && (str[len] != '+') )
         {
            free(buff);
            bindle_urldesc_free(budp);
            return(EINVAL);
         };
      };
      if ( len == 0)
      {
         free(buff);
         bindle_urldesc_free(budp);
         return(EINVAL);
      };
      // convert numeric service to port number
      port = (unsigned)strtoul(str, &endptr, 10);
      if ( (endptr == str) || (endptr[0] != '\0') )
         port = 0;
      // copies service
      if ((budp))
      {
         budp->bud_port = port;
         if ((budp->bud_service = bindle_strdup(str)) == NULL)
         {
            free(buff);
            bindle_urldesc_free(budp);
            return(ENOMEM);
         };
      };
      str = &str[len];
   };

   if (str[0] != '\0')
   {
      free(buff);
      bindle_urldesc_free(budp);
      return(EINVAL);
   };

   free(buff);
   if ((budpp))
      *budpp = budp;

   return(0);
}


int
bindle_urldesc_resolve(
         BindleURLDesc *               budp,
         int                           family )
{
   int                           rc;
   size_t                        count;
   size_t                        size;
   struct addrinfo               hints;
   struct addrinfo *             res;
   struct addrinfo *             next;
   struct sockaddr_in            sa;
   struct sockaddr_in6           sa6;

   // resovle port
   if ( ((budp->bud_service)) && (!(budp->bud_port)) )
   {
      if (getaddrinfo(NULL, budp->bud_service, NULL, &res) == 0)
      {
         if (res->ai_family == AF_INET)
         {
            memcpy(&sa, res->ai_addr, sizeof(struct sockaddr_in));
            budp->bud_port = ntohs(sa.sin_port);
         };
         if (res->ai_family == AF_INET6)
         {
            memcpy(&sa6, res->ai_addr, sizeof(struct sockaddr_in6));
            budp->bud_port = ntohs(sa6.sin6_port);
         };
         freeaddrinfo(res);
      };
   };

   // resolve host
   memset(&hints, 0, sizeof(hints));
   hints.ai_family   = (family == 0) ? AF_UNSPEC : family;
   hints.ai_flags    = 0;
   hints.ai_protocol = IPPROTO_TCP;
   hints.ai_socktype = SOCK_STREAM;
   if ((rc = getaddrinfo(budp->bud_host, NULL, &hints, &res)) != 0)
      return(rc);

   // allocate list
   for(next = res, count = 0; ((next)); next = next->ai_next, count++);
   size = (count + 1) * sizeof(void *);
   if ((budp->bud_sockaddrs = malloc(size)) == NULL)
   {
      freeaddrinfo(res);
      return(EAI_MEMORY);
   };
   memset(budp->bud_sockaddrs, 0, size);

   // save results
   for(next = res, count = 0; ((next)); next = next->ai_next, count++)
   {
      if ((budp->bud_sockaddrs[count] = malloc(next->ai_addrlen)) == NULL)
      {
         freeaddrinfo(res);
         bindle_urldesc_free_sockaddrs(budp);
         return(EAI_MEMORY);
      };
      memcpy(budp->bud_sockaddrs[count], next->ai_addr, next->ai_addrlen);
   };

   freeaddrinfo(res);

   return(0);
}

/* end of source */
