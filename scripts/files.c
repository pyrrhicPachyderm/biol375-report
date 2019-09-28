//Functions for working with files.

#include <stdio.h>
#include <stdlib.h>
#include "files.h"

FILE *openFile(const char *filename, const char* mode) {
	FILE* file = fopen(filename, mode);
	if(file == NULL) {
		fprintf(stderr, "Could not open file %s\n", filename);
		exit(1);
	}
	return file;
}

