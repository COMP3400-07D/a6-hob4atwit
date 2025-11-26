#include "parta.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * TODO: Describe what the function does
 */
struct pcb* init_procs(int* bursts, int blen) {
    if (blen <= 0) return NULL;
    struct pcb* procs = malloc(sizeof(struct pcb) * blen);
    if (!procs) return NULL;
    for (int i = 0; i < blen; ++i) {
        procs[i].pid = i;
        procs[i].burst_left = bursts[i];
        procs[i].wait = 0;
    }
    return procs;
}

/**
 * TODO: Describe what the function does
 */
void printall(struct pcb* procs, int plen) {
    if (!procs) return;
    for (int i = 0; i < plen; ++i) {
        printf("P%d: burst_left=%d wait=%d\n", procs[i].pid, procs[i].burst_left, procs[i].wait);
    }
}

/**
 * TODO: Describe what the function does
 */
void run_proc(struct pcb* procs, int plen, int current, int amount) {
    if (!procs) return;
    if (current < 0 || current >= plen) return;

    int avail = procs[current].burst_left;
    if (avail <= 0) return; // nothing to run

    int actual = amount;
    if (actual > avail) actual = avail;

    /* reduce current process burst */
    procs[current].burst_left -= actual;
    if (procs[current].burst_left < 0) procs[current].burst_left = 0;

    /* increase wait for other active processes */
    for (int i = 0; i < plen; ++i) {
        if (i == current) continue;
        if (procs[i].burst_left > 0) {
            procs[i].wait += actual;
        }
    }
}

/**
 * TODO: Describe what the function does
 */
int fcfs_run(struct pcb* procs, int plen) {
    if (!procs) return 0;
    int time = 0;
    for (int i = 0; i < plen; ++i) {
        if (procs[i].burst_left <= 0) continue;
        int amount = procs[i].burst_left;
        run_proc(procs, plen, i, amount);
        time += amount;
    }
    return time;
}

/**
 * TODO: Describe what the function does
 */
int rr_next(int current, struct pcb* procs, int plen) {
    if (!procs || plen <= 0) return -1;

    /* Search forward from current+1 for any active process */
    for (int offset = 1; offset < plen; ++offset) {
        int idx = (current + offset) % plen;
        if (procs[idx].burst_left > 0) return idx;
    }

    /* If none found, but current is still active, return current */
    if (current >= 0 && current < plen && procs[current].burst_left > 0) return current;

    /* All done */
    return -1;
}
/**
 * TODO: Describe what the function does
 */
int rr_run(struct pcb* procs, int plen, int quantum) {
    if (!procs || plen <= 0 || quantum <= 0) return 0;

    int time = 0;

    /* pick first process to start: first with burst_left > 0 */
    int prev = -1;
    int start = -1;
    for (int i = 0; i < plen; ++i) {
        if (procs[i].burst_left > 0) { start = i; break; }
    }
    if (start == -1) return 0; /* nothing to run */

    /* run the first chosen process, then continue round-robin */
    int idx = start;
    while (1) {
        if (idx == -1) break;

        int amount = procs[idx].burst_left;
        if (amount > quantum) amount = quantum;

    /* DEBUG: show scheduling steps */
    fprintf(stderr, "RR run idx=%d amount=%d before: ", idx, amount);
    for (int k=0;k<plen;++k) fprintf(stderr, "p%d(b=%d,w=%d) ", k, procs[k].burst_left, procs[k].wait);
    fprintf(stderr, "\n");

        run_proc(procs, plen, idx, amount);
        time += amount;

        /* pick next after idx */
        idx = rr_next(idx, procs, plen);
    }

    return time;
}

