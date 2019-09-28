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
	
	//The tau Hausdorff distance is the maximum if two different sums.
	long long unsigned int sum1 = 0, sum2 = 0;
	
	for(size_t iprime = 0; iprime < numRanks; iprime++) {
		for(size_t jprime = 0; jprime < numRanks; jprime++) {
			
			for(size_t i = 0; i < iprime; i++) {
				for(size_t j = jprime; j < numRanks; j++) {
					sum1 += getNijMatrix(nij, i, j) * getNijMatrix(nij, iprime, jprime);
				}
			}
			
			for(size_t i = 0; i <= iprime; i++) {
				for(size_t j = jprime + 1; j < numRanks; j++) {
					sum2 += getNijMatrix(nij, i, j) * getNijMatrix(nij, iprime, jprime);
				}
			}
			
		}
	}
	
	freeNijMatrix(nij);
	
	return sum1>sum2?sum1:sum2; //Return the greater of the two sums.
}


//Produces a distance matrix from an ordinalised data table.
DistanceMatrix *tauDistanceMatrix(const DataTableInt *table, size_t numRanks) {
	//TODO: Slap a generic allocater in tables.c that handles allocation for the correct ssize and observation naming.
	DistanceMatrix *mat = correspondingDistanceMatrix(table);
	for(size_t i = 0; i < table->numObs; i++) {
		for(size_t j = 0; j < table->numObs; j++) {
			long long unsigned int dist = tauHausdorffDistance(getRowDataTableInt(table, i), getRowDataTableInt(table, j), table->numVars, numRanks);
			setDistanceMatrix(mat, i, j, dist);
		}
	}
	return mat;
}


//TODO: There's some optimisation potential here.
//The NijMatrix is the same size for everything, but we alloc and free if n^2 times.
