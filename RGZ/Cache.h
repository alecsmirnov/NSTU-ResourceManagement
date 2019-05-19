#ifndef CACHE_H
#define CACHE_H

#include <cstdint>

// Класс представления информации о кэше
class Cache {
public:
	// Инициализация кэша
	Cache() : size(0), line_size(0) {};
	Cache(uint32_t size, uint32_t line_size) : size(size), line_size(line_size) {}

	// Получить размер кэша
	uint32_t getSize() const;
	// Получить длину строки кэша
	uint32_t getLineSize() const;

	// Проверка кэша на пустоту
	bool empty() const;

private:
	uint32_t size;
	uint32_t line_size;
};

#endif
