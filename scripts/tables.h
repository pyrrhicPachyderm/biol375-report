//Defines structures for storing data and functions for operating on them.

#ifndef __TABLES_H
#define __TABLES_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct data_table_int_s {
	size_t numObs;
	size_t numVars;
	size_t allocedObs;
	
	int *data;
	char** varNames;
	char** obsNames;
} DataTableInt;

extern DataTableInt *allocDataTableInt(size_t numVars, size_t numObs);

extern void setDataTableInt(DataTableInt *table, size_t obsNum, size_t varNum, int val);

extern int getDataTableInt(DataTableInt *table, size_t obsNum, size_t varNum);

extern void setDataTableIntVarName(DataTableInt *table, size_t varNum, const char *name);

extern void setDataTableIntObsName(DataTableInt *table, size_t obsNum, const char *name);

extern void freeDataTableInt(DataTableInt *table);

extern DataTableInt *readCSVDataTableInt(const char* filename, bool transpose);

extern void printCSVDataTableInt(const char *filename, DataTableInt *table, bool transpose);

typedef struct distance_matrix_s {
	size_t numObs;
	
	double *distances;
} DistanceMatrix;

extern DistanceMatrix *allocDistanceMatrix(size_t numVars);

#endif