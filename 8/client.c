#include "client.h"
#include "clientServerFunctions.h"

#include <string.h>

// Запуск клиента на подключение к указанному серверу
void startClient(const char* serv_name) {
	// Получить сокет клиента
	struct sockaddr_in serv_info = getServerInfo(getAddressFromFile(DNS_FILE));
	int client_sock = getClientSocket(serv_info);
	
	// Отправить DNS запрос на подключение к указанному серверу
	char msg[TEXT_LEN];
	strcpy(msg, serv_name);
	msgSend(client_sock, msg, sizeof(msg));

	// Получить ответ от DNS сервера
	ServerResponse serv_msg;
	msgRecv(client_sock, &serv_msg, sizeof(serv_msg));

	// В случае если сервера пустой -- вывести сообщение об отсутствующем сервере,
	// иначе вывести полученное сообщение
	if (isEmptyResponce(serv_msg))
		printf("Server not found!\n");
	else
		printf("Connection to server: %s, ip: %s, port: %hu\n", serv_msg.name, serv_msg.ip, serv_msg.port);

	close(client_sock);
}
