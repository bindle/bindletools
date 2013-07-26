/*
 *  Bindle Binaries Tools
 *  Copyright (C) 2013 Bindle Binaries <syzdek@bindlebinaries.com>.
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
 *   @file bindle/utf8.h
 *   Bindletools UTF-8 provides basic support for manipulating UTF-8 strings.
 */
#ifndef __BINDLE_REGEX_H
#define __BINDLE_REGEX_H 1

///////////////////
//               //
//  Definitions  //
//               //
///////////////////

/**
 *  Regular expression for IPv4 address.
 */
#define BINDLE_RE_IPV4_ADDR  \
   "((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\.){3,3}" \
   "(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])"


/**
 *  Regular expression for IPv6 address.
 */
#define BINDLE_RE_IPV6_ADDR  \
   "(" \
   "([0-9a-f]{1,4}:){7,7}[0-9a-f]{1,4}|" \
   "([0-9a-f]{1,4}:){1,7}:|" \
   "([0-9a-f]{1,4}:){1,6}:[0-9a-f]{1,4}|" \
   "([0-9a-f]{1,4}:){1,5}(:[0-9a-f]{1,4}){1,2}|" \
   "([0-9a-f]{1,4}:){1,4}(:[0-9a-f]{1,4}){1,3}|" \
   "([0-9a-f]{1,4}:){1,3}(:[0-9a-f]{1,4}){1,4}|" \
   "([0-9a-f]{1,4}:){1,2}(:[0-9a-f]{1,4}){1,5}|" \
   "[0-9a-f]{1,4}:((:[0-9a-f]{1,4}){1,6})|" \
   ":((:[0-9a-f]{1,4}){1,7}|:)|" \
   "fe08:(:[0-9a-f]{1,4}){2,2}%[0-9a-zA-Z]{1,}|" \
   "::(ffff(:0{1,4}){0,1}:){0,1}" \
   "((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9]).){3,3}" \
   "(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|" \
   "([0-9a-f]{1,4}:){1,4}:" \
   "((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9]).){3,3}" \
   "(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])" \
   ")"


/**
 *  Regular expression for DNS hostname.
 */
#define BINDLE_RE_HOSTNAME \
   "([0-9a-z]{1,}([-][0-9a-z]{1,}){0,}" \
   "(\.[0-9a-z]{1,}([-][0-9a-z]{1,}){0,}){0,})"


/**
 *  Regular expression for IP address.
 */
#define BINDLE_RE_IP_ADDR "(" BINDLE_RE_IPV4_ADDR "|" BINDLE_RE_IPV6_ADDR ")"


/**
 *  Regular expression for host.
 */
#define BINDLE_RE_HOST \
   "(" BINDLE_RE_HOSTNAME "|" BINDLE_RE_IPV4_ADDR "|" BINDLE_RE_IPV6_ADDR ")"


#endif /* end of header */
