/*
Daniel Juranec 2145171
APH Exercise 1
This is my own work as defined in the Academic Ethics agreement I have signed.
*/

#include "mentry.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLINE 100
#define MAXPOST 10

/* me_get returns the next file entry, or NULL if end of file*/
MEntry *me_get(FILE *fd) {

	MEntry *me;
	me = (MEntry *)malloc(sizeof(MEntry));

	/* create strings for lines */
	char nameline[MAXLINE] = "";
	char addrline[MAXLINE] = "";
	char postline[MAXPOST] = "";

	/* read the lines */
	fgets(nameline, MAXLINE, fd);
	fgets(addrline, MAXLINE, fd);
	fgets(postline, MAXPOST, fd);

	/* end if end of file */
	if (nameline[0] == '\0') {
		free(me);
		return NULL;
	}

	/* memory allocation */
	me->surname = malloc(sizeof(char)*MAXLINE);
	me->postcode = malloc(sizeof(char)*MAXPOST);
	me->full_address = malloc(sizeof(char)*(MAXLINE*2+MAXPOST));

	/* take surname from name line */
	int i;
	for (i = 0; nameline[i] != ','; i++) {
		me->surname[i] = tolower(nameline[i]);
	}
	me->surname[i] = '\0';

	/* take house number from address line */
	me->house_number = atoi(addrline);

	/* remove non-alphanumeric from postcode line */
	int j, k;
	for (j = 0, k = 0; postline[j] != '\n'; j++) {
		if (isalnum(postline[j])) {
			me->postcode[k] = postline[j];
			k++;
		}
	}
	me->postcode[k] = '\0';

	/* combine full address */
	strcpy(me->full_address, nameline);
	strcat(me->full_address, addrline);
	strcat(me->full_address, postline);

	return me;
}

/* me_hash computes a hash of the MEntry, mod size */
unsigned long me_hash(MEntry *me, unsigned long size) {

	/* add house number */
	unsigned long hash = me->house_number;

	/* add hashed surname */
	int i;
	for (i = 0; me->surname[i] != '\0'; i++) {
		hash = (int)me->surname[i] + hash*31;
	}

	/* add hashed postcode */
	for (i = 0; me->postcode[i] != '\0'; i++){
		hash = (int)me->postcode[i] + hash*31;
	}

	return hash%size;
}

/* me_print prints the full address on fd */
void me_print(MEntry *me, FILE *fd) {
	fprintf(fd, "%s", me->full_address);
}

/* me_compare compares two mail entries, returning <0, 0, >0 if
 * me1<me2, me1==me2, me1>me2
 */
int me_compare(MEntry *me1, MEntry *me2) {
	int i = strcmp(me1->surname, me2->surname);
	if (i == 0) {
		i = strcmp(me1->postcode, me2->postcode);
		if (i == 0) {
			i = me1->house_number - me2->house_number;
		}
	}
	return i;
}

/* me_destroy destroys the mail entry */
void me_destroy(MEntry *me) {
	free(me->surname);
	free(me->postcode);
	free(me->full_address);
	free(me);
}
