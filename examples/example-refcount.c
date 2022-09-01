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
#define _EXAMPLES_EXAMPLE_REFCOUNT_C 1


///////////////
//           //
//  Headers  //
//           //
///////////////
#pragma mark - Headers

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <bindle.h>


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#pragma mark - Definitions

#undef PROGRAM_NAME
#define PROGRAM_NAME "example-refcount"


//////////////////
//              //
//  Data Types  //
//              //
//////////////////
#pragma mark - Data Types

struct student
{
   bindle_obj_t   obj;
   int            year;
   int            grade;
   char *         name;
};


/////////////////
//             //
//  Variables  //
//             //
/////////////////
#pragma mark - Variables


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#pragma mark - Prototypes

int
main(
         void );


static void
student_free(
         void *                        ptr );


static struct student *
student_init(
         const char *                  name,
         int                           year );


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#pragma mark - Functions

int
main(
         void )
{
   struct student * student;
   if ((student = student_init("John Doe", 5)) == NULL)
   {
      fprintf(stderr, "out of virtual memory\n");
      return(1);
   };
   printf("1st reference count: %i\n", bindle_retain_count(&student->obj));
   bindle_retain(&student->obj);
   printf("2nd reference count: %i\n", bindle_retain_count(&student->obj));
   bindle_free(student);
   printf("3rd reference count: %i\n", bindle_retain_count(&student->obj));
   bindle_free(student);
   return(0);
}


void
student_free(
         void *                        ptr )
{
   struct student * student;
   const char *     name;
   if ((student = ptr) == NULL)
      return;
   name = ((student->name)) ? student->name : "unknown";
   printf("Freeing data struct for \"%s\".\n", name);
   if ((student->name))
      free(student->name);
   free(student);
   return;
}


struct student *
student_init(
         const char *                  name,
         int                           year )
{
   size_t            s;
   struct student *   person;
   s = sizeof(struct student);
   if ((person = bindle_alloc(s, &student_free)) == NULL)
   {
      fprintf(stderr, "out of virtual memory\n");
      return(NULL);
   };
   person->year = year;
   if ((person->name = bindle_strdup(name)) == NULL)
   {
      fprintf(stderr, "out of virtual memory\n");
      bindle_free(person);
      return(NULL);
   };
   return(bindle_retain(&person->obj));
}

/* end of source */

