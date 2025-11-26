#include "parta.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("ERROR: Missing arguments\n");
        return 1;
    }

    if (strcmp(argv[1], "fcfs") == 0) {
        /* Need at least one burst */
        if (argc < 3) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }

        int plen = argc - 2;
        int* bursts = malloc(sizeof(int) * plen);
        if (!bursts) return 1;
        for (int i = 0; i < plen; ++i) bursts[i] = atoi(argv[2 + i]);

        printf("Using FCFS\n\n");
        for (int i = 0; i < plen; ++i) {
            printf("Accepted P%d: Burst %d\n", i, bursts[i]);
        }

        struct pcb* procs = init_procs(bursts, plen);
        int total = fcfs_run(procs, plen);

        int sumwait = 0;
        for (int i = 0; i < plen; ++i) sumwait += procs[i].wait;
        double avg = ((double)sumwait) / (double)plen;
        printf("Average wait time: %.2f\n", avg);

        free(bursts);
        free(procs);
        return 0;

    } else if (strcmp(argv[1], "rr") == 0) {
        /* Need quantum and at least one burst */
        if (argc < 4) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }

        int quantum = atoi(argv[2]);
        int plen = argc - 3;
        int* bursts = malloc(sizeof(int) * plen);
        if (!bursts) return 1;
        for (int i = 0; i < plen; ++i) bursts[i] = atoi(argv[3 + i]);

        printf("Using RR(%d).\n\n", quantum);
        for (int i = 0; i < plen; ++i) {
            printf("Accepted P%d: Burst %d\n", i, bursts[i]);
        }

        struct pcb* procs = init_procs(bursts, plen);
        int total = rr_run(procs, plen, quantum);

        int sumwait = 0;
        for (int i = 0; i < plen; ++i) sumwait += procs[i].wait;
        double avg = ((double)sumwait) / (double)plen;
        printf("Average wait time: %.2f\n", avg);

        free(bursts);
        free(procs);
        (void)total; /* quiet unused var if needed */
        return 0;

    } else {
        printf("ERROR: Missing arguments\n");
        return 1;
    }
}
