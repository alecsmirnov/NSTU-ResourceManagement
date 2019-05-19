#include "Functions.h"


int main(int argc, char** argv) {
	switch(argc) {						// Проверяем аргументы командной строки
		case 1:  printNonEmptySubfolders("./"); break;
		case 2:  printNonEmptySubfolders(argv[1]); break;
		default: fprintf(stderr, "Error: invalid count of command line items!\n");
	}

    return 0;
}
