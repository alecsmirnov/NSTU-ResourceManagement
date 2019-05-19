#include "server.h"
#include "clientServerFunctions.h"

#include <string.h>

#define SERV_DATA_SIZE 3	// Размер данных ответа сервера
#define ADDR_DATA_SIZE 2	// Размер данных адреса сервера

#define EQUAL 0				// Статус равенства строк
#define CLOSE 0				// Статус закрытого файла

// Проверка адреса сервеа на пустоту
static bool isEmptyAddress(Address addr) {
	return *addr.ip == '\0' && addr.port == 0;
}

// Найти в файле соответствующий адресу и порту сервера ответ.
// Поиск в файле соответствий записи: имя сервера, Ip адрес текущего сервера, порт текущего сервера
static ServerResponse findServerResp(const char* filename, const char* name, const char* ip, uint16_t port) {
	FILE* fp = NULL;
	if ((fp = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "Error: unable to open \"%s\"\n", filename);
		exit(EXIT_FAILURE);
	}

	// Ищем соотетствующую серверу запись
	ServerResponse serv_msg;
	bool search = true;
	while(search && fscanf(fp, "%s %s %hu", serv_msg.name, serv_msg.ip, &serv_msg.port) == SERV_DATA_SIZE) 
		if (strcmp(serv_msg.name, name) == EQUAL &&
			strcmp(serv_msg.ip, ip) == EQUAL &&
			serv_msg.port == port)
			search = false;

	if (fclose(fp) != CLOSE)
		fprintf(stderr, "Error: unable to close \"%s\"\n", filename);

	// в случае если поиск продолжается (запись не найдена) -- обнуляем ответ сервера
	if (search) {
		*serv_msg.name = '\0';
		*serv_msg.ip   = '\0';
		 serv_msg.port =   0;
	}

	return serv_msg;
}

// Получить адрес следующего DNS сервера
static Address getNextServerAddr(const char* filename, const char* cur_ip, uint16_t cur_port) {
	FILE* fp = NULL;
	if ((fp = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "Error: unable to open \"%s\"\n", filename);
		exit(EXIT_FAILURE);
	}

	Address serv_addr;

	// Найти адрес текущего сервера
	bool search = true;
	while(search && fscanf(fp, "%s %hu", serv_addr.ip, &serv_addr.port) == ADDR_DATA_SIZE)
		if (strcmp(serv_addr.ip, cur_ip) == EQUAL && serv_addr.port == cur_port)
			search = false;

	// Получить адрес следующего за ним сервера
	int ret_val = fscanf(fp, "%s %hu", serv_addr.ip, &serv_addr.port);

	// Если поиск продолжается или текущий сервер последний (следующего за ним сервера нет)
	// -- обнуляем адрес следующего сервера
	if (search || ret_val != ADDR_DATA_SIZE) {
		*serv_addr.ip = '\0';
		 serv_addr.port = 0;
	}

	if (fclose(fp) != CLOSE)
		fprintf(stderr, "Error: unable to close \"%s\"\n", filename);

	return serv_addr;
}

// Запуск сервера с указанным Ip адресом и портом
void startServer(const char* ip, uint16_t port) {
	int serv_sock = getServerSocket(port);

	// Получить адрес следующего сервера,
	// для поиска записей и прекращения работы серверов
	Address next_serv = getNextServerAddr(DNS_FILE, ip, port);

	// Ожидать сообщения от клиентов до тех пор, пока не будет выключен сервер 
	bool listen_loop = true;
	while (listen_loop) {
		int client_sock = getListenClientSocket(serv_sock);

		// Получить сообщение от клиента
		char msg[TEXT_LEN];
		msgRecv(client_sock, msg, sizeof(msg));

		// Если сообщение не является сообщением на выключение серверов
		if (*msg != SHUTDOWN) {
			// Найти ответ сервера в списке соответствий имён и адресов
			ServerResponse serv_msg = findServerResp(SERV_FILE, msg, ip, port);

			// Если ответ сервера не найден (пуст), и если есть следующий сервер
			if (isEmptyResponce(serv_msg))
				if (!isEmptyAddress(next_serv)) {
					int tmp_sock = getClientSocket(getServerInfo(next_serv));

					// Посылаем следующему серверу имя сервера на подкючение
					msgSend(tmp_sock, msg, sizeof(msg));

					// Ожидаем ответ сервера от следующего DNS сервера
					msgRecv(tmp_sock, &serv_msg, sizeof(serv_msg));

					close(tmp_sock);
				}

			// Посылаем клиенту сообщение с ответом
			msgSend(client_sock, &serv_msg, sizeof(serv_msg));
		}
		else {
			// Если получено сообщени на выключение сервера,
			// заканчиваем ожидать сообщения от клиентов
			listen_loop = false;

			// Отправляем следующему серверу запрос на выключение
			if (!isEmptyAddress(next_serv))
				serverShutdown(getServerInfo(next_serv));
		}

		close(client_sock);
	}

	close(serv_sock);
}