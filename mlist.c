/*
Daniel Juranec 2145171
APH Exercise 1
This is my own work as defined in the Academic Ethics agreement I have signed.
*/

#include "mlist.h"
#include <stdlib.h>
#include <stdio.h>

#define BUCKETSIZE 20

typedef struct bucket {
	int bsize;
	MEntry *entries[BUCKETSIZE];
} Bucket;

struct mlist {
	int lsize;
	Bucket **buckets;
};

int ml_verbose = 0;
int size = BUCKETSIZE;

/* ml_create - created a new mailing list */
MList *ml_create(void) {

	MList *ml;
	int i;

	if (ml_verbose) {
		fprintf(stderr, "mlist: creating mailing list\n");
	}

	/* malloc for mailing list */
	ml = malloc(sizeof(MList));

	/* size of mailing list */
	ml->lsize = size;

	/* malloc for all buckets */
	ml->buckets = malloc(sizeof(Bucket)*size);

	/* malloc for each bucket */
	for (i = 0; i < ml->lsize; i++) {
		ml->buckets[i] = malloc(sizeof(MEntry)*BUCKETSIZE);
		ml->buckets[i]->bsize = 0;
	}

	return ml;
}

void *ml_resize(MList *ml) {

	MList *mlnew;
	int i, j;

	if (ml_verbose){
		fprintf(stderr, "mlist: resizing\n");
	}

	/* double the size, create new mailing list */
	size = size * 2;
	mlnew = ml_create();

	/* go through all entries of the old list
	 * and add them to the new one
	 * while freeing the old list */
	for (i = 0; i < ml->lsize; i++) {
		for (j = 0; j < ml->buckets[i]->bsize; j++) {
			ml_add(&mlnew, ml->buckets[i]->entries[j]);
		}
		free(ml->buckets[i]);
	}

	free(ml->buckets);
	free(ml);

	return mlnew;
}

/* ml_add - adds a new MEntry to the list;
 * returns 1 if successful, 0 if error (malloc)
 * returns 1 if it is a duplicate */
int ml_add(MList **ml, MEntry *me) {

	Bucket *b;
	unsigned long hashvalue;

	/* if duplicate found */
	if (ml_lookup((*ml), me) != NULL) {
		return 1;
	}

	/* find the right bucket */
	hashvalue = me_hash(me, (*ml)->lsize);
	b = (*ml)->buckets[hashvalue];

	/* add new entry */
	b->entries[b->bsize] = me;
	(b->bsize)++;

	/* if bucket filled, resize the list*/
	if (b->bsize == BUCKETSIZE) {
		*ml = ml_resize(*ml);
	}

	return 1;
}

/* ml_lookup - looks for MEntry in the list, returns matching entry or NULL */
MEntry *ml_lookup(MList *ml, MEntry *me) {

	Bucket *b;
	unsigned long hashvalue;
	int i;

	if (ml_verbose) {
		fprintf(stderr, "mlist: ml_lookup() entered\n");
	}

	/* find the right bucket */
	hashvalue = me_hash(me, ml->lsize);
	b = ml->buckets[hashvalue];

	/* go through the bucket */
	for (i = 0; i < b->bsize; i++) {
		if (me_compare(b->entries[i], me) == 0) {
			return b->entries[i];
		}
	}

	/* matching entry not found */
	return NULL;
}

/* ml_destroy - destroy the mailing list */
void ml_destroy(MList *ml) {
	
	int i, j;

	if (ml_verbose) {
		fprintf(stderr, "mlist: ml_destroy() entered\n");
	}

	/* go through all nodes */
	for (i = 0; i < ml->lsize; i++) {
		for (j = 0; j < ml->buckets[i]->bsize; j++) {
			me_destroy(ml->buckets[i]->entries[j]);
		}
		free(ml->buckets[i]);
	}

	free(ml->buckets);
	free(ml);
}
