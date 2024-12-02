/*
 *  Bindle Binaries Tools
 *  Copyright (C) 2024 David M. Syzdek <david@syzdek.net>.
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
#define _LIB_LIBBINDLE_LHASH_C 1
#include "libbindle.h"
#include "lhash.h"

/*
 *  The hash table is implemented as a array sorted by key.  Eventually this
 *  code will be refactored to use a tree structure.
 */

///////////////
//           //
//  Headers  //
//           //
///////////////
// MARK: - Headers

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <assert.h>


//////////////////
//              //
//  Data Types  //
//              //
//////////////////
// MARK: - Data Types

typedef struct _bindle_hash_key
{
   char                 bf_key_bv_val[BNDL_HASH_KEY_LIMIT];
   bindle_berval_t      bf_key_bv;
   bindle_element_t     bf_ele;
   bindle_element_t *   bf_elep;
} bindle_hash_key_t;


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
// MARK: - Prototypes

//--------------------//
// element prototypes //
//--------------------//
// MARK: element prototypes

static int
bindle_element_cmp(
         const void *                  v1,
         const void *                  v2 );


static int
bindle_hash_element_case(
         const void *                  v1,
         const void *                  v2 );


static void
bindle_element_free(
         bindle_element_t *            ele );


static bindle_element_t *
bindle_element_init(
         const void *                  key,
         size_t                        keylen,
         const void *                  val,
         size_t                        vallen );


//-----------------------//
// hash table prototypes //
//-----------------------//
// MARK: hash table prototypes

static int
bindle_hash_key(
         bindle_hash_key_t *           k,
         const void *                  key,
         size_t                        key_len );


static ssize_t
bindle_hash_validate(
         int                           type,
         const void *                  dat,
         size_t                        len );


/////////////////
//             //
//  Functions  //
//             //
/////////////////
// MARK: - Functions

//-------------------//
// element functions //
//-------------------//
// MARK: element functions

int
bindle_element_cmp(
         const void *                  v1,
         const void *                  v2 )
{
   size_t                              len;
   int                                 rc;
   const bindle_berval_t *    t1;
   const bindle_berval_t *    t2;

   BindleDebugTrace();
   assert( v1 != NULL );
   assert( v2 != NULL );
   assert( *((bindle_element_t * const *)v1) != NULL);
   assert( *((bindle_element_t * const *)v2) != NULL);

   t1 = (*((const bindle_element_t * const *)v1))->e_key;
   t2 = (*((const bindle_element_t * const *)v2))->e_key;

   len = (t1->bv_len < t2->bv_len) ? t1->bv_len : t2->bv_len;
   if ((rc = memcmp(t1->bv_val, t2->bv_val, len)) != 0)
      return(rc);

   if (t1->bv_len < t2->bv_len)
      return(-1);
   if (t1->bv_len > t2->bv_len)
      return(1);

   return(0);
}


int
bindle_hash_element_case(
         const void *                  v1,
         const void *                  v2 )
{
   const bindle_berval_t *    t1;
   const bindle_berval_t *    t2;

   BindleDebugTrace();
   assert( v1 != NULL );
   assert( v2 != NULL );
   assert( *((bindle_element_t * const *)v1) != NULL);
   assert( *((bindle_element_t * const *)v2) != NULL);

   t1 = (*((const bindle_element_t * const *)v1))->e_key;
   t2 = (*((const bindle_element_t * const *)v2))->e_key;

   return(strcasecmp(((const char *)t1->bv_val), ((const char *)t2->bv_val)));
}


void
bindle_element_free(
         bindle_element_t *            ele )
{
   BindleDebugTrace();

   if (ele == NULL)
      return;

   if ((ele->e_key))
      bindle_berval_free(ele->e_key);
   ele->e_key = NULL;

   if ((ele->e_val))
      bindle_berval_free(ele->e_val);
   ele->e_val = NULL;

   free(ele);

   return;
}


bindle_element_t *
bindle_element_init(
         const void *                  key,
         size_t                        keylen,
         const void *                  val,
         size_t                        vallen )
{
   bindle_element_t *      ele;

   BindleDebugTrace();
   assert( key    != NULL );
   assert( keylen != 0 );
   assert( ((val)) || ( (!(val)) && (!(vallen)) ) );

   if ((ele = malloc(sizeof(bindle_element_t))) == NULL)
      return(NULL);
   memset(ele, 0, sizeof(bindle_element_t));

   if ((ele->e_key = bindle_berval_init(key, keylen)) == NULL)
   {
      bindle_element_free(ele);
      return(NULL);
   };

   if ((ele->e_val = bindle_berval_init(val, vallen)) == NULL)
   {
      bindle_element_free(ele);
      return(NULL);
   };

   return(ele);
}


//----------------------//
// hash table functions //
//----------------------//
// MARK: hash table functions

int
bindle_hash_del(
         bindle_hash_t *               bh,
         const void *                  key,
         size_t                        key_len )
{
   ssize_t              idx;
   size_t               pos;
   bindle_hash_key_t    k;

   BindleDebugTrace();
   assert( bh      != NULL );
   assert( key     != NULL );
   assert( key_len != 0 );

   // find index of key
   bindle_hash_key(&k, key, key_len);
   idx = bindle_bindex(&k.bf_elep, bh->h_elements, bh->h_len, sizeof(bindle_element_t *), BNDL_BREPLACE, NULL, bh->cmp);
   if (idx == -1)
      return(0);

   // free element and shift array
   bindle_element_free(bh->h_elements[idx]);
   for(pos = ((size_t)idx)+1; (pos < bh->h_len); pos++)
      bh->h_elements[pos-1] = bh->h_elements[pos];
   bh->h_len--;

   return(0);
}


void
bindle_hash_free(
         bindle_hash_t *               bh )
{
   size_t      pos;

   BindleDebugTrace();
   assert(bh != NULL);

   for(pos = 0; (pos < bh->h_len); pos++)
      bindle_element_free(bh->h_elements[pos]);

   if (bh->h_elements == NULL)
      free(bh->h_elements);
   bh->h_elements = NULL;

   memset(bh, 0, sizeof(bindle_hash_t));
   free(bh);

   return;
}


int
bindle_hash_get(
         bindle_hash_t *               bh,
         const void *                  key,
         size_t                        key_len,
         const void **                 valp,
         size_t *                      val_lenp )
{
   bindle_element_t **  elep;
   bindle_hash_key_t    k;

   BindleDebugTrace();
   assert( bh      != NULL );
   assert( key     != NULL );
   assert( key_len != 0 );

   if (bindle_hash_validate(bh->h_key_type, key, key_len) == -1)
      return(-1);

   // find vale value if exists
   bindle_hash_key(&k, key, key_len);
   if ((elep = bindle_bsearch(&k.bf_elep, bh->h_elements, bh->h_len, sizeof(bindle_element_t *), BNDL_BREPLACE, bh->cmp)) == NULL)
   {
      errno = ENODATA;
      return(-1);
   };

   if ((valp))
      *valp = (*elep)->e_val->bv_val;
   if ((val_lenp))
      *val_lenp = (*elep)->e_val->bv_len;

   return(0);
}


int
bindle_hash_get_r(
         bindle_hash_t *               bh,
         const void *                  key,
         size_t                        key_len,
         void *                        dst,
         size_t                        n,
         size_t *                      val_lenp )
{
   const void *   val;
   size_t         val_len;

   BindleDebugTrace();
   assert( bh      != NULL );
   assert( key     != NULL );
   assert( key_len != 0 );

   if (bindle_hash_get(bh, key, key_len, &val, &val_len) == -1)
      return(-1);

   if ((dst))
   {
      if (n < val_len)
      {
         errno = ENOBUFS;
         return(-1);
      };
      memcpy(dst, val, val_len);
   };
   if ((val_lenp))
      *val_lenp = val_len;

   return(0);
}


int
bindle_hash_initialize(
         bindle_hash_t **              bhp,
         int                           opts )
{
   bindle_hash_t *      bh;

   BindleDebugTrace();
   assert(bhp != NULL);

   if ((bh = malloc(sizeof(bindle_hash_t))) == NULL)
      return(-1);
   memset(bh, 0, sizeof(bindle_hash_t));

   bh->h_opts     = opts;
   bh->h_key_type = (opts & BNDL_HKEY_TYPE) >> 0;
   bh->h_val_type = (opts & BNDL_HVAL_TYPE) >> 4;

   // check key and value types
   switch(bh->h_key_type)
   {
      case BNDL_HASH_BERVAL:
      bh->cmp = bindle_element_cmp;
      break;

      case BNDL_HASH_STRING:
      bh->cmp = bindle_element_cmp;
      if ((opts & BNDL_HASH_CASE))
         bh->cmp = bindle_hash_element_case;
      break;

      default:
      bindle_hash_free(bh);
      errno = EINVAL;
      return(-1);
   };
   switch(bh->h_val_type)
   {
      case BNDL_HASH_BERVAL:
      case BNDL_HASH_STRING:
      case BNDL_HASH_INT:
      case BNDL_HASH_UNSIGNED:
      case BNDL_HASH_DOUBLE:
      break;

      default:
      bindle_hash_free(bh);
      errno = EINVAL;
      return(-1);
   };

   if ((bh->h_elements = malloc(sizeof(bindle_element_t *))) == NULL)
   {
      bindle_hash_free(bh);
      return(-1);
   };
   bh->h_elements[0] = NULL;

   *bhp = bh;

   return(0);
}


int
bindle_hash_key(
         bindle_hash_key_t *           k,
         const void *                  key,
         size_t                        key_len )
{
   BindleDebugTrace();
   assert( k       != NULL );
   assert( key     != NULL );
   assert( (key_len > 0) &&  (key_len <= BNDL_HASH_KEY_LIMIT));

   memcpy(k->bf_key_bv_val, key, key_len);
   k->bf_key_bv.bv_val = k->bf_key_bv_val;
   k->bf_key_bv.bv_len = key_len;
   memset(&k->bf_ele, 0, sizeof(bindle_element_t));
   k->bf_ele.e_key = &k->bf_key_bv;
   k->bf_elep      = &k->bf_ele;

   return(0);
}


int
bindle_hash_keys(
         bindle_hash_t *               bh,
         bindle_berval_t ***           bvap )
{
   bindle_berval_t **      bva;
   size_t                  pos;

   BindleDebugTrace();
   assert( bh != NULL );
   assert( bvap != NULL );

   // allocate array
   if (bindle_bvarray_init(&bva, bh->h_len) == -1)
      return(-1);

   // dupulicate bervals
   for(pos = 0; (pos < bh->h_len); pos++)
   {
      if (bindle_bvarray_add(&bva, bh->h_elements[pos]->e_key) == -1)
      {
         bindle_bvarray_free(bva);
         return(-1);
      };
   };

   *bvap = bva;

   return(0);
}


size_t
bindle_hash_len(
         bindle_hash_t *               bh )
{
   BindleDebugTrace();
   assert(bh != NULL);
   return(bh->h_len);
}


int
bindle_hash_opts(
         bindle_hash_t *               bh )
{
   BindleDebugTrace();
   assert(bh != NULL);
   return(bh->h_opts);
}


int
bindle_hash_sdel(
         bindle_hash_t *               bh,
         const char *                  key )
{
   BindleDebugTrace();
   assert( bh      != NULL );
   assert( key     != NULL );
   assert( key[0]  != 0 );
   return(bindle_hash_del(bh, key, (strlen(key)+1)));
}


int
bindle_hash_set(
         bindle_hash_t *               bh,
         const void *                  key,
         size_t                        key_len,
         const void *                  val,
         size_t                        val_len )
{
   void *               ptr;
   bindle_element_t *   ele;
   bindle_element_t **  elep;
   bindle_hash_key_t    k;

   BindleDebugTrace();
   assert( bh      != NULL );
   assert( key     != NULL );
   assert( key_len != 0 );
   assert( ((val)) || ( (!(val)) && (!(val_len)) ) );

   if (bindle_hash_validate(bh->h_key_type, key, key_len) == -1)
      return(-1);
   if (bindle_hash_validate(bh->h_val_type, val, val_len) == -1)
      return(-1);
   if ((key_len > BNDL_HASH_KEY_LIMIT) || (key_len == 0))
   {
      errno = EINVAL;
      return(-1);
   };
   if ( (bh->h_key_type == BNDL_HASH_STRING) && (((const char *)key)[key_len-1] != 0) )
   {
      errno = EINVAL;
      return(-1);
   };
   if ( (bh->h_val_type == BNDL_HASH_STRING) && (((const char *)val)[val_len-1] != 0) )
   {
      errno = EINVAL;
      return(-1);
   };

   // check for existing key and update value if exists
   bindle_hash_key(&k, key, key_len);
   if ((elep = bindle_bsearch(&k.bf_elep, bh->h_elements, bh->h_len, sizeof(bindle_element_t *), BNDL_BREPLACE, bh->cmp)) != NULL)
   {
      if (!(bh->h_opts & BNDL_HASH_REPLACE))
      {
         errno = EPERM;
         return(-1);
      };
      if (val_len > (*elep)->e_val->bv_len)
      {
         if ((ptr = realloc((*elep)->e_val->bv_val, val_len)) == NULL)
            return(-1);
         (*elep)->e_val->bv_val = ptr;
      };
      memcpy((*elep)->e_val->bv_val, val, val_len);
      (*elep)->e_val->bv_len = val_len;
      return(0);
   };

   // allocate bindle_element_t
   if ((ele = bindle_element_init(key, key_len, val, val_len)) == NULL)
      return(-1);

   // adds new element to hash
   if (bindle_badd(&ele, (void **)&bh->h_elements, &bh->h_len, sizeof(bindle_element_t *), 0, bh->cmp, (void (*)(void *))bindle_element_free, realloc) == -1)
      return(-1);

   return(0);
}


int
bindle_hash_sget(
         bindle_hash_t *               bh,
         const char *                  key,
         const void **                 valp,
         size_t *                      val_lenp )
{
   BindleDebugTrace();
   assert( bh      != NULL );
   assert( key     != NULL );
   assert( key[0]  != '\0' );
   if (bh->h_key_type != BNDL_HASH_STRING)
   {
      errno = ENOTSUP;
      return(-1);
   };
   return(bindle_hash_get(bh, key, (strlen(key)+1), valp, val_lenp));
}


int
bindle_hash_sget_r(
         bindle_hash_t *               bh,
         const char *                  key,
         void *                        dst,
         size_t                        n,
         size_t *                      val_lenp )
{
   const void *   val;
   size_t         val_len;

   BindleDebugTrace();
   assert( bh      != NULL );
   assert( key     != NULL );
   assert( key[0]  != '\0' );

   if (bh->h_key_type != BNDL_HASH_STRING)
   {
      errno = ENOTSUP;
      return(-1);
   };

   if (bindle_hash_get(bh, key, (strlen(key)+1), &val, &val_len) == -1)
      return(-1);

   if ((dst))
   {
      if (n < val_len)
      {
         errno = ENOBUFS;
         return(-1);
      };
      memcpy(dst, val, val_len);
   };
   if ((val_lenp))
      *val_lenp = val_len;

   return(0);
}


int
bindle_hash_skeys(
         bindle_hash_t *               bh,
         char ***                      keysp )
{
   char **              keys;
   size_t               pos;
   size_t               size;
   bindle_berval_t *    bv;

   BindleDebugTrace();
   assert( bh    != NULL );
   assert( keysp != NULL );

   if (bh->h_key_type != BNDL_HASH_STRING)
   {
      errno = ENOTSUP;
      return(-1);
   };

   // allocate array
   size = sizeof(char *) * (bh->h_len+1);
   if ((keys = malloc(size)) == NULL)
      return(-1);
   memset(keys, 0, size);

   // dupulicate bervals
   for(pos = 0; (pos < bh->h_len); pos++)
   {
      bv = bh->h_elements[pos]->e_key;
      if ((keys[pos] = malloc(bv->bv_len)) == NULL)
      {
         bindle_strsfree(keys);
         return(-1);
      };
      memcpy(keys[pos], bv->bv_val, bv->bv_len);
   };

   *keysp = keys;

   return(0);
}


int
bindle_hash_sset(
         bindle_hash_t *               bh,
         const char *                  key,
         const void *                  val,
         size_t                        val_len )
{
   BindleDebugTrace();
   assert( bh      != NULL );
   assert( key     != NULL );
   assert( key[0]  != 0 );
   assert( ((val)) || ( (!(val)) && (!(val_len)) ) );
   if (bh->h_key_type != BNDL_HASH_STRING)
   {
      errno = ENOTSUP;
      return(-1);
   };
   return(bindle_hash_set(bh, key, (strlen(key)+1), val, val_len));
}


ssize_t
bindle_hash_validate(
         int                           type,
         const void *                  dat,
         size_t                        len )
{
   size_t   tsize;
   if ((dat))
   {
      switch(type)
      {
         case BNDL_HASH_STRING:
         if (((const char *)dat)[len-1] != '\0')
         {
            errno = EINVAL;
            return(-1);
         };
         break;

         default:
         break;
      };
   };
   switch(type)
   {
      case BNDL_HASH_BERVAL:     tsize = len;               break;
      case BNDL_HASH_STRING:     tsize = len;               break;
      case BNDL_HASH_INT:        tsize = sizeof(int);       break;
      case BNDL_HASH_UNSIGNED:   tsize = sizeof(unsigned);  break;
      case BNDL_HASH_DOUBLE:     tsize = sizeof(double);    break;

      default:
      errno = EINVAL;
      return(-1);
   };
   if (tsize == len)
      return((ssize_t)len);
   errno = EINVAL;
   return(-1);
}


/* end of source */
