/*
Daniel Juranec 2145171
APH Exercise 1
This is my own work as defined in the Academic Ethics agreement I have signed.
*/

#include "mlist.h"
#include <stdlib.h>
#include <stdio.h>

#define BUCKETSIZE 20;

typedef struct node {
	MEntry *entry;
	struct node *next;
} Node;

struct mlist {
	Node **nodes;
	int size;
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
	ml->size = size;

	/* malloc for all nodes */
	ml->nodes = malloc(sizeof(Node)*ml->size);

	/* malloc for each node */
	for (i = 0; i < size; i++) {
		ml->nodes[i] = calloc(1, sizeof(Node));
		//ml->nodes[i]->next = NULL;
	}

	return ml;
}

void *ml_resize(MList *ml) {

	MList *mlnew;
	Node *cur, *curnext;
	int i;

	if (ml_verbose){
		fprintf(stderr, "mlist: resizing\n");
	}

	/* double the size, create new mailing list */
	size = (ml->size) * 2;
	mlnew = ml_create();

	/* go through all entries of the old list
	 * and add them to the new one
	 * while freeing the old list */
	for (i = 0; i < ml->size; i++) {
		/* cursor on 1st node */
		cur = ml->nodes[i];
		/* go through all nodes */
		while (cur->next != NULL) {
			/* save next cursor */
			curnext = cur->next;
			/* add entry to new list */
			ml_add(&mlnew, cur->entry);
			free(cur);
			/* move cursor to previously saved one */
			cur = curnext;
		}
		free(cur);
	}

	free(ml->nodes);
	free(ml);

	return mlnew;
}

/* ml_add - adds a new MEntry to the list;
 * returns 1 if successful, 0 if error (malloc)
 * returns 1 if it is a duplicate */
int ml_add(MList **ml, MEntry *me) {

	Node *n, *newn;
	unsigned long hashvalue;
	int bsize;

	/* if duplicate found */
	if (ml_lookup((*ml), me) != NULL) {
		return 1;
	}

	/* memory allocation for the new entry */
	if ((newn = calloc(1, sizeof(Node))) == NULL) {
		return 0;
	}
	//newn->next = NULL;

	/* find the right bucket */
	hashvalue = me_hash(me, (*ml)->size);
	n = (*ml)->nodes[hashvalue];

	/* go through the bucket and find
	 * how many entries there already */
	bsize = 0;
	while(n->next != NULL) {
		n = n->next;
		bsize++;
	}

	/* add new entry */
	n->next = newn;
	n->entry = me;

	/* if bucket filled, resize the list*/
	if (bsize > (*ml)->size) {
		*ml = ml_resize(*ml);
	}

	return 1;
}

/* ml_lookup - looks for MEntry in the list, returns matching entry or NULL */
MEntry *ml_lookup(MList *ml, MEntry *me) {

	Node *n;
	unsigned long hashvalue;

	if (ml_verbose) {
		fprintf(stderr, "mlist: ml_lookup() entered\n");
	}

	/* find the right bucket */
	hashvalue = me_hash(me, ml->size);
	n = ml->nodes[hashvalue];

	while (n->next != NULL) {
		if (me_compare(n->entry, me) == 0) {
			/* matching entry found */
			return n->entry;
		}
		else {
			/* not found, go on */
			n = n->next;
		}
	}

	/* matching entry not found */
	return NULL;
}

/* ml_destroy - destroy the mailing list */
void ml_destroy(MList *ml) {
	
	Node *cur, *curnext;
	int i;

	if (ml_verbose) {
		fprintf(stderr, "mlist: ml_destroy() entered\n");
	}

	/* go through all nodes */
	for (i = 0; i < ml->size; i++) {
		cur = ml->nodes[i];
		while (cur->next != NULL) {
			curnext = cur->next;
			me_destroy(cur->entry);
			free(cur);
			cur = curnext;
		}
		free(cur);
	}

	free(ml->nodes);
	free(ml);
}
