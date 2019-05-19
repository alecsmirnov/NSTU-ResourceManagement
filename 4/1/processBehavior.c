#include "processBehavior.h"

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

#define RD	  0				// Чтение из канала
#define WR	  1				// Запись в канал

// Поведение для процесса потомка
void processBehavior(uint8_t n, int* fd) {
	uint64_t result = factorial(n);		// Подсчёт факториала

	close(fd[RD]);						// Запись результата в канал
	write(fd[WR], &result, sizeof(uint64_t));

	exit(0);							// Завершение работы процесса-потомка
}