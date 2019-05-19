#ifndef PROCESSLAUNCHER_H
#define PROCESSLAUNCHER_H

#include <stdint.h>

// Поддерживать в активном состоянии указанное число процессов из общего кол-ва
void start(uint8_t proc_count, uint8_t active_proc_count);

#endif
