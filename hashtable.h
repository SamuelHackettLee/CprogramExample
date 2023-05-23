#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

#define _POSIX_C_SOURCE 200809L
#define MAX_LENGTH      256
#define TABLE_SIZE      6000000

typedef struct {
   char *name;
   int count;
} word;

typedef struct {
   size_t table_size;
   int num_items;
   word **table;
} hash_table;

unsigned int hash(char *name, hash_table *ht);
void init_hash_table(int newsize, hash_table *ht);
float get_load_factor(hash_table ht);
bool hash_table_insert(word *w, hash_table *ht);
word *hash_table_lookup(char *name, hash_table *ht);
void free_table(hash_table *ht);
void resize_table(hash_table *ht);
void delete(int i, hash_table *ht);
void kwordtableindicies(int k, hash_table *ht, word **wrdarry);

#endif
