#ifndef PROCESSLIST_H
#define PROCESSLIST_H

#include <stdlib.h>
#include <stdint.h>

// Состояние процесса в списке отслеживаемых процессов
typedef enum ProcessStatus {
	ACTIVE,				// Активный процесс
	INACTIVE			// Неактивный процесс
} ProcessStatus;

// Список отслеживаемых процессов
typedef struct ProcessList {
	ProcessStatus* status;	// Статус процесса
	uint8_t size;			// Размер списка

	uint8_t active;			// Кол-во активных процессов
	uint8_t closed;			// Кол-во закрытых процессов
	uint8_t amount;			// Общее число процессов
} ProcessList;

// Инициализировать список отслеживаемых процессов
void initProcessList(ProcessList* proc_list, uint8_t size);
// Очистить список отслеживаемых процессов
void freeProcessList(ProcessList* proc_list);

// Сделать активным процесс под указанным номером
void makeActiveProcess(ProcessList* proc_list, uint8_t proc_num);
// Сделать неактивным процесс под указанным номером
void makeInactiveProcess(ProcessList* proc_browser, uint8_t proc_num);

#endif
