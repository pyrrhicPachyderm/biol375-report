//Defines functions to calculate the various metrics defined in Critchlow (1985).

#ifndef __CRITCHLOW_METRICS_H
#define __CRITCHLOW_METRICS_H

#include <stdlib.h>
#include <stdbool.h>
#include "tables.h"

typedef struct nij_matrix_s {
	size_t numRanks;
	
	long long unsigned int *nijs;
} NijMatrix;

extern long long unsigned int getNijMatrix(const NijMatrix *nij, size_t rank1, size_t rank2);

extern void freeNijMatrix(NijMatrix *nij);

extern NijMatrix *getNij(const int *a, const int *b, size_t length, size_t numRanks);

extern size_t ordinaliseDataTableInt(DataTableInt *table);

//Define a type of function pointer: MetricDistanceMatrixFunc
//They take a DataTableInt and return a corresponding DistanceMatrix
typedef DistanceMatrix *(*DistanceMatrixFunc)(const DataTableInt *table, size_t numRanks);

extern DistanceMatrix *ulamDistanceMatrix(const DataTableInt *table, size_t numRanks);

extern DistanceMatrix *tauDistanceMatrix(const DataTableInt *table, size_t numRanks);

#endif
