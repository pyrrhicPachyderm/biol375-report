//Defines function for converting ordinal data.

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tables.h"

//A function for use in qsort
int intCompareAscending(const void *p, const void *q) {
	int x = *(const int *)p;
	int y = *(const int *)q;
	if (x < y)
		return -1;
	else if (x > y)
		return 1;
	return 0;
}

//Converts the elements of the data table to consecutive integers, beginning at zero.
//Preserves ordinal properties.
//Returns the number of ranks.
int ordinaliseDataTableInt(DataTableInt *table) {
	//First, we need to determine how many ranks are present, and what number maps to what.
	//Grab a linearised copy of the data to do this.
	size_t dataLen = table->numVars * table->numObs;
	size_t dataSize = dataLen * sizeof(int);
	int *ranks = malloc(dataSize);
	memcpy(ranks, table->data, dataSize);
	//Sort it.
	qsort(ranks, dataLen, sizeof(int), &intCompareAscending);
	
	//Make it unique.
	//Uses the first numRanks items of the ranks array.
	//The rest of the ranks array will be left untouched, but will be useless after this.
	size_t numRanks = 1;
	for(size_t i = 1; i < dataLen; i++) {
		if(ranks[numRanks-1] != ranks[i]) {
			ranks[numRanks] = ranks[i];
			numRanks++;
		}
	}
	
	//Convert the original data to ranks.
	for(size_t i = 1; i < dataLen; i++) {
		int datum = table->data[i];
		bool foundMatch = false;
		for(size_t rank = 0; rank < numRanks; rank++) {
			if(ranks[rank] == datum) {
				table->data[i] = rank;
				foundMatch = true;
				break;
			}
		}
		if(!foundMatch) {
			fprintf(stderr, "The ordinalisation function has reached an unreachable state.\n");
			exit(1);
		}
	}
	
	return numRanks;
}
