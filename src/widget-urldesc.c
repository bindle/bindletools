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
#define _SRC_WIDGET_URLDESC_C 1
#include "bindletools.h"


///////////////
//           //
//  Headers  //
//           //
///////////////
// MARK: - Headers

#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <arpa/inet.h>


//////////////
//          //
//  Macros  //
//          //
//////////////
// MARK: - Macros


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
// MARK: - Definitions

#define BINDLE_FLG_IPV4       0x00000100UL
#define BINDLE_FLG_IPV6       0x00000200UL
#define BINDLE_FLG_NORESOLVE  0x00000400UL
#define BINDLE_FLG_RESOLVE    0x00000800UL
#define BINDLE_FLG_IO_SHELL   0x00001000UL
#define BINDLE_FLG_SCHEME     0x00002000UL
#define BINDLE_FLG_USERINFO   0x00004000UL
#define BINDLE_FLG_HOST       0x00008000UL
#define BINDLE_FLG_SERVICE    0x00010000UL
#define BINDLE_FLG_PORT       0x00020000UL
#define BINDLE_FLG_PATH       0x00040000UL
#define BINDLE_FLG_FRAGMENT   0x00080000UL
#define BINDLE_FLG_QUERY      0x00100000UL
#define BINDLE_FLG_SUPPRESS   0x00200000UL
#define BINDLE_FLG_IO_SHORT   0x00400000UL
#define BINDLE_FLG_ADDRESS    0x00800000UL
#define BINDLE_FLG_IO_ALL     ( BINDLE_FLG_IO_SHORT | BINDLE_FLG_IO_SHELL )
#define BINDLE_FLG_ALL        (  BINDLE_FLG_SCHEME | BINDLE_FLG_USERINFO | \
                                 BINDLE_FLG_HOST | BINDLE_FLG_SERVICE | \
                                 BINDLE_FLG_PORT | BINDLE_FLG_PATH | \
                                 BINDLE_FLG_FRAGMENT | BINDLE_FLG_QUERY | \
                                 BINDLE_FLG_ADDRESS )


//////////////////
//              //
//  Data Types  //
//              //
//////////////////
// MARK: - Data Types


/////////////////
//             //
//  Variables  //
//             //
/////////////////
// MARK: - Variables

// MARK: bindle_widget_urldesc_options[]
const char * const bindle_widget_urldesc_options[] =
{
   "  -4                        resolve host to IPv4 address",
   "  -6                        resolve host to IPv6 address",
   "  -n, --dryrun              do not resolve hostname",
   "  -S, --service             print URL service",
   "  -s, --scheme              print URL scheme",
   "  -H, --host                print URL host",
   "  -u, --userinfo            print URL userinfo",
   "  -p, --port                print URL port",
   "  -P, --path                print URL path",
   "  -Q, --query               print URL query",
   "  -r, --resolve             resolve hostname",
   "  -f, --fragment            print URL fragment",
   "  --all                     print all values, even if empty",
   "  --short                   print only component values",
   "  --shell                   print components as shell variables",
   NULL
};


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
// MARK: - Prototypes

static void
bindle_widget_urldesc_print(
         bindle_conf_t *               cnf,
         const char *                  component,
         char **                       valuep,
         uintptr_t                     flag );


void
bindle_widget_urldesc_addr(
         bindle_conf_t *               cnf,
         const char *                  component,
         void **                       sockaddrs,
         uintptr_t                     flag );


void
bindle_widget_urldesc_unsigned(
         bindle_conf_t *               cnf,
         const char *                  component,
         uintptr_t                     value,
         uintptr_t                     flag );


/////////////////
//             //
//  Functions  //
//             //
/////////////////
// MARK: - Functions

int
bindle_widget_urldesc(
         bindle_conf_t *               cnf )
{
   int               c;
   int               rc;
   int               opt_index;
   const char *      url;
   BindleURLDesc *   budp;

   // getopt options
   static const char *  short_opt = BINDLE_COMMON_SHORT "1246AafHnPpQrSsu";
   static struct option long_opt[] =
   {
      BINDLE_COMMON_LONG,
      { "all",             no_argument,      NULL, 'A' },
      { "address",         no_argument,      NULL, 'a' },
      { "dryrun",          no_argument,      NULL, 'n' },
      { "fragment",        no_argument,      NULL, 'f' },
      { "host",            no_argument,      NULL, 'H' },
      { "path",            no_argument,      NULL, 'P' },
      { "port",            no_argument,      NULL, 'p' },
      { "query",           no_argument,      NULL, 'Q' },
      { "resolve",         no_argument,      NULL, 'r' },
      { "scheme",          no_argument,      NULL, 's' },
      { "service",         no_argument,      NULL, 'S' },
      { "short",           no_argument,      NULL, '1' },
      { "shell",           no_argument,      NULL, '2' },
      { "userinfo",        no_argument,      NULL, 'u' },
      { NULL, 0, NULL, 0 }
   };

   optind    = 1;
   opt_index = 0;

   while((c = bindle_getopt(cnf, cnf->argc, cnf->argv, short_opt, long_opt, &opt_index)) != -1)
   {
      switch(c)
      {
         case -2:       /* --help or --version */
         return(0);

         case -1:       /* no more arguments */
         case 0:        /* long options toggles */
         break;

         case 1:        /* error processing argument */
         return(1);

         case '1':
         cnf->widget_flags &= ~BINDLE_FLG_IO_ALL;
         cnf->widget_flags |= BINDLE_FLG_IO_SHORT;
         break;

         case '2':
         cnf->widget_flags &= ~BINDLE_FLG_IO_ALL;
         cnf->widget_flags |= BINDLE_FLG_IO_SHELL;
         break;

         case '4':
         cnf->widget_flags |=   BINDLE_FLG_IPV4 | BINDLE_FLG_RESOLVE;
         cnf->widget_flags &= ~(BINDLE_FLG_IPV6 | BINDLE_FLG_NORESOLVE);
         break;

         case '6':
         cnf->widget_flags |=   BINDLE_FLG_IPV6 | BINDLE_FLG_RESOLVE;
         cnf->widget_flags &= ~(BINDLE_FLG_IPV4 | BINDLE_FLG_NORESOLVE);
         break;

         case 'A':
         cnf->widget_flags |= (BINDLE_FLG_ALL | BINDLE_FLG_RESOLVE);
         break;

         case 'a':
         cnf->widget_flags |= (BINDLE_FLG_ADDRESS | BINDLE_FLG_RESOLVE);
         break;

         case 'f':
         cnf->widget_flags |= BINDLE_FLG_FRAGMENT;
         break;

         case 'H':
         cnf->widget_flags |=  BINDLE_FLG_HOST;
         break;

         case 'n':
         cnf->widget_flags |=  BINDLE_FLG_NORESOLVE;
         cnf->widget_flags &= ~BINDLE_FLG_RESOLVE;
         break;

         case 'P':
         cnf->widget_flags |=  BINDLE_FLG_PATH;
         break;

         case 'p':
         cnf->widget_flags |=  BINDLE_FLG_PORT;
         break;

         case 'Q':
         cnf->widget_flags |=  BINDLE_FLG_QUERY;
         break;

         case 'r':
         cnf->widget_flags |=  BINDLE_FLG_RESOLVE;
         cnf->widget_flags &= ~BINDLE_FLG_NORESOLVE;
         break;

         case 'S':
         cnf->widget_flags |=  BINDLE_FLG_SERVICE;
         break;

         case 's':
         cnf->widget_flags |=  BINDLE_FLG_SCHEME;
         break;

         case 'u':
         cnf->widget_flags |=  BINDLE_FLG_USERINFO;
         break;

         case '?':
         fprintf(stderr, "Try `%s --help' for more information.\n", PROGRAM_NAME);
         return(1);

         default:
         fprintf(stderr, "%s: unrecognized option `--%c'\n", PROGRAM_NAME, c);
         fprintf(stderr, "Try `%s --help' for more information.\n", PROGRAM_NAME);
         return(1);
      };
   };
   if ((cnf->argc - optind) < 1)
   {
      fprintf(stderr, "%s: missing required argument\n", cnf->prog_name);
      fprintf(stderr, "Try `%s --help' for more information.\n", cnf->prog_name);
      return(1);
   };
   url = cnf->argv[optind];
   optind++;
   if ((cnf->argc - optind) > 0)
   {
      fprintf(stderr, "%s: unknown argument -- `%s'\n", cnf->prog_name, cnf->argv[optind]);
      fprintf(stderr, "Try `%s --help' for more information.\n", cnf->prog_name);
      return(1);
   };
   if ((cnf->widget_flags & BINDLE_FLG_NORESOLVE))
      cnf->widget_flags &= ~BINDLE_FLG_RESOLVE;
   if ((cnf->widget_flags & BINDLE_FLG_ALL) == 0)
      cnf->widget_flags |= BINDLE_FLG_ALL | BINDLE_FLG_SUPPRESS;

   if ((rc = bindle_urldesc_parse(url, &budp)) != 0)
   {
      fprintf(stderr, "%s: bindle_urldesc_parse(): %s\n", cnf->prog_name, strerror(rc));
      return(1);
   };
   if ((cnf->widget_flags & BINDLE_FLG_RESOLVE))
   {
      if ((cnf->widget_flags & BINDLE_FLG_IPV4))
         bindle_urldesc_resolve(budp, AF_INET, 0);
      else if ((cnf->widget_flags & BINDLE_FLG_IPV6))
         bindle_urldesc_resolve(budp, AF_INET6, 0);
      else
         bindle_urldesc_resolve(budp, 0, 0);
   };

   bindle_widget_urldesc_print(cnf, "scheme",      &budp->bud_scheme,      BINDLE_FLG_SCHEME);
   bindle_widget_urldesc_print(cnf, "userinfo",    &budp->bud_userinfo,    BINDLE_FLG_USERINFO);
   bindle_widget_urldesc_addr( cnf, "address",      budp->bud_sockaddrs,   BINDLE_FLG_ADDRESS);
   bindle_widget_urldesc_print(cnf, "host",        &budp->bud_host,        BINDLE_FLG_HOST);
   bindle_widget_urldesc_print(cnf, "service",     &budp->bud_service,     BINDLE_FLG_SERVICE);
   bindle_widget_urldesc_unsigned(cnf, "port",     budp->bud_port,         BINDLE_FLG_PORT);
   bindle_widget_urldesc_print(cnf, "path",        &budp->bud_path,        BINDLE_FLG_PATH);
   bindle_widget_urldesc_print(cnf, "fragment",    &budp->bud_fragment,    BINDLE_FLG_FRAGMENT);
   bindle_widget_urldesc_print(cnf, "query",       &budp->bud_query,       BINDLE_FLG_QUERY);

   bindle_urldesc_free(budp);

   return(0);
}


void
bindle_widget_urldesc_print(
         bindle_conf_t *               cnf,
         const char *                  component,
         char **                       valuep,
         uintptr_t                     flag )
{
   const char *   value;
   char           field[64];
   size_t         pos;

   if (!(cnf->widget_flags & flag))
      return;
   if ( (!(*valuep)) && ((cnf->widget_flags & BINDLE_FLG_SUPPRESS)) )
      return;
   value = ((*valuep)) ? *valuep : "";

   switch (cnf->widget_flags & BINDLE_FLG_IO_ALL)
   {
      case BINDLE_FLG_IO_SHORT:
      printf("%s\n", value);
      break;

      case BINDLE_FLG_IO_SHELL:
      bindle_strlcpy(field, "URL_", sizeof(field));
      bindle_strlcat(field, component, sizeof(field));
      for(pos = 0; ((field[pos])); pos++)
         field[pos] = toupper(field[pos]);
      printf("%s=\"%s\"\n", field, value);
      break;

      default:
      bindle_strlcpy(field, component, sizeof(field));
      bindle_strlcat(field, ":", sizeof(field));
      printf("%-12s %s\n", field, value);
      break;
   };

   return;
}


void
bindle_widget_urldesc_addr(
         bindle_conf_t *               cnf,
         const char *                  component,
         void **                       sockaddrs,
         uintptr_t                     flag )
{
   unsigned                   pos;
   char                       field[64];
   char                       addr[INET_ADDRSTRLEN+INET6_ADDRSTRLEN];
   char *                     ptr;
   struct sockaddr_storage *  sas;
   struct sockaddr_in *       sa;
   struct sockaddr_in6 *      sa6;

   if (!(sockaddrs))
   {
      ptr = NULL;
      bindle_widget_urldesc_print(cnf, component, &ptr, flag);
      return;
   };

   for(pos = 0; ((sockaddrs[pos])); pos++)
   {
      ptr = addr;
      sas = sockaddrs[pos];
      bindle_strlcpy(field, component, sizeof(field));
      if ((pos))
         snprintf(field, sizeof(field), "%s%u", component, pos);
      switch(sas->ss_family)
      {
         case AF_INET:
         sa = sockaddrs[pos];
         inet_ntop(sas->ss_family, &sa->sin_addr, addr, sizeof(addr));
         break;

         case AF_INET6:
         sa6 = sockaddrs[pos];
         inet_ntop(sas->ss_family, &sa6->sin6_addr, addr, sizeof(addr));
         break;

         default:
         ptr = NULL;
         break;
      };
      bindle_widget_urldesc_print(cnf, field, &ptr, flag);
   };

   return;
}


void
bindle_widget_urldesc_unsigned(
         bindle_conf_t *               cnf,
         const char *                  component,
         uintptr_t                     value,
         uintptr_t                     flag )
{
   char           uint[64];
   char *         ptr;
   ptr = NULL;
   if ((value))
   {
      snprintf(uint, sizeof(uint), "%u", (unsigned)value);
      ptr = uint;
   };
   bindle_widget_urldesc_print(cnf, component, &ptr, flag);
   return;
}


/* end of source */
