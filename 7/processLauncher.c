#include "processLauncher.h"
#include "programList.h"
#include "processList.h"

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>

#define CHILD  0			// Id процесса-потомка
#define ERROR -1			// Код ошибки

#define NO_OPTIONS 0		// Ожидание процесса без указанных опций

#define ACCESS_RIGHTS 0666	// Права доступа очереди сообщений

typedef struct Message {	// Тип данных очереди сообщений
	long type;				// Тип сообщения
	const char* name;		// Передаваемое имя программы
} Message;

// Основное поведение процесса -- передача названия программы в очередь сообщений
static void processBehavior(ProgramList programs, key_t msgq_key) {
	int msgq_id = msgget(msgq_key, IPC_EXCL | ACCESS_RIGHTS);
	if (msgq_id == ERROR)
		fprintf(stderr, "Error: msgget() error!\n");

	// Формирование сообщеня, получение случайного названия программы
	srand(time(NULL) * getpid());
	Message msg = {1, getRandomProgramName(programs)};

	// Передача сообщения в очередь сообщений
	ssize_t sended = msgsnd(msgq_id, &msg, sizeof(Message) - sizeof(long), IPC_NOWAIT);
	if (sended == ERROR)
		fprintf(stderr, "Error: sending error!\n");

	exit(EXIT_SUCCESS);
}

// Создание процессов потомков для формирования содержимого в очереди сообщений
static void createChildProcesses(uint8_t process_count, key_t msgq_key) {
	ProgramList programs;
	initProgramList(&programs);

	pid_t* pid = (pid_t*)malloc(sizeof(pid_t) * process_count);

	for (uint8_t i = 0; i != process_count; ++i)
		if ((pid[i] = fork()) == ERROR)
			fprintf(stderr, "Error: child %hhu fork error!\n", i);
		else
			if (pid[i] == CHILD)
				processBehavior(programs, msgq_key);

	for (uint8_t i = 0; i != process_count; ++i)
		if (pid[i] != waitpid(pid[i], NULL, NO_OPTIONS))
			fprintf(stderr, "Error: child %hhu isn't done!\n", i);

	free(pid);
}

// Запуск программ из очереди сообщении и поддержание постоянного указанного кол-ва активных процессов
static void launchProgramProcesses(const char** prog_name, uint8_t proc_count, uint8_t active_proc_count) {
	pid_t* pid = (pid_t*)malloc(sizeof(pid_t) * active_proc_count);

	// Создание и инициализация списка активных процессов
	ProcessList proc_list;
	initProcessList(&proc_list, active_proc_count);

	// Выполнять создание и закрытие процессов до тех пор,
	// пока кол-во закрытых процессов не будет равно числу процессов
	while (proc_list.closed != proc_count) {
		for (uint8_t i = 0; i != proc_list.size && proc_list.amount != proc_count; ++i)
			if (proc_list.status[i] == INACTIVE) {		// Если процесс неактивен --
				makeActiveProcess(&proc_list, i);		// присвоить ему статус "АКТИВЕН"

				if ((pid[i] = fork()) == ERROR)
					fprintf(stderr, "Error: child %hhu fork error!\n", i);
				else
					if (pid[i] == CHILD)	// Запустить программу из списка программ в новом терминале
						execl("/usr/bin/xterm", "xterm", "-hold", "-e", prog_name[proc_list.amount - 1], NULL);
			}

		// Если процесс завершён -- закрыть процесс и присвоить ему статус "НЕАКТИВЕН"
		for (uint8_t i = 0; i != proc_list.size; ++i)
			if (pid[i] == waitpid(pid[i], NULL, WNOHANG))
				makeInactiveProcess(&proc_list, i);
	}

	free(pid);
	freeProcessList(&proc_list);
}

// Поддерживать в активном состоянии указанное число процессов из общего кол-ва
void start(uint8_t proc_count, uint8_t active_proc_count) {
	if (active_proc_count <= proc_count) {
		// Сгенерировать ключ и создать очередь сообщений
		key_t msgq_key = ftok(".", 'q');
		int msgq_id = msgget(msgq_key, IPC_CREAT | ACCESS_RIGHTS);

		// Создать дочерние процессы для генерации наполнения очереди сообщений
		createChildProcesses(proc_count, msgq_key);

		const char** prog_name = (const char**)malloc(sizeof(const char*) * proc_count);

		uint8_t received_count = 0;
		while (received_count != proc_count) {
			// Считать данные из очереди сообщений
			Message msg = {1, "Empty"};
			ssize_t received = msgrcv(msgq_id, &msg, sizeof(Message) - sizeof(long), msg.type, NO_OPTIONS);

			// Заполнить список названий программ из очереди сообщений
			if (received != ERROR)
				prog_name[received_count++] = msg.name;
		}

		// Закрыть очередь сообщений
		msgctl(msgq_id, IPC_RMID, NULL);

		printf("Start running processes...\n");
		launchProgramProcesses(prog_name, proc_count, active_proc_count);

		printf("All processes are completed!\n");
		free(prog_name);
	}
	else
		fprintf(stderr, "Error: active processes count can't exceed the total count of processes!\n");
}
