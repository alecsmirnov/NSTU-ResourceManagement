#ifndef FILEPROCESS_H
#define FILEPROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <unistd.h>
#include <fcntl.h>

// Представление элемента таблицы файлов
typedef struct FileTable {
	const char*	filename;	// Название файла
	ino_t ino;				// Номер inode
	int flags;				// Режим открытия файла
	nlink_t nlink;			// Количество ссылок
} FileTable;

// Представление элемента таблицы открытых файлов
typedef struct OpenFileTable {
	const char* filename;	// Название файла
	int fd;					// Описатель файла
	int flags;				// Флаги файла
} OpenFileTable;

// Представление элемента таблицы описателей файлов
typedef struct FileDescTable {
	const char* filename;	// Название файла
	dev_t dev;				// Устройство
	ino_t ino;				// Номер inode
	mode_t mode;			// Режим доступа
	nlink_t nlink;			// Количество ссылок
	uid_t uid;				// Id пользователя-владельца
	gid_t gid;				// Id группы-владельца
	dev_t rdev;				// Тип устройства
	off_t size;				// Общий размер в байтах
	blksize_t blksize;		// Размер блока ввода-вывода
	blkcnt_t blocks;		// Количество выделенных блоков
	time_t atime;			// Время последнего доступа
	time_t mtime;			// Время последней модификации
	time_t ctime;			// Время последнего изменения
} FileDescTable;

// Структура представления памяти
typedef struct Memory {
	FileTable* ft;		// Таблица файлов
	OpenFileTable* oft;	// Таблица открытых файлов
	FileDescTable* fdt;	// Таблица описателей файлов

	uint32_t ft_size;	// Число элементов таблицы файлов
	uint32_t oft_size;	// Число элементов таблицы открытых файлов
	uint32_t fdt_size;	// Число элементов таблицы описателей файлов
} Memory;

// Инициализация памяти
void memoryInit(Memory* memory, uint32_t tablesize_max);
// Очистка памяти
void memoryFree(Memory* memory);

// Функция имитирующая работу функции open()
int myOpen(Memory* memory, const char* pathname, int flags);
// Функция имитирующая работу функции open() для стандартных потоков
int mySTDOpen(Memory* memory, int std_fd);

// Функция имитирующая работу функции close()
int myClose(Memory* memory, int fd);
// Функция имитирующая работу функции close() для стандартных потоков
int mySTDClose(Memory* memory, int std_fd);

// Вывод на экран таблиц файлов
void printTables(Memory* memory);

#endif
