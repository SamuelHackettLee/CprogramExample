#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

#include "hashtable.h"

unsigned int hash(char *name, hash_table *ht){
   int i, len;
   unsigned int hash_value;
   len = strlen(name);
   hash_value = 0;

   for (i=0; i<len; i++){
      /* hashing value: 85407253 */
      hash_value += name[i] * 85407253;
      hash_value = (hash_value * name[i]) % ht->table_size;
   }

   return hash_value;
}

void init_hash_table(int newsize, hash_table *ht){
   /* Initialize hash table. Fill with NULLs */
   int i;
   ht->table_size = newsize;
   ht->num_items = 0;

   for(i=0; i< newsize; i++){
      ht->table[i] = NULL;
   }
}

float get_load_factor(hash_table ht){
   /* Load factor for increasing size later */
   float lf;
   lf = ((float)(ht.num_items));
   lf = lf / ((1.0)*(ht.table_size));
   return lf;
}

void print_table(hash_table ht){
   /* Print the hash table */
   int i;
   printf("Start");
   for(i=0; i< ht.table_size; i++){
      if(ht.table[i] == NULL){
	      /*printf("%s is the name", ht.table[i]->name);*/
	      printf("\t%i\t--\n", i);
      }
      else
	      printf("\t%i\t%s\tcount: %d\n", 
            i, ht.table[i]->name, ht.table[i]->count);
   }

   printf("End\n");
}

bool hash_table_insert(word *w, hash_table *ht){
   /* Insert words into the hash table */
   int index, i;

   /*
   printf("NEW INSERT\n");
   */
   if (w == NULL)
      return false;
   if (strcmp(w->name, "") == 0) {
      free(w->name);
      free(w);
      return true;
   }
   /* also how to change hash to take in ht for hash function
    * so I can mod by its table size*/
   index = hash(w->name, ht);
   for(i=0; i < ht->table_size; i++){
      int try = (i + index) % ht->table_size;

      if ((ht->table[try] != NULL) && !strcmp(ht->table[try]->name,  w->name)){
         free(w->name);
         free(w);
         ht->table[try]->count += 1;
	      return true;
      }

      if (ht->table[try] == NULL) {
         ht->num_items +=1;
         ht->table[try] = w;
	      ht->table[try]->count += 1;
         if(get_load_factor(*ht) > 0.5){
            /*
	         print_table(*ht);
            */
	         resize_table(ht);
         }

         return true;
      }
   }

   return false;
}

word *hash_table_lookup(char *name, hash_table *ht){
   int i, try;
   int index = hash(name, ht);

   for (i=0; i < ht->table_size; i++){
      try = (i + index) % TABLE_SIZE;

      if (ht->table[try] != NULL &&
         strcmp(ht->table[try]->name, name)==0){
         return ht->table[try];
      }  
   }
   return NULL;
}

void free_table(hash_table *ht){
   int i;

   for(i=0; i < ht->table_size; i++){
      if(ht->table[i] != NULL ){
         if(ht->table[i]->name)
 	    free(ht->table[i]->name);
	 free(ht->table[i]);
      }
   }
   free(ht->table);
}

/* for rezise should it be void or should i set original ht to a 
 * return value of a new ht?*/
void resize_table(hash_table *ht){
   int new_size, old_size, i;
   word ** old_table;

   old_size = ht->table_size;
   new_size = (2 * (ht->table_size)) + 1;
   /*
   printf(" New table size is %d \n", new_size);
   */
   old_table = ht->table;

   /* Decrement count because insert will increment */
   for (i = 0; i < ht->table_size; i++) {
      if (ht->table[i] != NULL) {
         ht->table[i]->count -= 1;
      }
   }

   if ( !(ht->table = (word**)calloc(new_size, sizeof(word*)))){
      perror("malloc error\n");
      exit(1);
   }

   ht->table_size = new_size;
   init_hash_table(new_size, ht);

   for(i=0; i < old_size; i++){
      if(old_table[i] != NULL){
         hash_table_insert(old_table[i] , ht);
         /*
         printf("old table name %s: \n", old_table[i]->name);
         */
      }
   }

   free(old_table);
}

void delete(int i, hash_table *ht){
   /*
   free(ht->table[i]->name);
   free(ht->table[i]);
   */
   ht->table[i] = NULL;
}

void kwordtableindicies(int k, hash_table *ht, word **wrdarry) {
   int i, j, curk, curindex;
   word *temp;

   for (i = 0; i < k; i++) {
      curk = 0;
      
      for (j = 0; j < ht->table_size; j++) {
         if (ht->table[j]) {
            if (ht->table[j]->count > curk &&
                strcmp(ht->table[j]->name, "\n") != 0 &&
                strcmp(ht->table[j]->name, " ") != 0) {
               curk = ht->table[j]->count;
               wrdarry[i] = ht->table[j];
               curindex = j;
            }
            else if (ht->table[j]->count == curk &&
                     strcmp(ht->table[j]->name, wrdarry[i]->name) > 0 &&
                     strcmp(ht->table[j]->name, "\n") != 0 &&
                     strcmp(ht->table[j]->name, " ") != 0) {
               temp = ht->table[j];
               ht->table[j] = wrdarry[i];
               wrdarry[i] = temp;
            }
         }
      }
      
      delete(curindex, ht);
   }
}
