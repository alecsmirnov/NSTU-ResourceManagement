#include <stdio.h>
#include <stdlib.h>

#include "combination.h"

#define FACTORIAL_MAX 20		// Максимальное значение факториала в типе uint64_t

int main(int argc, char** argv) {
	if (argc != 3)
		fprintf(stderr, "Invalid count of command line items!\n");
	else {
		uint8_t n = atoi(argv[1]);
		uint8_t k = atoi(argv[2]);

		if (1 <= n && n <= FACTORIAL_MAX && 1 <= k && k <= FACTORIAL_MAX)
			if (k <= n)
				printf("Number of combination: %lu\n", calculateCombination(n, k));
			else 
				fprintf(stderr, "Error: K can't be more than N!\n");
		else 
			fprintf(stderr, "Error: K and N should be in {1, 2,..., %hhu} range!\n", FACTORIAL_MAX);
	}

    return 0;
}
