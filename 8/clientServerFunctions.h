#ifndef CLIENTSERVERFUNCTIONS_H
#define CLIENTSERVERFUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DNS_FILE  "dns.url"		// Название файла с адресами серверов
#define SERV_FILE "server.url"	// Название файла с соответствиями адресов и названий серверов

#define TEXT_LEN 128			// Длина текстового сообщения к серверу
#define IP_LEN   16				// Длина Ip адреса

#define SHUTDOWN '\0'			// Сообщение прекращения работы сервера

typedef struct Address {		// Структура адреса сервера
	char ip[IP_LEN];			// Ip
	uint16_t port;				// Порт
} Address;

typedef struct ServerResponse {	// Структура ответа сервера
	char name[TEXT_LEN];		// Имя сервера
	char ip[IP_LEN];			// Адрес
	uint16_t port;				// Порт
} ServerResponse;

// Проверка ответа сервера на пустоту
bool isEmptyResponce(ServerResponse serv_msg);

// Получить адрес сервера из файла
Address getAddressFromFile(const char* filename);
// Преобразовать адрес сервера в информацию
struct sockaddr_in getServerInfo(Address serv_addr);
// Получить клиентский сокет
int getClientSocket(struct sockaddr_in serv_info);
// Получить серверный сокет
int getServerSocket(uint16_t port);
// Получить сокет для прослушивания сообщений
int getListenClientSocket(int serv_sock);

// Завершить работу серверов
void serverShutdown(struct sockaddr_in serv_info);

// Отправить сообщение
void msgSend(int client_sock, void* msg, size_t msg_size);
// Принять сообщение
void msgRecv(int client_sock, void* msg, size_t msg_size);

#endif