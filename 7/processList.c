#include "processList.h"

// Инициализировать список отслеживаемых процессов
void initProcessList(ProcessList* proc_list, uint8_t size) {
	proc_list->size = size;
	proc_list->status = (ProcessStatus*)malloc(sizeof(ProcessStatus) * proc_list->size);

	// Присвоить всем процессам статус "НЕАКТИВЕН"
	for (uint8_t i = 0; i != proc_list->size; ++i)
		proc_list->status[i] = INACTIVE;

	proc_list->active = 0;
	proc_list->closed = 0;
	proc_list->amount = 0;
}

// Очистить список отслеживаемых процессов
void freeProcessList(ProcessList* proc_list) {
	free(proc_list->status);
	proc_list->size = 0;

	proc_list->active = 0;
	proc_list->closed = 0;
	proc_list->amount = 0;
}

// Сделать активным процесс под указанным номером
void makeActiveProcess(ProcessList* proc_list, uint8_t proc_num) {
	if (proc_num < proc_list->size) {
		proc_list->status[proc_num] = ACTIVE;

		// Увеличить ко-во активных процессов
		++proc_list->active;
		// Изменить общее число процессов
		proc_list->amount = proc_list->active + proc_list->closed;
	}
}

// Сделать неактивным процесс под указанным номером
void makeInactiveProcess(ProcessList* proc_list, uint8_t proc_num) {
	if (proc_num < proc_list->size) {
		proc_list->status[proc_num] = INACTIVE;

		// Уменьшить ко-во активных процессов
		--proc_list->active;
		// Увеличить ко-во закрытых процессов
		++proc_list->closed;
		// Изменить общее число процессов
		proc_list->amount = proc_list->active + proc_list->closed;
	}
}
