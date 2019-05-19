#include "fileProcess.h"

#define ERROR -1

int main(int argc, char** argv) {
	Memory memory;
	uint32_t memory_size;

	memory_size = argc != 2 ? 64 : atoi(argv[1]);

	memoryInit(&memory, memory_size);

	puts("Implicit opening of stdin");
	int stdin_fd = mySTDOpen(&memory, STDIN_FILENO);
	if (stdin_fd == ERROR)
		fprintf(stderr, "Error: cannot open stdin!\n");
	printTables(&memory);

	puts("\nImplicit opening of stdout");
	int stdout_fd = mySTDOpen(&memory, STDOUT_FILENO);
	if (stdout_fd == ERROR)
		fprintf(stderr, "Error: cannot open stdout!\n");
	printTables(&memory);

	puts("\nImplicit opening of stderr");
	int stderr_fd = mySTDOpen(&memory, STDERR_FILENO);
	if (stderr_fd == ERROR)
		fprintf(stderr, "Error: cannot open stderr!\n");
	printTables(&memory);

	puts("\nOpening file.txt (fd1) for reading");
	int fd1 = myOpen(&memory, "file.txt", O_RDONLY);
	if (fd1 == ERROR)
		fprintf(stderr, "Error: cannot open file.txt!\n");
	printTables(&memory);

	puts("\nOpening empty.txt (fd2) for reading");
	int fd2 = myOpen(&memory, "empty.txt", O_RDONLY);
	if (fd2 == ERROR)
		fprintf(stderr, "Error: cannot open empty.txt!\n");
	printTables(&memory);

	puts("\nOpening file.txt (fd3) for writing");
	int fd3 = myOpen(&memory, "file.txt", O_WRONLY);
	if (fd3 == ERROR)
		fprintf(stderr, "Error: cannot open file.txt!\n");
	printTables(&memory);

	puts("\nOpening file.txt (fd4) for reading");
	int fd4 = myOpen(&memory, "file.txt", O_RDONLY);
	if (fd4 == ERROR)
		fprintf(stderr, "Error: cannot open file.txt!\n");
	printTables(&memory);

	puts("\nClosing std streams");
	mySTDClose(&memory, stdin_fd);
	mySTDClose(&memory, stdout_fd);
	mySTDClose(&memory, stderr_fd);
	printTables(&memory);

	puts("\nClosing file.txt (fd4) for reading");
	myClose(&memory, fd4);
	printTables(&memory);

	puts("\nOpening empty.txt (fd2) for reading");
	myClose(&memory, fd2);
	printTables(&memory);

	memoryFree(&memory);

	return 0;
}
