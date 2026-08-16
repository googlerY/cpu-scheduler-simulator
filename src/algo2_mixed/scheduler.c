#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "scheduler.h"

// Q2 정렬 기준: 우선순위 오름차순, 동률이면 도착 순서
int cmp_q2(const void* a, const void* b) {
    Process* pa = &procs[*(int*)a];
    Process* pb = &procs[*(int*)b];
    if (pa->priority != pb->priority)
        return pa->priority - pb->priority;
    return pa->arrival - pb->arrival;
}

const char* get_queue_id(int qid) {
    static char buf[4];
    sprintf(buf, "Q%d", qid);
    return buf;
}

void input_process(void) {
    int type, pid, pri, burst;
    int arrival_time = 0;

    while (scanf("%d", &type) == 1) {
        if (type < 0) break;
        if (type == 0) {
            scanf("%d %d %d", &pid, &pri, &burst);
            Process* p = &procs[n++];
            p->pid = pid;
            p->priority = pri;
            p->burst = burst;
            p->rem = burst;
            p->arrival = arrival_time;
            p->finish = -1;
            p->qid = -1;
            p->rr_rem = 0;
        }
        else {
            // 시간 마커: 한 틱(QUANTUM)만큼 도착 시각을 진행시킴
            scanf("%*d %*d %*d");
            arrival_time += QUANTUM;
        }
    }
}

// Q0(SRTF) > Q1(RR) > Q2(비선점 우선순위) > Q3(FCFS) 순으로 우선순위를 두고 실행
void run_scheduler(void) {
    int curr_time = 0, completed = 0, current = -1;

    while (completed < n) {
        // 이번 시각에 도착한 프로세스를 burst/priority 기준으로 큐에 배정
        for (int i = 0; i < n; i++) {
            Process* p = &procs[i];
            if (p->qid == -1 && p->arrival == curr_time) {
                if (p->burst <= QUANTUM) { p->qid = 0; q0_push(i); }
                else if (p->priority >= -10 && p->priority <= 20) { p->qid = 1; p->rr_rem = QUANTUM; Q1[q1t++] = i; }
                else if (p->priority >= 21 && p->priority <= 30) { p->qid = 2; Q2[q2t++] = i; }
                else { p->qid = 3; p->priority = 31; Q3[q3t++] = i; }
            }
        }

        int next = -1;
        // Q0에 작업이 생기면 즉시 선점 (SRTF)
        if (q0h > 0) {
            if (current >= 0 && procs[current].qid != 0) {
                int old = current;
                if (procs[old].qid == 1) {
                    // RR 도중 선점당하면 남은 퀀텀을 버리고 페널티만 적용해 재분류
                    procs[old].priority += QUANTUM;
                    if (procs[old].priority > 31) procs[old].priority = 31;
                    if (procs[old].priority >= 21 && procs[old].priority <= 30) { procs[old].qid = 2; Q2[q2t++] = old; }
                    else if (procs[old].priority > 30) { procs[old].qid = 3; Q3[q3t++] = old; }
                    else { procs[old].qid = 1; procs[old].rr_rem = QUANTUM; Q1[q1t++] = old; }
                }
                else if (procs[old].qid == 2) { Q2[q2t++] = old; }
                else { Q3[q3t++] = old; }
                current = -1;
            }
            next = q0_pop();
        }
        // Q1: RR (이미 실행 중이면 남은 퀀텀 계속 소진)
        else if (current >= 0 && procs[current].qid == 1 && procs[current].rr_rem > 0) next = current;
        else if (q1h < q1t) next = Q1[q1h++];
        // Q2: 비선점 우선순위 (실행 중이면 끝까지 보장)
        else if (current >= 0 && procs[current].qid == 2) next = current;
        else if (q2h < q2t) {
            int best = q2h;
            for (int i = q2h + 1; i < q2t; i++) {
                int id = Q2[i], bid = Q2[best];
                if (procs[id].priority < procs[bid].priority ||
                    (procs[id].priority == procs[bid].priority && procs[id].arrival < procs[bid].arrival)) best = i;
            }
            next = Q2[best];
            for (int i = best; i < q2t - 1; i++) Q2[i] = Q2[i + 1];
            q2t--;
        }
        // Q3: FCFS
        else if (current >= 0 && procs[current].qid == 3) next = current;
        else if (q3h < q3t) next = Q3[q3h++];

        if (next < 0) { curr_time++; continue; }

        current = next;
        int run = (procs[current].qid == 1 ? 1 : procs[current].rem);
        procs[current].rem -= run;
        curr_time += run;
        if (procs[current].qid == 1) procs[current].rr_rem -= run;

        if (procs[current].rem == 0) {
            procs[current].finish = curr_time;
            completed++;
            current = -1;
        }
        else if (procs[current].qid == 1 && procs[current].rr_rem == 0) {
            // 퀀텀 소진: 페널티를 부여하고 새 우선순위에 맞는 큐로 재분류
            procs[current].priority += QUANTUM;
            if (procs[current].priority > 31) procs[current].priority = 31;
            if (procs[current].priority >= 21 && procs[current].priority <= 30) { procs[current].qid = 2; Q2[q2t++] = current; }
            else if (procs[current].priority > 30) { procs[current].qid = 3; Q3[q3t++] = current; }
            else { procs[current].qid = 1; procs[current].rr_rem = QUANTUM; Q1[q1t++] = current; }
            current = -1;
        }
        else if (procs[current].qid == 0) {
            q0_push(current);
            current = -1;
        }
    }
}

// Q0 -> Q1 -> Q2(우선순위) -> Q3 순으로 그룹핑해서 출력
void show_output(void) {
    int tt;
    double ntt, sum_ntt = 0;

    printf("%-8s %-6s %-8s %-8s %-8s %-10s\n",
        "PID", "Queue", "Priority", "Burst", "TT", "Normalized");

    for (int q = 0; q <= 3; q++) {
        if (q < 2) {
            for (int i = 0; i < n; i++) {
                Process* p = &procs[i];
                if (p->qid == q) {
                    tt = p->finish - p->arrival;
                    ntt = (double)tt / p->burst;
                    sum_ntt += ntt;
                    printf("%-8d %-6s %-8d %-8d %-8d %-10.2f\n",
                        p->pid, get_queue_id(q), p->priority, p->burst, tt, ntt);
                }
            }
        }
        else if (q == 2) {
            int idxs[MAXP], cnt = 0;
            for (int i = 0; i < n; i++) if (procs[i].qid == 2) idxs[cnt++] = i;
            qsort(idxs, cnt, sizeof(int), cmp_q2);
            for (int j = 0; j < cnt; j++) {
                Process* p = &procs[idxs[j]];
                tt = p->finish - p->arrival;
                ntt = (double)tt / p->burst;
                sum_ntt += ntt;
                printf("%-8d %-6s %-8d %-8d %-8d %-10.2f\n",
                    p->pid, get_queue_id(q), p->priority, p->burst, tt, ntt);
            }
        }
        else {
            for (int i = 0; i < n; i++) {
                Process* p = &procs[i];
                if (p->qid == 3) {
                    tt = p->finish - p->arrival;
                    ntt = (double)tt / p->burst;
                    sum_ntt += ntt;
                    printf("%-8d %-6s %-8d %-8d %-8d %-10.2f\n",
                        p->pid, get_queue_id(q), p->priority, p->burst, tt, ntt);
                }
            }
        }
    }
    printf("Average Normalized TT: %.2f\n", sum_ntt / n);
}