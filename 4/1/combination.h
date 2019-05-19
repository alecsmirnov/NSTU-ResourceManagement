#ifndef COMBINATION_H
#define COMBINATION_H

#include <stdint.h>

// Получить количество сочетаний из C(n, k)
uint64_t calculateCombination(uint8_t n, uint8_t k);

// Посчитать количество сочетаний из C(n, k) по указанным факториалам
static inline uint64_t combination(uint64_t n_fact, uint64_t k_fact, uint64_t nk_fact) {
	return n_fact / (k_fact * nk_fact);
}

#endif
