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
// MARK: - Headers

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
#include <errno.h>
#include <ctype.h>
#include <assert.h>


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
// MARK: - Definitions

#define BINDLE_URLDESC_SCHEME       0x01
#define BINDLE_URLDESC_USERINFO     0x02
#define BINDLE_URLDESC_HOST         0x04
#define BINDLE_URLDESC_SERVICE      0x08
#define BINDLE_URLDESC_PATH         0x10
#define BINDLE_URLDESC_QUERY        0x20
#define BINDLE_URLDESC_FRAGMENT     0x40


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
// MARK: - Prototypes

static void
bindle_urldesc_free_sockaddrs(
         BindleURLDesc *               budp );


/////////////////
//             //
//  Variables  //
//             //
/////////////////
// MARK: - Variables


/////////////////
//             //
//  Functions  //
//             //
/////////////////
// MARK: - Functions

int
bindle_is_url(
         const char *                  str )
{
   return(((bindle_urldesc_parse(str, NULL))) ? 0 : 1);
}


char *
bindle_urldesc2str(
         BindleURLDesc *               budp )
{
   size_t                  size;
   struct in6_addr         addr6;
   int                     is_ipv6addr;
   char                    port[8];
   char *                  url;

   if (budp == NULL)
   {
      errno = EINVAL;
      return(NULL);
   };

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
   if ( (!(budp->bud_service)) && ((budp->bud_port)) )
      size += (size_t)snprintf(port, sizeof(port), "%u", (unsigned)(0xffff & budp->bud_port)) + 1;

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
   if ( ((budp->bud_service)) || ((budp->bud_port)) )
   {
      bindle_strlcat(url, ":", size);
      if ((budp->bud_service))
         bindle_strlcat(url, budp->bud_service, size);
      else
         bindle_strlcat(url, port, size);
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
   unsigned          fields;
   BindleURLDesc *   budp;
   struct in6_addr   addr6;
   struct in_addr    addr;
   char              addrstr[INET_ADDRSTRLEN+INET6_ADDRSTRLEN];

   if (!(url))
      return(EINVAL);

   fields = 0;

   // allocate memory for URL desc
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

   // test if IP address
   if ( (inet_pton(AF_INET, str, &addr) == 1) || (inet_pton(AF_INET6, str, &addr6) == 1) )
   {
      if ((budpp))
      {
         budp->bud_host = buff;
         *budpp = budp;
      };
      if (!(budpp))
         free(buff);
      return(0);
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
      fields |= BINDLE_URLDESC_SCHEME;
      str = (chr[2] == '/') ? &chr[3]: &chr[1];
   };

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
      fields |= BINDLE_URLDESC_FRAGMENT;
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
      fields |= BINDLE_URLDESC_QUERY;
      chr[0] = '\0';
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
      fields |= BINDLE_URLDESC_PATH;
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
      fields |= BINDLE_URLDESC_USERINFO;
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
      fields |= BINDLE_URLDESC_HOST;
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
      if (len > 0)
      {
         if ( ((budp)) && ((budp->bud_host = bindle_strndup(str, len)) == NULL) )
         {
            free(buff);
            bindle_urldesc_free(budp);
            return(ENOMEM);
         };
      fields |= BINDLE_URLDESC_HOST;
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
      fields |= BINDLE_URLDESC_SERVICE;
      str = &str[len];
   };

   if ( ((!(fields & BINDLE_URLDESC_SCHEME)) && ((fields & BINDLE_URLDESC_QUERY))) ||
        ((!(fields & BINDLE_URLDESC_SCHEME)) && ((fields & BINDLE_URLDESC_FRAGMENT))) ||
        ((!(fields & BINDLE_URLDESC_PATH))   && ((fields & BINDLE_URLDESC_QUERY))) ||
        ((!(fields & BINDLE_URLDESC_PATH))   && ((fields & BINDLE_URLDESC_FRAGMENT))) ||
        ((!(fields & BINDLE_URLDESC_HOST))   && ((fields & BINDLE_URLDESC_USERINFO))) ||
        ((!(fields & BINDLE_URLDESC_HOST))   && ((fields & BINDLE_URLDESC_SERVICE))) )
   {
      free(buff);
      bindle_urldesc_free(budp);
      return(EINVAL);
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
   else
      bindle_urldesc_free(budp);

   return(0);
}


int
bindle_urldesc_resolve(
         BindleURLDesc *               budp,
         int                           family,
         unsigned                      default_port )
{
   int                           rc;
   size_t                        count;
   size_t                        size;
   char                          portbuf[6];
   char *                        portstr;
   unsigned                      portnum;
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

   // convert numeric port to string
   portnum = (unsigned)(((budp->bud_port)) ? budp->bud_port : default_port);
   portstr = ((portnum)) ? portbuf : NULL;
   snprintf(portbuf, sizeof(portbuf), "%u", (0xffff & portnum));

   // resolve host
   memset(&hints, 0, sizeof(hints));
   hints.ai_family   = (family == 0) ? AF_UNSPEC : family;
   hints.ai_flags    = AI_NUMERICSERV;
   hints.ai_protocol = IPPROTO_TCP;
   hints.ai_socktype = SOCK_STREAM;
   if ((rc = getaddrinfo(budp->bud_host, portstr, &hints, &res)) != 0)
      return(rc);

   // allocate list
   for(next = res, count = 0; ((next)); next = next->ai_next, count++);
   size = (count + 1) * sizeof(void *);
   bindle_urldesc_free_sockaddrs(budp);
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
