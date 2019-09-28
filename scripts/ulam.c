//Calculates the Ulam Hausdorff metric defined in Critchlow (1985).

#include <stdlib.h>
#include <stdbool.h>
#include "tables.h"
#include "critchlow-metrics.h"

//Computes the Ulam distance between full rankings `a` and the identity ranking.
//The identity ranking is the ingeters 0,...,(length-1) in order.
//Being a full ranking, there are `length` ranks.
//Using the description Sellers' (1980) algorithm as given in Critchlow (1985).
double ulamIdentityDistance(const int *a, size_t length) {
	return 0.0;
}

//Computes the Ulam Hausdorff distance between partial rankings `a` and `b`.
//Each partial ranking is of `length` items.
//Not every ranking need be present in `a` or `b`.
double ulamHausdorffDistance(const int *a, const int *b, size_t length, size_t numRanks) {
	return 0.0;
}

//Produces a distance matrix from an ordinalised data table.
DistanceMatrix *ulamDistanceMatrix(const DataTableInt *table, size_t numRanks) {
	return NULL;
}
