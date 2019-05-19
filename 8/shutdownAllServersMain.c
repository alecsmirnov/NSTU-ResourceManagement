#include "clientServerFunctions.h"

int main(int argc, char** argv) {
	// Послать первому серверу из файла со списком адресов серверов
	// запрос на выключение. Первый сервер пошлёт запрос второму и т.д.
	struct sockaddr_in serv_info = getServerInfo(getAddressFromFile(DNS_FILE));
	serverShutdown(serv_info);

	return 0;
}