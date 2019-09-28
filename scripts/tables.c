//Defines structures for storing data and functions for operating on them.

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "files.h"
#include "tables.h"

DataTableInt *allocDataTableInt(size_t numVars, size_t numObs) {
	DataTableInt *table = malloc(sizeof(DataTableInt));
	table->numVars = numVars;
	table->numObs = numObs;
	
	table->data = malloc(table->numVars * table->numObs * sizeof(int));
	
	table->varNames = malloc(table->numVars * sizeof(char*));
	table->obsNames = malloc(table->numObs * sizeof(char*));
	for(size_t i = 0; i < table->numVars; i++)
		table->varNames[i] = NULL;
	for(size_t i = 0; i < table->numObs; i++)
		table->obsNames[i] = NULL;
	
	return table;
}

void setDataTableInt(DataTableInt *table, size_t obsNum, size_t varNum, int val) {
	table->data[obsNum * table->numVars + varNum] = val;
}

int getDataTableInt(DataTableInt *table, size_t obsNum, size_t varNum) {
	return table->data[obsNum * table->numVars + varNum];
}

void setDataTableIntVarName(DataTableInt *table, size_t varNum, const char *name) {
	free(table->varNames[varNum]);
	table->varNames[varNum] = strdup(name);
}

void setDataTableIntObsName(DataTableInt *table, size_t obsNum, const char *name) {
	free(table->obsNames[obsNum]);
	table->obsNames[obsNum] = strdup(name);
}

void freeDataTableInt(DataTableInt *table) {
	for(size_t i = 0; i < table->numVars; i++)
		free(table->varNames[i]);
	for(size_t i = 0; i < table->numObs; i++)
		free(table->obsNames[i]);
	free(table->varNames);
	free(table->obsNames);
	free(table->data);
	free(table);
}

DataTableInt *readCSVDataTableInt(const char* filename, bool transpose) {
	FILE* file = openFile(filename, "r");
	
	size_t numObs;
	size_t numVars;
	
	//First, determine the number of rows in the file.
	size_t numRows = 0;
	//bool foundData = false; //Any characters on this line yet?
	for(char c = getc(file); c != EOF; c = getc(file)) {
		if(c == '\n') {
			numRows++;
		}/* else if(!isspace(c)) {
			foundData = true;
		}*/
	}
	//if(foundData) numRows++; //Not relying on the last line being newline-terminated.
	numRows--; //Discount the header line.
	
	//Go back to the beginning of the file to read it properly.
	fseek(file, 0, SEEK_SET);
	
	char c;
	//Remove the top-left cell (clobber up to and including a comma).
	while((c = getc(file)) != ',' && c != EOF) continue;
	
	char* headBuff = NULL;
	size_t headBuffSize = 0;
	int headLen = getline(&headBuff, &headBuffSize, file);
	if(headLen <= 0) {
		fprintf(stderr, "File %s has empty header line.\n", filename);
		exit(1);
	}
	if(headBuff[headLen-1] == '\n')
		headBuff[headLen-1] = '\0'; //Remove the newline from the end.
	
	char** colNames = NULL; //Each entry points within headBuff.
	char*  colName; //Points within headBuff.
	size_t numCols = 0;
	while((colName = strtok(numCols==0?headBuff:NULL, ",")) != NULL) {
		numCols++;
		colNames = realloc(colNames, numCols*sizeof(char*));
		colNames[numCols-1] = colName;
	}
	if(numCols == 0) {
		fprintf(stderr, "File %s has no entries in the header line.\n", filename);
		exit(1);
	}
	
	//Initialise the data table.
	if(!transpose) {
		numObs = numRows;
		numVars = numCols;
	} else {
		numObs = numCols;
		numVars = numRows;
	}
	DataTableInt *table = allocDataTableInt(numVars, numObs);
	
	//Copy the column names over.
	for(size_t col = 0; col < numCols; col++) {
		if(!transpose)
			setDataTableIntVarName(table, col, colNames[col]);
		else
			setDataTableIntObsName(table, col, colNames[col]);
	}
	free(colNames);
	free(headBuff);
	
	//Read the rest of the rows.
	char* rowBuff = NULL;
	size_t rowBuffSize = 0;
	int rowLen = 0; //Needs to be signed because getline gives -1 on error.
	size_t rowNum = 0;
	while((rowLen = getline(&rowBuff, &rowBuffSize, file)) > 0) {
		if(rowBuff[rowLen-1] == '\n')
			rowBuff[rowLen-1] = '\0'; //Remove the newline from the end.
		char* rowName = strtok(rowBuff, ",");
		if(!transpose)
			setDataTableIntObsName(table, rowNum, rowName);
		else
			setDataTableIntVarName(table, rowNum, rowName);
		
		char* cellStr;
		size_t colNum = 0;
		while((cellStr = strtok(NULL, ",")) != NULL) {
			char *cellEnd;
			int cell = (int)strtol(cellStr, &cellEnd, 10); //Always base 10.
			if(!(*cellStr != '\0' && *cellEnd == '\0')) {
				fprintf(stderr, "%s is not a valid integer.\n", cellStr);
				exit(1);
			}
			
			if(!transpose)
				setDataTableInt(table, rowNum, colNum, cell);
			else
				setDataTableInt(table, colNum, rowNum, cell);
			
			colNum++;
		}
		if(colNum < numCols) {
			fprintf(stderr, "The row %s doesn't have enough entries to match the header. The header has %ld entries, and this row has %ld.\n", rowName, numCols, colNum);
			exit(1);
		}
		if(colNum > numCols) {
			fprintf(stderr, "The row %s has too many entries to match the header. The header has %ld entries, and this row has %ld.\n", rowName, numCols, colNum);
			exit(1);
		}
		rowNum++;
	}
	free(rowBuff);
	fclose(file);
	
	return table;
}

void printCSVDataTableInt(const char *filename, DataTableInt *table, bool transpose) {
	FILE* file = openFile(filename, "w");
	
	size_t numRows;
	size_t numCols;
	char** rowNames;
	char** colNames;
	if(!transpose) {
		numRows = table->numObs;
		numCols = table->numVars;
		rowNames = table->obsNames;
		colNames = table->varNames;
	} else {
		numCols = table->numObs;
		numRows = table->numVars;
		rowNames = table->varNames;
		colNames = table->obsNames;
	}
	
	//Print the header line.
	for(size_t col = 0; col < numCols; col++) {
		fprintf(file, ",%s", colNames[col]);
	}
	fprintf(file, "\n");
	
	//Print the rows.
	for(size_t row = 0; row < numRows; row++) {
		fprintf(file, "%s", rowNames[row]);
		for(size_t col = 0; col < numCols; col++) {
			if(!transpose)
				fprintf(file, ",%d", getDataTableInt(table, row, col));
			else
				fprintf(file, ",%d", getDataTableInt(table, col, row));
		}
		fprintf(file, "\n");
	}
	
	fclose(file);
}

DistanceMatrix *allocDistanceMatrix(size_t numObs) {
	DistanceMatrix *mat = malloc(sizeof(DistanceMatrix));
	mat->numObs = numObs;
	
	mat->distances = malloc(mat->numObs * mat->numObs * sizeof(double));
	mat->obsNames = malloc(mat->numObs  * sizeof(char*));
	for(size_t i = 0; i < mat->numObs; i++)
		mat->obsNames[i] = NULL;
	
	return mat;
}

void setDistanceMatrix(DistanceMatrix *mat, size_t obsNum1, size_t obsNum2, double val) {
	mat->distances[obsNum1 * mat->numObs + obsNum2] = val;
}

double getDistanceMatrix(DistanceMatrix *mat, size_t obsNum1, size_t obsNum2) {
	return mat->distances[obsNum1 * mat->numObs + obsNum2];
}

void setDistanceMatrixObsName(DistanceMatrix *mat, size_t obsNum, const char *name) {
	free(mat->obsNames[obsNum]);
	mat->obsNames[obsNum] = strdup(name);
}

void freeDistanceMatrix(DistanceMatrix *mat) {
	for(size_t i = 0; i < mat->numObs; i++)
		free(mat->obsNames[i]);
	free(mat->obsNames);
	free(mat->distances);
	free(mat);
}
