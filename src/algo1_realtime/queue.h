#ifndef QUEUE_H
#define QUEUE_H

// PCB 구조체
typedef struct _P {
    int pid;
    int priority;
    int computing_time;
    int remaining_time;
    int arrival_time;
    int end_time;
} P;

// 이중 연결 리스트 노드
typedef struct _node {
    P* process;
    struct _node* prev;
    struct _node* next;
} node;

// 큐 구조체
typedef struct _queue {
    node* head;
    node* tail;
} queue;

void initQueue(queue* q);
P* dequeue(queue* q);
void enqueue(queue* q, P* p);
void enqueue_by_priority(queue* q, P* p);

#endif