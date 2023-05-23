#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

#include "rlw.h"

char * read_long_word(FILE * file)
{
   size_t size, len, last;
   char c;
   char * wrd  = NULL;
   size = len = last  = 0;

   while ((c = fgetc(file)) > 0){
      if( len  == size){
         size += BUFSIZE;
         if ( !(wrd = realloc(wrd,size))){
            perror("realloc error\n");
            exit(1);
         }
      }

      if (!(isalpha(c))&& len != 0){
         if ( !(wrd = realloc(wrd,len+1))){
            perror("realloc error\n");
            exit(1);
         }
         
         wrd[len] = '\0';
         return wrd;
      }
      if( isalpha(c)){
         wrd[len] = tolower(c);
         len += 1;}
   }

   if ( len > 0){
      if (!(wrd = realloc(wrd,len+1))){
            perror("realloc error\n");
            exit(1);
         }
      wrd[len] = '\0';
      return wrd;
   }
   else 
      return NULL;
}
