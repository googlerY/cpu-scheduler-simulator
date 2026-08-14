#include <stdio.h>
#include "scheduler.h"

queue queue_array[5];
queue process_end;
P process_input[PROCESS_LIST_SIZE];
int process_count = 0;

void select_queue(P* new_process) {
    int priority = new_process->priority;
    if (-10 <= priority && priority <= -1) {
        enqueue(&queue_array[0], new_process);
    } else if (1 <= priority && priority <= 10) {
        enqueue_by_priority(&queue_array[1], new_process);
    } else if (11 <= priority && priority <= 20) {
        enqueue_by_priority(&queue_array[2], new_process);
    } else if (21 <= priority && priority <= 30) {
        enqueue_by_priority(&queue_array[3], new_process);
    } else {
        enqueue(&queue_array[4], new_process);
    }
}

void input_process() {
    int type, process_id, priority, computing_time;
    int arrival_time = 0;
    
    while (1) {
        scanf("%d", &type);
        if (type == -1) break;
        
        scanf("%d %d %d", &process_id, &priority, &computing_time);
        
        if (type == 1) {
            arrival_time += 20;
        } else {
            P new_process;
            new_process.pid = process_id;
            new_process.priority = priority;
            new_process.computing_time = computing_time;
            new_process.remaining_time = computing_time;
            new_process.arrival_time = arrival_time;
            
            process_input[process_count++] = new_process;
        }
    }
}

void scheduler() {
    int process_completed = 0;
    int curr_time = 0;
    int i=0;
    
    while (process_completed < process_count) {
        // 새롭게 시스템에 도착한 프로세스 큐에 배정
        for (; i < process_count; i++) {
            if (process_input[i].arrival_time <= curr_time)
                select_queue(&process_input[i]);
            else
                break;
        }
        
        int queue_index;
        P* running_process = NULL;
        
        // Q0부터 순차 탐색
        for (queue_index = 0; queue_index < 5; queue_index++) {
            running_process = dequeue(&queue_array[queue_index]);
            if (running_process != NULL) break;
        }
        
        // 대기 중인 프로세스가 없으면 시간 1 증가 후 스킵
        if (running_process == NULL) {
            curr_time++;
            continue;
        }
        
        // Q0: 실시간 큐 (비선점, 보장 실행)
        if (queue_index == 0) {
            curr_time += running_process->remaining_time;
            running_process->remaining_time = 0;
            running_process->end_time = curr_time;
            enqueue(&process_end, running_process);
            process_completed++;
        } 
        // Q1 ~ Q3: 우선순위 기반 선점 큐 (타임퀀텀 제한, 패널티 적용)
        else if (0 < queue_index && queue_index < 4) {
            if (running_process->remaining_time > TIME_QUANTUM) {
                running_process->priority += 10;
                if (running_process->priority > 31) {
                    running_process->priority = 31;
                }
                curr_time += TIME_QUANTUM;
                running_process->remaining_time -= TIME_QUANTUM;
                select_queue(running_process); // 갱신된 우선순위로 다시 배정
            } else {
                curr_time += running_process->remaining_time;
                running_process->remaining_time = 0;
                running_process->end_time = curr_time;
                process_completed++;
                enqueue(&process_end, running_process);
            }
        } 
        // Q4: RR 방식 (패널티 없이 재삽입)
        else {
            if (running_process->remaining_time > TIME_QUANTUM) {
                curr_time += TIME_QUANTUM;
                running_process->remaining_time -= TIME_QUANTUM;
                enqueue(&queue_array[4], running_process); 
            } else {
                curr_time += running_process->remaining_time;
                running_process->remaining_time = 0;
                running_process->end_time = curr_time;
                process_completed++;
                enqueue(&process_end, running_process);
            }
        }
    }
}

const char* get_queue_id(int priority) {
    if (-10 <= priority && priority <= -1) return "real_time";
    else if (1 <= priority && priority <= 10) return "Q1";
    else if (11 <= priority && priority <= 20) return "Q2";
    else if (21 <= priority && priority <= 30) return "Q3";
    else return "Q4";
}

void show_output() {
    P* process;
    int turn_around_time, service_time;
    double sum_normalized_turn_around_time = 0;
    
    printf("Process_id\tQueue_id\tpriority\tcomputing_time\tturn_around_time\n");
    
    while ((process = dequeue(&process_end)) != NULL) {
        turn_around_time = process->end_time - process->arrival_time;
        service_time = process->computing_time;
        sum_normalized_turn_around_time += (double)turn_around_time / service_time;
        
        printf("%d\t\t%s\t\t%d\t\t%d\t\t%d\n", 
               process->pid, 
               get_queue_id(process->priority), 
               process->priority, 
               process->computing_time, 
               turn_around_time);
    }
    
    printf("Normalized average turn_around time: %f\n", 
           sum_normalized_turn_around_time / process_count);
}
