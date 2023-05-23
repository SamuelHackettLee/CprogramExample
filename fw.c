#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

#include "rlw.h"
#include "hashtable.h"

#define DEFAULT_K 10

extern int getopt(int argc, char *const argv[], const char *optstring);

/* Program usage */
void usage() {
   fprintf(stderr, "usage: fw [ -n num ] [ file1 [ file 2 ...] ]\n");
   exit(EXIT_FAILURE);
}

int getN(int argc, char *argv[]);
void sortWordArr(word **wordArr, int size);

int main(int argc, char *argv[]) {
   extern int optind;
   unsigned int kWords;
   FILE *file = stdin;
   char *words;

   hash_table ht;
   word *insertWord;
   int i, size, printK;
   word **wordArr;

   /* get the k for the top k-Words to print */
   kWords = getN(argc, argv);
   size = TABLE_SIZE;
   
   if (!(ht.table = calloc(TABLE_SIZE, sizeof(word*)))) {
      perror("malloc error\n");
      exit(EXIT_FAILURE);
   }
   init_hash_table(size, &ht);

   /* If there are files to open, open them */
   if (optind < argc) {
      while (optind < argc) {
         file = fopen(argv[optind], "r");

         /* Check if file opened */
         if (file == NULL) {
            perror(argv[optind]);
         }
         else {
            while ((words = read_long_word(file)) != NULL) {
               if (!(insertWord = calloc(1, sizeof(word)))) {
                  perror("malloc error\n");
                  exit(EXIT_FAILURE);
               }

               insertWord->name = words;
               insertWord->count = 0;
               hash_table_insert(insertWord, &ht);
            }
            
            /* close the file */
            fclose(file);
         }

         optind++;
      }
   }
   /* There are no files to open, run fw with stdin */
   else {
      while((words = read_long_word(stdin)) != NULL) {
         if (!(insertWord = calloc(1, sizeof(word)))) {
               perror("malloc error\n");
               exit(EXIT_FAILURE);
         }

         insertWord->name = words;
         insertWord->count = 0;
         hash_table_insert(insertWord, &ht);
      }
   }

   printf("The top %d words (out of %d) are:\n", kWords, ht.num_items);
   /* How many words are we really printing */
   if (ht.num_items < kWords) {
      printK = ht.num_items;
   }
   else {
      printK = kWords;
   }

   /* Get the top k words*/
   if (!(wordArr = calloc(printK, sizeof(word*)))) {
      perror("malloc error\n");
      exit(EXIT_FAILURE);
   }
   kwordtableindicies(printK, &ht, wordArr);

   /* Sort wordArr */
   /* sortWordArr(wordArr, printK); */

   /* Print results */
   for (i = 0; i < printK; i++) {
      printf("%9d %s\n",wordArr[i]->count, wordArr[i]->name);
   }

   /* Clean up memory */
   for (i = 0; i < printK; i++) {
      if (wordArr[i]) {
         free(wordArr[i]->name);
         free(wordArr[i]);
      }
   }
   free(wordArr);
   free_table(&ht);

   return 0;
}

int getN(int argc, char *argv[]) {
   /*
    * Look for the n flag followed by a number
    * If it exists use that for the top K words
    * else set the number to the default value of 10
    */
   
   extern int opterr;
   extern char *optarg;

   int opt;
   int n = DEFAULT_K;
   char *end;

   /* Do not display flag error. Handle ourselves */
   opterr = 0;

   while ((opt = getopt(argc, argv, "n:")) != -1) {
      switch(opt) {
         case 'n':
            errno = 0;
            n = strtol(optarg, &end, 10);
            if (errno == 0 && *end == '\0') {
               break;
            }
            fprintf(stderr, "%s: not a well formed number.\n", optarg);
         /*Both the '?' and default cases lead to usage() */
         case '?':
         default:
            usage();
      }
   }
   
   if (n < 0) {
      fprintf(stderr,
         "fw: It is not possible to counter fewer than zero words\n");
      exit(EXIT_FAILURE);
   }

   return n;
}

void sortWordArr(word **wordArr, int size) {
   int i, j;
   word *tempWord;

   for (i = 0; i < size; i++) {
      for (j = 0; j < size; j++) {
         if (wordArr[i]->count == wordArr[j]->count &&
             strcmp(wordArr[i]->name, wordArr[j]->name) > 0) {
               tempWord = wordArr[j];
               wordArr[j] = wordArr[i];
               wordArr[i] = tempWord;
         }
      }
   }

}
