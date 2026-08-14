#include <stdio.h>
#include "scheduler.h"

extern queue queue_array[5];
extern queue process_end;

int main() {
    for (int i = 0; i < 5; i++) {
        initQueue(&queue_array[i]);
    }
    initQueue(&process_end);
    
    input_process();
    scheduler();
    show_output();
    
    return 0;
}
