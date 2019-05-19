#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>


// Функция объединения двух строк
static inline char* concat(const char* s1, const char* s2) {
    char* result = (char*)malloc(strlen(s1) + strlen(s2) + 1); 
    strcpy(result, s1);
    strcat(result, s2);
    
    return result;
}

// Проверка, является ли имя псевдонимом пути
static inline bool isNotDots(const char* name) {
	return strcmp(name, ".") && strcmp(name, "..");
}

// Проверка, является ли полный путь к файлу директорией
static inline bool isDirectory(const char* path) {
	struct stat path_stat;
	stat(path, &path_stat);

	return !S_ISREG(path_stat.st_mode);
}

// Вывод всех папок, содержащих подпапки, в указанной директории
void printNonEmptySubfolders(const char* path);

#endif
