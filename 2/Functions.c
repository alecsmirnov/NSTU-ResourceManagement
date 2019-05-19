#include "Functions.h"

#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>


// Вывод всех папок, содержащих подпапки, в указанной директории
void printNonEmptySubfolders(const char* path) {
	DIR* dir = opendir(path);			// Начальная директория

	if (dir == NULL)					// Проверка на открытие каталога
		fprintf(stderr, "Error: can't open current dir!\n");
	else {
		struct dirent* entry;

		// Просматриваем всё содержимое начальной директории
		// и проверяем является ли файл каталогом и не псевдонимом
		while ((entry = readdir(dir)))	
			if (isDirectory(entry->d_name) && isNotDots(entry->d_name)) {
				// Формируем полный путь до каталога в начальной директории
    			char* full_path = concat(path, entry->d_name);
    			DIR* subdir = opendir(full_path);

				if (subdir == NULL)		// Проверка на открытие каталога
					fprintf(stderr, "Error: can't open \"%s\" subdir!\n", entry->d_name);
				else {
					struct dirent* subentry;

					// Просматриваем всё содержимое каталога до тех пор,
					// пока не найдём в нём подкаталог
					bool subfolder_search = true;
					while ((subentry = readdir(subdir)) && subfolder_search) {
						// Формируем путь до файла в каталоге
						char* temp = concat(full_path, "/");
						char* full_subpath = concat(temp, subentry->d_name);

						// Проверяем, является ли файл в каталоге подкаталогом и не псевдонимом
						if (isDirectory(full_subpath) && isNotDots(subentry->d_name))
							subfolder_search = false;

						free(temp);
						free(full_subpath);
					}

					// Если в каталоге был найден подкаталог -- выводим название каталога
					if (!subfolder_search)
						printf("\"%s\" is a non-empty folder with subfolder!\n", entry->d_name);
				}

				free(full_path);
				closedir(subdir);
			}
	}

	closedir(dir);
}
