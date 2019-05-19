#include "commandPipeline.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

#define PROCESS_COUNT 3		// Кол-во процессов
#define PROCESS_1	  0		// Id первого процесса-потомка

#define NO_OPTIONS	  0		// Ожидание процесса без указанных опций

#define ERR  -1				// Код ошибки
#define CHILD 0				// Id процесса-потомка

#define RD	  0				// Чтение из канала
#define WR	  1				// Запись в канал
#define RD_WR 2				// Количество операций в канале

// Номер канала, работу с которым завершил процесс
#define processPipe(pid) ((pid) - 1)

// Первый процесс в конвейере команд:
// чтение текстового файла и удаление из него всех цифр
static void processOne(int fd_0[RD_WR], const char* filename) {
	fprintf(stderr, "P0:\tStart.\n");

	fprintf(stderr, "P0:\tOpen file \"%s\" for reading.\n", filename);
	int fd_tmp = open(filename, O_RDONLY);

	if (fd_tmp == ERR) {
		fprintf(stderr, "Error: \"%s\" file not found!\n", filename);
		exit(EXIT_FAILURE);
	}
	else {
		fprintf(stderr, "P0:\tRedirect input from \"%s\" file.\n", filename);
		dup2(fd_tmp, STDIN_FILENO);
		close(fd_tmp);

		fprintf(stderr, "P0:\tRedirect output to the \"fd_0\".\n");
		close(fd_0[RD]);
		dup2(fd_0[WR], STDOUT_FILENO);
		close(fd_0[WR]);

		fprintf(stderr, "P0:\tExecute \"tr\" utility.\n");
		execl("/usr/bin/tr", "tr", "-d", "\"[0-9]\"", NULL);
		exit(EXIT_SUCCESS);
	}
}

// Второй процесс в конвейере команд:
// сортировка всех слов в алфавитном порядке
static void processTwo(int fd[PROCESS_COUNT - 1][RD_WR]) {
	fprintf(stderr, "P1:\tStart.\n");

	fprintf(stderr, "P1:\tRedirect input from the \"fd_0\".\n");
	close(fd[0][WR]);
	dup2(fd[0][RD], STDIN_FILENO);
	close(fd[0][RD]);

	fprintf(stderr, "P1:\tRedirect output to the \"fd_1\".\n");
	close(fd[1][RD]);
	dup2(fd[1][WR], STDOUT_FILENO);
	close(fd[1][WR]);

	fprintf(stderr, "P1:\tExecute \"sort\" utility.\n");
	execl("/usr/bin/sort", "sort", NULL);
	exit(EXIT_SUCCESS);
}

// Третий процесс в конвейере команд:
// удаление дубликатов и помещение результата в текстовый файл
static void processThree(int fd_1[RD_WR], const char* filename) {
	fprintf(stderr, "P2:\tStart.\n");

	fprintf(stderr, "P2:\tOpen file \"%s\" for writing.\n", filename);
	int fd_tmp = open(filename, O_CREAT | O_RDWR | O_TRUNC);

	if (fd_tmp == ERR) {
		fprintf(stderr, "Error: \"%s\" file not found!\n", filename);
		exit(EXIT_FAILURE);
	}
	else {
		fprintf(stderr, "P2:\tRedirect input from the \"fd_1\".\n");
		close(fd_1[WR]);
		dup2(fd_1[RD], STDIN_FILENO);
		close(fd_1[RD]);

		fprintf(stderr, "P2:\tRedirect output to \"%s\" file.\n", filename);
		dup2(fd_tmp, STDOUT_FILENO);
		close(fd_tmp);

		fprintf(stderr, "P2:\tExecute \"uniq\" utility.\n");
		execl("/usr/bin/uniq", "uniq", NULL);
		exit(EXIT_SUCCESS);
	}
}

// Основной процесс (родительский процесс).
// Осуществляет последовательное выполнение указанных процессов и команд
static void processMain(const char* input_filename, const char* output_filename) {
	pid_t pid[PROCESS_COUNT];			// Кол-во процессов потомков

	int fd[PROCESS_COUNT - 1][RD_WR];	// Кол-во каналов для обмена между процессами
	for (uint8_t i = 0; i != PROCESS_COUNT - 1; ++i)
		pipe(fd[i]);

	fprintf(stderr, "Parent:\tPipes were created.\n");

	for (uint8_t i = 0; i != PROCESS_COUNT; ++i) {		// Порождение процессов потомков
		if ((pid[i] = fork()) == ERR)
			fprintf(stderr, "Error: child %hhu fork error!\n", i);
		else
			if (pid[i] == CHILD) {						// Поочерёдный запуск процессов-потомков
				fprintf(stderr, "Parent:\tProcess %d was created.\n", i);

				switch (i) {
					case 0: processOne(fd[0], input_filename);    break;
					case 1: processTwo(fd);						  break;
					case 2: processThree(fd[1], output_filename); break;
				}
			}

		// Закрытие неиспользуемых каналов по окончании работы
		// последнего используемого процесса-потомка
		if (i != PROCESS_1) {
			close(fd[processPipe(i)][RD]);
			close(fd[processPipe(i)][WR]);
		}

		// Поочерёдное ожидание завершения процессов-потомков
		if (pid[i] != waitpid(pid[i], NULL, NO_OPTIONS))
			fprintf(stderr, "Error: child %hhu isn't done!\n", i);
		else
			fprintf(stderr, "P%hhu:\tShutdown.\n", i);
	}

	fprintf(stderr, "Parent:\tAll processes are done.\n");
}

// Проверка файла на существование
static bool isFileExists(const char* filename) {
	return access(filename, F_OK) != ERR;
}

// Запуск конвейера команд: 
// tr -d "[0-9]" < input_filename | sort | uniq > output_filename
void startComminication(const char* input_filename, const char* output_filename) {
	if (isFileExists(input_filename))		// Проверка входного файла на существование
		processMain(input_filename, output_filename);
	else
		fprintf(stderr, "Error: \"%s\" file doesn't exist!\n", input_filename);
}
