#include "combination.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define PROCESS_COUNT 3		// Кол-во процессов
#define NO_OPTIONS	  0		// Ожидание процесса без указанных опций

#define ERR  -1				// Код ошибки
#define CHILD 0				// Id процесса-потомка

#define FILENAME "./.tmp"	// Временный файл для обмена информации

static bool record = true;	// Состоянии записи результата процесса-потомка 

// Обработка сигнала, посланного процессу потомку
static void signalHandler(int signal_num) {
	if (signal_num == SIGUSR1)		// Прекратить запись,
		record = false;				// если получен сигнал
}

// Поведение для процесса потомка
static void childBehavior(uint64_t val, uint8_t id) {
	signal(SIGUSR1, signalHandler);
	uint64_t result = factorial(val);				// Подсчёт факториала

	while (record)									// Перезаписывать файл,
		if (access(FILENAME, F_OK) == ERR) {		// пока не получено сообщение
			FILE* fp = NULL;						// о завершении работы
			if ((fp = fopen(FILENAME, "w")) == NULL)
				fprintf(stderr, "Error: child can't open file!\n");

			fprintf(fp, "%lu %hhu\n", result, id);	// Запись результата в канал

			if (fclose(fp) != 0)
				fprintf(stderr, "Error: child file close error!\n");
		}

	exit(0);		// Завершение работы процесса-потомка
}

// Получить количество сочетаний из C(n, k)
uint64_t calculateCombination(uint8_t n, uint8_t k) {
	pid_t pid[PROCESS_COUNT];			// Кол-во процессов потомков

	uint64_t n_fact;					// n!
	uint64_t k_fact;					// k!
	uint64_t nk_fact;					// (n - k)!

	// Порождение процессов потомков
	for (uint8_t i = 0; i != PROCESS_COUNT; ++i) {
		pid[i] = fork();

		if (pid[i] == ERR)
			fprintf(stderr, "Error: child fork error!\n");
		else
			if (pid[i] == CHILD)	// Разделение вычислений на каждого потомка
				switch (i) {
					case 0: childBehavior(n, i);     break;
					case 1: childBehavior(k, i);     break;
					case 2: childBehavior(n - k, i); break;
					default: break;
				}
	}

	uint8_t closed_processes = 0;		// Кол-во завершённых процессов-потомков
	while (closed_processes != PROCESS_COUNT) 		// Переоткрывать файл для проверки
		if (access(FILENAME, F_OK) != ERR) {		// ещё не полученных сообщений от потомков
			FILE* fp = NULL;
			if ((fp = fopen(FILENAME, "r")) == NULL)
				fprintf(stderr, "Error: can't open file!\n");

			uint64_t tmp_val;
			uint8_t  tmp_id;
			if (fscanf(fp, "%lu %hhu\n", &tmp_val, &tmp_id) == 2) {	// Если принято сообщение от процесса --
				++closed_processes;									// сохраняем результат в соответствующую переменную,
				kill(pid[tmp_id], SIGUSR1);							// увеличиваем кол-во завершённых процессов
																	// и посылаем сигнал на завершения работы потомку
				switch (tmp_id) {
					case 0: n_fact  = tmp_val; break;
					case 1: k_fact  = tmp_val; break;
					case 2: nk_fact = tmp_val; break;
					default: break;
				}
			}

			if (fclose(fp) != 0)									// Закрыть и удалить файл
				fprintf(stderr, "Error: file close error!\n");
			if (remove(FILENAME) != 0)
				fprintf(stderr, "Error: deleting file error!");
        }

	for (uint8_t i = 0; i != PROCESS_COUNT; ++i)		// Приостанавливаем выполнение всех процессов-потомков
		if (pid[i] != waitpid(pid[i], NULL, NO_OPTIONS))
			fprintf(stderr, "Error: child isn't done!\n");

	return combination(n_fact, k_fact, nk_fact);		// Вычиляем сочетание по факториалам и возвращаем результат
}
