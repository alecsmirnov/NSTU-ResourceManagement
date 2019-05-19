#include <stdio.h>

#include "commandPipeline.h"

int main(int argc, char** argv) {
	if (argc != 3) {
		fprintf(stderr, "Error: Invalid count of command line items!\n");
		fprintf(stderr, "Note:  Please specify input and output files.\n");
	}
	else
		startComminication(argv[1], argv[2]);

	return 0;
}
