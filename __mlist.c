#include "mlist.h"
#include <stdlib.h>
#include <stdio.h>

#define MAXELEMENTS 20

int ml_verbose=0;

typedef struct Bucket{
	int size;
	MEntry *elements[MAXELEMENTS];
} Bucket;

struct mlist{
	int num_of_buckets;
	Bucket **buckets;
};

/* Creates a hash table with the specified capacity */
MList *ml_create_custom_capacity(int capacity){
	MList *hash_table;
	int i;

	hash_table = malloc(sizeof(MList));
	if (hash_table == NULL)
		return NULL;
	
	hash_table->num_of_buckets = capacity;

	hash_table->buckets = malloc(sizeof(Bucket *) * capacity);
	if (hash_table->buckets == NULL){
		free(hash_table);
		return NULL;
	}

	for (i = 0; i < hash_table->num_of_buckets; ++i)
	{
		hash_table->buckets[i] = malloc(sizeof(Bucket));
		hash_table->buckets[i]->size = 0;
	}

	return hash_table;
}

MList *ml_create(void){
	return ml_create_custom_capacity(20);
}

/*	Free the memory associated with the old hash table */
void ml_destroy_old_elements(MList **ml){
	int i;

	for (i = 0; i < (*ml)->num_of_buckets; i++)
		free((*ml)->buckets[i]);

	free((*ml)->buckets);
	free(*ml);
}

/*	The function doubles the capacity of the current hash table */
MList *ml_resize(MList **ml){
	MList *hash_table;
	int i,j,hash_index,current_bucket_size;
	MEntry *element = NULL;

	hash_table = ml_create_custom_capacity((*ml)->num_of_buckets*2);

	/* Transfer the elements from the old hash table to the new one. */
	for (i = 0; i < (*ml)->num_of_buckets; i++)
	{
		for (j = 0; j < (*ml)->buckets[i]->size; j++)
		{
			element = (*ml)->buckets[i]->elements[j];
			hash_index = me_hash(element,hash_table->num_of_buckets);
			current_bucket_size = hash_table->buckets[hash_index]->size;

			hash_table->buckets[hash_index]->elements[current_bucket_size] = element;
			hash_table->buckets[hash_index]->size++;
		}
	}

	ml_destroy_old_elements(ml);
	*ml = hash_table;

	return *ml;
}

/*	Computes hash value for a mailing list entry 
	and checks if the the hash table is full.
	If it's full then it doubles its size
	and adds the new entry at the end. */
int ml_add(MList **ml, MEntry *me){
	int i,j,hash_index;

	hash_index = me_hash(me,(*ml)->num_of_buckets);
	Bucket *bucket = (*ml)->buckets[hash_index];

	if(bucket->size == MAXELEMENTS){
		if (ml_verbose)
			fprintf(stderr, "Doubling the size of buckets from %d to %d\n", (*ml)->num_of_buckets,(*ml)->num_of_buckets*2 );
		*ml = ml_resize(ml);
		hash_index = me_hash(me,(*ml)->num_of_buckets);
		bucket = (*ml)->buckets[hash_index];
	}

	bucket->elements[bucket->size] = me;
	(bucket->size)++;

	return 1;
}

/* 	ml_lookup - looks for MEntry in the list, 
	returns matching element or NULL */
MEntry *ml_lookup(MList *ml, MEntry *me){
 	int i;
 	int hash_index = me_hash(me,ml->num_of_buckets);

 	for(i = 0; i < ml->buckets[hash_index]->size; i++){
 	    if(me_compare(ml->buckets[hash_index]->elements[i],me)==0)
 	        return ml->buckets[hash_index]->elements[i];
 	}

 	return NULL;
}

/* Free the memory allocated for all elements,buckets,structs and pointers. */
void ml_destroy(MList *ml){
	int i,j;

	for(i = 0; i < ml->num_of_buckets; i++){
	    for(j = 0; j < ml->buckets[i]->size; j++)
	        me_destroy(ml->buckets[i]->elements[j]); 
	    free(ml->buckets[i]);
	}

	free(ml->buckets);
	free(ml);
}