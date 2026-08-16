#ifndef QUEUE_H
#define QUEUE_H

#define MAXP 20

typedef struct {
    int pid;
    int priority;
    int burst;
    int rem;
    int arrival;
    int finish;
    int qid;
    int rr_rem;
} Process;

extern Process procs[MAXP];
extern int n;

// Q0: SRTF용 최소 힙 (잔여시간 기준)
extern int Q0[MAXP], q0h;
void q0_push(int idx);
int q0_pop(void);

// Q1: Round Robin, Q2: 비선점 우선순위, Q3: FCFS (배열 기반 FIFO)
extern int Q1[MAXP], q1h, q1t;
extern int Q2[MAXP], q2h, q2t;
extern int Q3[MAXP], q3h, q3t;

#endif
