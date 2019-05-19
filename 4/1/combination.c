#include "combination.h"

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

#define PROCESS_COUNT 3		// Кол-во процессов

#define INCOMPLETE_DATA 0	// Неполные данные при пересылке
#define NO_OPTIONS		0	// Ожидание процесса без указанных опций

#define ERR  -1				// Код ошибки
#define CHILD 0				// Id процесса-потомка

#define RD	  0				// Чтение из канала
#define WR	  1				// Запись в канал
#define RD_WR 2				// Количество операций в канале

// Получить количество сочетаний из C(n, k)
uint64_t calculateCombination(uint8_t n, uint8_t k) {
	pid_t pid[PROCESS_COUNT];			// Кол-во процессов потомков
	int   fds[PROCESS_COUNT][RD_WR];	// Кол-во каналов обмена сообщений

	uint64_t n_fact;					// n!
	uint64_t k_fact;					// k!
	uint64_t nk_fact;					// (n - k)!

	// Порождение процессов потомков
	for (uint8_t i = 0; i != PROCESS_COUNT; ++i) {
		pipe(fds[i]);
		pid[i] = fork();

		if (pid[i] == ERR)
			fprintf(stderr, "Error: child fork error!\n");
		else
			if (pid[i] == CHILD) {		// Разделение вычислений на каждого потомка
				uint8_t val = 0;

				switch (i) {
					case 0: val = n;     break;
					case 1: val = k;     break;
					case 2: val = n - k; break;
					default: break;
				}

				char str_val[9], str_pipe[RD_WR][5];

				snprintf(str_val, sizeof(str_val), "%hhu", val);
				snprintf(str_pipe[RD], sizeof(str_pipe[RD]), "%d", fds[i][RD]);
				snprintf(str_pipe[WR], sizeof(str_pipe[WR]), "%d", fds[i][WR]);

				execl("./childProcessMain", "childProcessMain",  str_val, str_pipe[RD], str_pipe[WR], NULL);
			}
	}

	// Асинхронный приём сообщений, не требующий ожидания, для каналов
	for (uint8_t i = 0; i != PROCESS_COUNT; ++i)
		fcntl(fds[i][RD], F_SETFL, fcntl(fds[i][RD], F_GETFL) | O_NONBLOCK);

	uint8_t closed_processes = 0;		// Кол-во завершённых процессов-потомков
	while (closed_processes != PROCESS_COUNT)
		for (uint8_t i = 0; i != PROCESS_COUNT; ++i) {	// Проверяем каналы на наличие сообщений
			close(fds[i][WR]);							// до тех пор, пока не завершены все процессы-потомки

			uint64_t factorial_val;
			ssize_t received = read(fds[i][RD], &factorial_val, sizeof(uint64_t));

			if (received > INCOMPLETE_DATA) {			// Если принято сообщение от процесса --
				++closed_processes;						// сохраняем результат в соответствующую переменную
														// и увеличиваем кол-во завершённых процессов
				switch (i) {
					case 0: n_fact  = factorial_val; break;
					case 1: k_fact  = factorial_val; break;
					case 2: nk_fact = factorial_val; break;
					default: break;
				}
			}
		}

	for (uint8_t i = 0; i != PROCESS_COUNT; ++i)		// Приостанавливаем выполнение всех процессов-потомков
		if (pid[i] != waitpid(pid[i], NULL, NO_OPTIONS))
			fprintf(stderr, "Error: child isn't done!\n");

	return combination(n_fact, k_fact, nk_fact);		// Вычиляем сочетание по факториалам и возвращаем результат
}
