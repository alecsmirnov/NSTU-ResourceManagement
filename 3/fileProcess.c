#include "fileProcess.h"

#include <stdbool.h>
#include <string.h>
#include <malloc.h>

#include <sys/stat.h>
#include <fcntl.h>

#define EQUAL     0		// Код равенства строк
#define ERROR    -1		// Код ошибки

#define STD_FLAG -1		// Флаг стандартного потока

#define FNAME_COLUMN 7			// Длина столбца имени файла
#define INO_COLUMN  1000000		// Длина столбца Inode

// Инициализация памяти
void memoryInit(Memory* memory, uint32_t tablesize_max) {
	memory->ft_size = 0;
	memory->oft_size = 0;
	memory->fdt_size = 0;

	memory->ft = (FileTable*)malloc(sizeof(FileTable) * tablesize_max);
	memory->oft = (OpenFileTable*)malloc(sizeof(OpenFileTable) * tablesize_max);
	memory->fdt = (FileDescTable*)malloc(sizeof(FileDescTable) * tablesize_max);
}

// Очистка памяти
void memoryFree(Memory* memory) {
	memory->ft_size = 0;
	memory->oft_size = 0;
	memory->fdt_size = 0;

	free(memory->ft);
	free(memory->oft);
	free(memory->fdt);

	memory->ft = NULL;
	memory->oft = NULL;
	memory->fdt = NULL;
}

// Инициализация элемента таблицы файлов
static void fileTableElemInit(FileTable* table_elem, const char* filename, ino_t ino, int flags) {
	table_elem->filename = filename;	// Имя файла
	table_elem->ino = ino;				// Номер inode
	table_elem->flags = flags;			// Флаг открытия
	table_elem->nlink = 1;				// Количество ссылок
}

// Инициализация элемента таблицы открытых файлов
static void openFileTableElemInit(OpenFileTable* table_elem, const char* filename, int fd, int flags) {
	table_elem->filename = filename;	// Имя файла
	table_elem->fd = fd;				// Описатель файла
	table_elem->flags = flags;			// Флаг открытия
}

// Инициализация элемента таблицы описателей файлов
static void fileDescTableElemInit(FileDescTable* table_elem, const char* filename, const struct stat* st) {
	table_elem->filename = filename;			// Имя файла
	table_elem->dev = st->st_dev;				// Устройство
	table_elem->ino = st->st_ino;				// Номер inode
	table_elem->mode = st->st_mode;				// Режим доступа
	table_elem->nlink = st->st_nlink;			// Количество ссылок
	table_elem->uid = st->st_uid;				// Id пользователя-владельца
	table_elem->gid = st->st_gid;				// Id группы-владельца
	table_elem->rdev = st->st_rdev;				// Тип устройства
	table_elem->size = st->st_size;				// Общий размер в байтах
	table_elem->blksize = st->st_blksize;		// Размер блока ввода-вывода
	table_elem->blocks = st->st_blocks;			// Количество выделенных блоков
	table_elem->atime = st->st_atime;			// Время последнего доступа
	table_elem->mtime = st->st_mtime;			// Время последней модификации
	table_elem->ctime = st->st_ctime;			// Время последнего изменения
}

// Получение стандартного имени потока по его описателю
static const char* getSTDName(int std_fd) {
	const char* std_name;

	switch (std_fd) {
		case STDIN_FILENO:  std_name = "stdin";  break;
		case STDOUT_FILENO: std_name = "stdout"; break;
		case STDERR_FILENO: std_name = "stderr"; break;
	}

	return std_name;
}

// Получение строкового представления флага по целому значению
static const char* getStrFlag(int flag) {
	const char* str_flag;

	switch (flag) {
		case 0 : str_flag = "RD"; break;
		case 1 : str_flag = "WR"; break;
		case 2 : str_flag = "RDWR"; break;
		default: str_flag = " ___"; break;
	}

	return str_flag;
}

// Функция имитирующая работу функции open()
int myOpen(Memory* memory, const char* pathname, int flags) {
	int fd = open(pathname, flags);		// Получение описателя файла

	if (fd == ERROR)
		fprintf(stderr, "Error: cannot open %s!\n", pathname);
	else {
		struct stat st;

		if (stat(pathname, &st) == ERROR)	// Получение информации о файле
			fprintf(stderr, "Error: stat failure!\n");
		else {
			// Проверяем есть ли такой файл в таблице файлов.
			// Если файла есть в таблице -- увеличиваем количество ссылок на него
			bool exist = false;
			for (uint32_t i = 0; i != memory->ft_size && !exist; ++i)
				if (memory->ft[i].ino == st.st_ino && memory->ft[i].flags == flags) {
					++memory->ft[i].nlink;
					exist = true;
				}

			// Если файла нет в таблице или таблица пуста --
			// записываем информацию о файле в таблицу файлов
			if (!exist || memory->ft_size == 0)
				fileTableElemInit(&memory->ft[memory->ft_size++], pathname, st.st_ino, flags);

			// Записываем информации о файле в таблицу открытых файлов
			openFileTableElemInit(&memory->oft[memory->oft_size++], pathname, fd, flags);

			// Провермяем есть ли такой файл в таблице описателей файлов.
			// Если файл есть в таблице -- увеличиваем количество ссылок на него
			exist = false;
			for (uint32_t i = 0; i != memory->fdt_size && !exist; ++i)
				if (memory->fdt[i].ino == st.st_ino) {
					++memory->fdt[i].nlink;
					exist = true;
				}

			// Если файла нет в таблице или таблица пуста --
			// записываем информацию о файле в таблицу описателей файлов
			if (!exist || memory->fdt_size == 0)
				fileDescTableElemInit(&memory->fdt[memory->fdt_size++], pathname, &st);
		}
	}

	return fd;		// Возвращаем описатель
}

// Функция имитирующая работу функции open() для стандартных потоков
int mySTDOpen(Memory* memory, int std_fd) {
	int fd = ERROR;

	// Провермяем есть ли такой поток в таблице открытых файлов.
	// Если поток есть в таблице -- завершаем работу и возвращаем ERROR
	bool found = false;
	for (uint32_t i = 0; i != memory->oft_size && !found; ++i)
		if (memory->oft[i].fd == std_fd)
			found = true;

	// Если стандартного потока нет в таблице
	if (!found) {
		struct stat st;

		// Получаем информацию о стандартном потоке
		if (fstat(std_fd, &st) == ERROR)
			fprintf(stderr, "Error: fstat failure!\n");
		else {
			fd = std_fd;								// Записываем описатель
			const char* std_name = getSTDName(fd);		// Получаем имя по описателю

			// Записываем стандартный поток во все три таблицы файлов
			fileTableElemInit(&memory->ft[memory->ft_size++], std_name, st.st_ino, STD_FLAG);
			openFileTableElemInit(&memory->oft[memory->oft_size++], std_name, std_fd, STD_FLAG);
			fileDescTableElemInit(&memory->fdt[memory->fdt_size++], std_name, &st);
		}
	}

	return fd;	// Возвращаем описатель или код ошибки
}

// Функция имитирующая работу функции close()
int myClose(Memory* memory, int fd) {
	const char* filename;
	int flags;

	// Проверяем есть ли такой дескриптор в таблице открытых файлов.
	// Если дескриптор есть в таблице, удаляем строку из таблцы открытых файлов
	bool found = false;
	for (uint32_t i = 0; i != memory->oft_size && !found; ++i)
		if (memory->oft[i].fd == fd) {
			filename = memory->oft[i].filename;
			flags = memory->oft[i].flags;

			for (uint32_t j = i; j != memory->oft_size - 1; ++j)
				memory->oft[j] = memory->oft[j + 1];
			--memory->oft_size;

			found = true;
		}

	// Проверяем есть ли такой дескриптор в таблице файлов.
	// Если дескриптор есть в таблице, удаляем строку из таблцы файлов
	found = false;
	for (uint32_t i = 0; i != memory->ft_size && !found; ++i)
		if (strcmp(memory->ft[i].filename, filename) == EQUAL && memory->ft[i].flags == flags) {
			if (memory->ft[i].nlink - 1 != 0)
				--memory->ft[i].nlink;
			else {
				for (uint32_t j = i; j != memory->ft_size - 1; ++j)
					memory->ft[j] = memory->ft[j + 1];
				--memory->ft_size;
			}

			found = true;
		}

	// Проверяем есть ли такой дескриптор в таблице описателей файлов.
	// Если дескриптор есть в таблице, удаляем строку из таблцы описателей файлов
	found = false;
	for (uint32_t i = 0; i != memory->fdt_size && !found; ++i) {
		if (strcmp(memory->fdt[i].filename, filename) == EQUAL) {
			if (memory->fdt[i].nlink - 1 != 0)
				--memory->fdt[i].nlink;
			else {
				for (uint32_t j = i; j != memory->fdt_size - 1; ++j)
					memory->fdt[j] = memory->fdt[j + 1];
				--memory->fdt_size;
			}

			found = true;
		}
	}

	return close(fd);		// Закрываем описатель и выводим код завершения
}

// Функция имитирующая работу функции close() для стандартных потоков
int mySTDClose(Memory* memory, int std_fd) {
	int close = ERROR;

	// Проверяем есть ли такой дескриптор в таблице открытых файлов.
	// Если дескриптор есть в таблице, удаляем строку из таблцы открытых файлов
	bool found = false;
	for (uint32_t i = 0; i != memory->oft_size && !found; ++i) {
		if (memory->oft[i].fd == std_fd) {
			for (uint32_t j = i; j != memory->oft_size - 1; ++j)
				memory->oft[j] = memory->oft[j + 1];
			--memory->oft_size;

			found = true;
		}
	}

	// Проверяем есть ли такой дескриптор в таблице файлов.
	// Если дескриптор есть в таблице, удаляем строку из таблцы файлов
	found = false;
	for (uint32_t i = 0; i != memory->ft_size && !found; ++i) {
		if (strcmp(memory->ft[i].filename, getSTDName(std_fd)) == EQUAL) {
			for (uint32_t j = i; j != memory->ft_size - 1; ++j)
				memory->ft[j] = memory->ft[j + 1];
			--memory->ft_size;

			found = true;
		}
	}

	// Проверяем есть ли такой дескриптор в таблице описателей файлов.
	// Если дескриптор есть в таблице, удаляем строку из таблцы описателей файлов
	found = false;
	for (uint32_t i = 0; i != memory->fdt_size && !found; ++i) {
		if (strcmp(memory->fdt[i].filename, getSTDName(std_fd)) == EQUAL) {
			for (uint32_t j = i; j != memory->fdt_size - 1; ++j)
				memory->fdt[j] = memory->fdt[j + 1];
			--memory->fdt_size;

			found = true;
		}
	}

	return close;	// Возвращаем код завершения
}

// Вывод на экран таблиц файлов
void printTables(Memory* memory) {
	puts("\nOpen file table");			// Таблица открытых файлов
	puts("________________________________________");
	printf("Filename\t| File desc\t| Flag\t|\n");
	printf("================|===============|=======|\n");
	for (uint32_t i = 0; i != memory->oft_size; ++i)
		strlen(memory->oft[i].filename) < FNAME_COLUMN ?
			printf("%s\t\t| %d\t\t| %s\t|\n", memory->oft[i].filename, memory->oft[i].fd, getStrFlag(memory->oft[i].flags)) :
			printf("%s\t| %d\t\t| %s\t|\n", memory->oft[i].filename, memory->oft[i].fd, getStrFlag(memory->oft[i].flags));
	puts("________________|_______________|_______|");

	puts("\nFile table");				// Таблица файлов
	puts("________________________________________________");
	printf("Filename\t| Inode\t\t| Flag\t| NLink |\n");
	printf("================|===============|=======|=======|\n");
	for (uint32_t i = 0; i != memory->ft_size; ++i)
		memory->ft[i].ino < INO_COLUMN ?
			printf("%s\t\t| %zd\t\t| %s \t| %zd\t|\n",
				   memory->ft[i].filename, memory->ft[i].ino, getStrFlag(memory->ft[i].flags), memory->ft[i].nlink) :
			printf("%s\t| %zd\t| %s \t| %zd\t|\n",
				 memory->ft[i].filename, memory->ft[i].ino, getStrFlag(memory->ft[i].flags), memory->ft[i].nlink);
	puts("________________|_______________|_______|_______|");

	puts("\nFile descriptor table");	// Таблица описателей файлов
	puts("________________________________________________________");
	printf("Filename\t| Inode\t\t| NLink | Mode\t | Size\t|\n");
	printf("================|===============|=======|========|======|\n");
	for (uint32_t i = 0; i != memory->fdt_size; ++i)
		memory->fdt[i].ino < INO_COLUMN ?
			printf("%s\t\t| %zd\t\t| %zd\t| %o  | %li\t|\n",
				   memory->fdt[i].filename, memory->fdt[i].ino, memory->fdt[i].nlink, memory->fdt[i].mode, memory->fdt[i].size) :
			printf("%s\t| %zd\t| %zd\t| %o | %li\t|\n",
				memory->fdt[i].filename, memory->fdt[i].ino, memory->fdt[i].nlink, memory->fdt[i].mode, memory->fdt[i].size);
	puts("________________|_______________|_______|________|______|");
}
