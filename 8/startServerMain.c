#include <stdio.h>
#include <stdlib.h>

#include "server.h"

int main(int argc, char** argv) {
	// Запуск сервера с переданным ему Ip адресом и портом
	if (argc != 3) {
		fprintf(stderr, "Error: Invalid count of command line items!\n");
		fprintf(stderr, "Note:  Please enter the server Ip and Port.\n");
	}
	else
		startServer(argv[1], atoi(argv[2]));

	return 0;
}
