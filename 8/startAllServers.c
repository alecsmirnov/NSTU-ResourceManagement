#include "startAllServers.h"
#include "server.h"
#include "vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>

#define CHILD  0			// Id процесса потомка
#define EQUAL  0			// Статус равенства строк
#define CLOSE  0			// Статус закрытого файла
#define ERROR -1			// Статус принятой ошибки

#define NO_OPTIONS 0		// Без указанных опций для различных функций

#define PORT_LEN 5			// Длина порта
#define ADDR_DATA_SIZE 2	// Размер данных адреса сервера

// Заполнить список с адресами серверов
static void fillServerAddressList(Vector* addr_list, const char* filename) {
	FILE* fp = NULL;
	if ((fp = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "Error: unable to open \"%s\"\n", filename);
		exit(EXIT_FAILURE);
	}

	Elem serv_addr;
	while(fscanf(fp, "%s %hu", serv_addr.ip, &serv_addr.port) == ADDR_DATA_SIZE)
		vectorAdd(addr_list, serv_addr);

	if (fclose(fp) != CLOSE)
		fprintf(stderr, "Error: unable to close \"%s\"\n", filename);
}

// Запуск всех серверов из списка с адресами серверов
// в виде отдельных процессов
void startAllServers() {
	// Получить список серверов
	Vector addr_list;
	vectorInit(&addr_list, 0);
	fillServerAddressList(&addr_list, DNS_FILE);

	pid_t* pid = (pid_t*)malloc(sizeof(pid_t) * addr_list.size);

	for (uint8_t i = 0; i != addr_list.size; ++i)
		if ((pid[i] = fork()) == ERROR)
			fprintf(stderr, "Error: server %hhu fork error!\n", i);
		else
			if (pid[i] == CHILD) {
				// Запустить сервер с указанным адресом и портом
				char port[PORT_LEN];
				sprintf(port, "%hu", addr_list.data[i].port);

				execl("./startServer", "startServer", addr_list.data[i].ip, port, NULL);
			}

	for (uint8_t i = 0; i != addr_list.size; ++i)
		if (pid[i] != waitpid(pid[i], NULL, NO_OPTIONS))
			fprintf(stderr, "Error: server %hhu isn't done!\n", i);

	free(pid);
}