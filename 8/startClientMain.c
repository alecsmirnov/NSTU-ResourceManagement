#include <stdio.h>
#include <stdlib.h>

#include "client.h"

int main(int argc, char** argv) {
	// Запуск клиента на подключение к указанному серверу
	// по переданному ему имени сервера
	if (argc != 2) {
		fprintf(stderr, "Error: Invalid count of command line items!\n");
		fprintf(stderr, "Note:  Please enter the Server name.\n");
	}
	else
		startClient(argv[1]);

	return 0;
}
