#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "processLauncher.h"

int main(int argc, char** argv) {
	if (argc != 3) {
		fprintf(stderr, "Error: Invalid count of command line items!\n");
		fprintf(stderr, "Note:  Please enter the total count of processes and the number of active processes.\n");
	}
	else {
		uint8_t proc_count = atoi(argv[1]);
		uint8_t active_proc_count = atoi(argv[2]);

		start(proc_count, active_proc_count);
	}

	return 0;
}
