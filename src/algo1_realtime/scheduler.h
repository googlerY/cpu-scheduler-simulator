#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "queue.h"

#define PROCESS_LIST_SIZE 1024
#define TIME_QUANTUM 20

void input_process();
void scheduler();
void show_output();

#endif