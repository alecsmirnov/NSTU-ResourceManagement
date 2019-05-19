#ifndef COMBINATION_H
#define COMBINATION_H

#include <stdint.h>

// Получить количество сочетаний из C(n, k)
uint64_t calculateCombination(uint8_t n, uint8_t k);

// Посчитать количество сочетаний из C(n, k) по указанным факториалам
static inline uint64_t combination(uint64_t n_fact, uint64_t k_fact, uint64_t nk_fact) {
	return n_fact / (k_fact * nk_fact);
}

// Посчитать факториал от n
static inline uint64_t factorial(uint8_t n) {
	uint64_t result = 1;

	for (uint8_t i = 2; i <= n; ++i)
		result *= i;

	return result;
}

#endif
