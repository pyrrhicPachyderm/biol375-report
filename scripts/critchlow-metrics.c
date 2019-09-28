//Takes a matrix of ordinal data (integers).
//Prints a matrix of distances between each pair of observations.
//Uses one of the metrics defined in Critchlow (1985).

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <ctype.h>
#include "tables.h"
#include "critchlow-metrics.h"

#define NUM_MANDATORY_ARGUMENTS 3

int usageErr(int argc, char *argv[]) {
	fprintf(stderr,
		"Usage: %s [OPTIONS] <metric> <infile> <outfile>\n"
		"\tThe only metric currently supported is 'tau'\n"
		"Options:\n"
		"\t -t\n"
		"\t\tTranspose: Each column is an observation, each variable a row\n",
		argv[0]);
	exit(1);
}

int main(int argc, char *argv[]) {
	bool transpose = false;
	int opt;
	while((opt = getopt(argc, argv, "t")) != -1) {
		switch(opt) {
			case 't':
				transpose = true;
				break;
			default:
				usageErr(argc, argv);
		}
	}
	
	if(optind + NUM_MANDATORY_ARGUMENTS != argc) {
		usageErr(argc, argv);
	}
	char* metricName = argv[optind++];
	char* infileName = argv[optind++];
	char* outfileName = argv[optind++];
	
	//Lowercase the metric name.
	for(size_t i = 0; i < strlen(metricName); i++) {
		metricName[i] = tolower(metricName[i]);
	}
	
	//Select the appropriate function based on the metric name.
	DistanceMatrixFunc metricFunc;
	/*if(strcmp(metricName, "ulam") == 0) {
		metricFunc = &ulamDistanceMatrix;
	}*/ if(strcmp(metricName, "tau") == 0) {
		metricFunc = &tauDistanceMatrix;
	} else {
		fprintf(stderr, "%s is not a supported metric.\n", metricName);
		exit(1);
	}
	
	DataTableInt *table = readCSVDataTableInt(infileName, transpose);
	//DistanceMatrix *distMat = metricFunc(table);
	
	size_t numRanks = ordinaliseDataTableInt(table);
	DistanceMatrix *mat = metricFunc(table, numRanks);
	
	printCSVDistanceMatrix(outfileName, mat);
	
	freeDataTableInt(table);
	freeDistanceMatrix(mat);
	return 0;
}

