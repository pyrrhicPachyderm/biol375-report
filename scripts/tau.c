//Calculates the Kendall's tau Hausdorff metric defined in Critchlow (1985).

#include <stdlib.h>
#include <stdbool.h>
#include "tables.h"
#include "critchlow-metrics.h"

//Computes the Kendall's tau Hausdorff distance between partial rankings `a` and `b`.
//Each partial ranking is of `length` items.
//Not every ranking need be present in `a` or `b`.
long long unsigned int tauHausdorffDistance(const int *a, const int *b, size_t length, size_t numRanks) {
	NijMatrix *nij = getNij(a, b, length, numRanks);
	
	freeNijMatrix(nij);
	return distance;
}


//Produces a distance matrix from an ordinalised data table.
DistanceMatrix *tauDistanceMatrix(const DataTableInt *table, size_t numRanks) {
	return NULL;
}


//TODO: There's some optimisation potential here.
//The NijMatrix is the same size for everything, but we alloc and free if n^2 times.
