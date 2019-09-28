//Defines functions to calculate the various metrics defined in Critchlow (1985).

#ifndef __CRITCHLOW_METRICS_H
#define __CRITCHLOW_METRICS_H

#include <stdlib.h>
#include <stdbool.h>
#include "tables.h"

extern size_t ordinaliseDataTableInt(DataTableInt *table);

//Define a type of function pointer: MetricDistanceMatrixFunc
//They take a DataTableInt and return a corresponding DistanceMatrix
typedef DistanceMatrix *(*DistanceMatrixFunc)(const DataTableInt *table, size_t numRanks);

extern DistanceMatrix *ulamDistanceMatrix(const DataTableInt *table, size_t numRanks);

#endif
