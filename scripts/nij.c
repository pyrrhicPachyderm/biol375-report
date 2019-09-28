//Calculates the Kendall's tau Hausdorff metric defined in Critchlow (1985).

#include <stdlib.h>
#include <stdbool.h>
#include "critchlow-metrics.h"

NijMatrix *callocNijMatrix(size_t numRanks) {
	NijMatrix *nij = malloc(sizeof(NijMatrix));
	nij->numRanks = numRanks;
	nij->nijs = calloc(nij->numRanks * nij->numRanks, sizeof(long long unsigned int));
	
	return nij;
}

void setNijMatrix(NijMatrix *nij, size_t rank1, size_t rank2, long long unsigned int val) {
	nij->nijs[rank1 * nij->numObs + rank2] = val;
}

long long unsigned int getNijMatrix(NijMatrix *nij, size_t rank1, size_t rank2) {
	return nij->nijs[rank1 * nij->numObs + rank2];
}

void incrementNijMatrix(NijMatrix *nij, size_t rank1, size_t rank2) {
	nij->nijs[rank1 * nij->numObs + rank2] ++;
}

void freeNijMatrix(NijMatrix *nij) {
	free(nij->nijs);
	free(nij);
}

//Computes n_{ij} (as defined on page 36 of Critchlow (1985)) for partial rankings `a` and `b`.
//Computes for all i and j, and allocaed a NijMatrix for storing everything.
//Each partial ranking is of `length` items.
//Not every ranking need be present in `a` or `b`.
NijMatrix *getNij(const int *a, const int *b, size_t length, size_t numRanks) {
	NijMatrix *nij = callocNijMatrix(numRanks);
	
	for(size_t i = 0; i < length; i++) {
		incrementNijMatrix(nij, a[i], b[i]);
	}
}
