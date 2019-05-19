#ifndef PROGRAMLIST_H
#define PROGRAMLIST_H

#include <stdlib.h>
#include <time.h>

#define PROGRAM_LIST_SIZE 5		// Кол-во программ

// Список названий программ для запуска
typedef struct ProgramList {
	const char* name[PROGRAM_LIST_SIZE];
} ProgramList;

// Инициализация списка названий программ
static inline void initProgramList(ProgramList* prog_list) {
	prog_list->name[0] = "./NonEmptyFoldersPrint";
	prog_list->name[1] = "./FileProcess";
	prog_list->name[2] = "./Combinations";
	prog_list->name[3] = "./MessageExchanger";
	prog_list->name[4] = "./CommandPipeline";
}

// Получить случайное название программу из списка
static inline const char* getRandomProgramName(ProgramList prog_list) {
	return prog_list.name[rand() % PROGRAM_LIST_SIZE];
}

#endif
