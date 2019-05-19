#include "Cache.h"

// Получить размер кэша
uint32_t Cache::getSize() const {
	return size;
}

// Получить длину строки кэша
uint32_t Cache::getLineSize() const {
	return line_size;
}

// Проверка кэша на пустоту
bool Cache::empty() const {
	return size == 0 && line_size == 0;
}