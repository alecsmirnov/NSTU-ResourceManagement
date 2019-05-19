#include <stdio.h>
#include <stdlib.h>

#include "processBehavior.h"

int main(int argc, char** argv) {
	if (argc != 4)
		fprintf(stderr, "Invalid count of child command line items: %d\n", argc);
	else {
		uint8_t n = atoi(argv[1]);

		int fd[2];
		fd[0] = atoi(argv[2]);
		fd[1] = atoi(argv[3]);

		processBehavior(n, fd);
    }

	return 0;
}