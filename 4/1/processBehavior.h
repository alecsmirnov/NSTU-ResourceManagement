#ifndef PROCESSBEHAVIOR_H
#define PROCESSBEHAVIOR_H

#include <stdint.h>

// Поведение для процесса потомка
void processBehavior(uint8_t n, int* fd);

// Посчитать факториал от n
static inline uint64_t factorial(uint8_t n) {
	uint64_t result = 1;

	for (uint8_t i = 2; i <= n; ++i)
		result *= i;

	return result;
}

#endif