#include "queue.h"

Process procs[MAXP];
int n = 0;

int Q0[MAXP], q0h = 0;
int Q1[MAXP], q1h = 0, q1t = 0;
int Q2[MAXP], q2h = 0, q2t = 0;
int Q3[MAXP], q3h = 0, q3t = 0;

// 잔여시간(rem) 기준 최소 힙에 삽입
void q0_push(int idx) {
    int i = q0h++;
    Q0[i] = idx;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (procs[Q0[p]].rem <= procs[Q0[i]].rem) break;
        int tmp = Q0[p]; Q0[p] = Q0[i]; Q0[i] = tmp;
        i = p;
    }
}

// 잔여시간이 가장 적은 프로세스를 꺼냄 (SRTF)
int q0_pop(void) {
    if (q0h == 0) return -1;
    int ret = Q0[0];
    Q0[0] = Q0[--q0h];
    int i = 0;
    while (1) {
        int l = 2 * i + 1, r = 2 * i + 2, m = i;
        if (l < q0h && procs[Q0[l]].rem < procs[Q0[m]].rem) m = l;
        if (r < q0h && procs[Q0[r]].rem < procs[Q0[m]].rem) m = r;
        if (m == i) break;
        int tmp = Q0[i]; Q0[i] = Q0[m]; Q0[m] = tmp;
        i = m;
    }
    return ret;
}
