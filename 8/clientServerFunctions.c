#include "clientServerFunctions.h"

#define CONNECTION_QUEUE_SIZE 1		// Размер очереди подключений к серверу 

#define DEFAULT_PROTOCOL 0			// Стандартный протокол обмена сообщений
#define NO_OPTIONS 0				// Без указанных опций для различных функций

#define CLOSE  0					// Статус закрытого файла
#define ERROR -1					// Статус принятой ошибки

// Проверить результат работы функции на ошибку
static void errorCheck(int value, const char* error_msg) {
	if (value == ERROR) {
		fprintf(stderr, "Error: failed to %s!\n", error_msg);
		exit(EXIT_FAILURE);
	}
}

// Проверка ответа сервера на пустоту
bool isEmptyResponce(ServerResponse serv_msg) {
	return *serv_msg.name == '\0' && *serv_msg.ip == '\0' && serv_msg.port == 0;
}

// Получить адрес сервера из файла
Address getAddressFromFile(const char* filename) {
	FILE* fp = NULL;
	if ((fp = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "Error: unable to open \"%s\"\n", filename);
		exit(EXIT_FAILURE);
	}

	Address addr;
	fscanf(fp, "%s %hu\n", addr.ip, &addr.port);

	if (fclose(fp) != CLOSE)
		fprintf(stderr, "Error: unable to close \"%s\"\n", filename);

	return addr;
}

// Преобразовать адрес сервера в информацию
struct sockaddr_in getServerInfo(Address serv_addr) {
	struct sockaddr_in serv_info;

	serv_info.sin_family = AF_INET;
	serv_info.sin_addr.s_addr = inet_addr(serv_addr.ip);
	serv_info.sin_port = htons(serv_addr.port);

	return serv_info;
}

// Получить клиентский сокет
int getClientSocket(struct sockaddr_in serv_info) {
	int client_sock = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
	errorCheck(client_sock, "create client socket");

	int ret_val = connect(client_sock, (struct sockaddr*)&serv_info, sizeof(serv_info));
	errorCheck(ret_val, "connect client socket");

	return client_sock;
}

// Получить серверный сокет
int getServerSocket(uint16_t port) {
	int serv_sock = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
	errorCheck(serv_sock, "create server socket");

	int ret_val = setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
	errorCheck(ret_val, "socket reuse");

	struct sockaddr_in serv_info;
	serv_info.sin_family = AF_INET;
	serv_info.sin_addr.s_addr = INADDR_ANY;
	serv_info.sin_port = htons(port);

	ret_val = bind(serv_sock, (struct sockaddr*)&serv_info, sizeof(serv_info));
	errorCheck(ret_val, "bind socket");

	return serv_sock;
}

// Получить сокет для прослушивания сообщений
int getListenClientSocket(int serv_sock) {
	int ret_val = listen(serv_sock, CONNECTION_QUEUE_SIZE);
	errorCheck(ret_val, "listen socket");

	int client_sock = accept(serv_sock, NULL, NULL);
	errorCheck(client_sock, "accept socket");

	return client_sock;
}

// Завершить работу серверов
void serverShutdown(struct sockaddr_in serv_info) {
	int client_sock = getClientSocket(serv_info);

	char msg[TEXT_LEN];
	*msg = SHUTDOWN;
	msgSend(client_sock, msg, sizeof(msg));

	close(client_sock);
}

// Отправить сообщение
void msgSend(int client_sock, void* msg, size_t msg_size) {
	int ret_val = send(client_sock, msg, msg_size, NO_OPTIONS);
	errorCheck(ret_val, "send client data");
}

// Принять сообщение
void msgRecv(int client_sock, void* msg, size_t msg_size) {
	int ret_val = recv(client_sock, msg, msg_size, NO_OPTIONS);
	errorCheck(ret_val, "client data receive");
}