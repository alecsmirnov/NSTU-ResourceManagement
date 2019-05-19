#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define IP_LEN 16			// Длина Ip адреса

typedef struct Elem {		// Элемент вектора
	char ip[IP_LEN];		// Ip адрес
	uint16_t port;			// Порт
} Elem;

typedef struct Vector {		// Структура динамического вектора (Аналог STL вектора C++)
    uint8_t size;			// Размера
    uint8_t capacity;		// Вместимость

    Elem* data;				// Хранимые данные
} Vector;

// Проверка вектора на пустоту
static inline bool vectorIsEmpty(const Vector* V) {
    return V->data == NULL;
}

// Операция копирования элементов вектора
static inline void vectorElemAssign(Elem* E1, Elem E2) {
	strcpy(E1->ip, E2.ip);
	E1->port = E2.port;
}

// Инициализация вектора и его начальной вместимости
void vectorInit(Vector* V, uint8_t capacity);
// Добавить элемент в конец вектора
void vectorAdd(Vector* V, Elem E);
// Удалить хранимые данные вектор
void vectorDelete(Vector* V);

#endif

