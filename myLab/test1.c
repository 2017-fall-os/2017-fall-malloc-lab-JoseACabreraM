#include "stdio.h"
#include "stdlib.h"
#include "myAllocator.h"
#include "sys/time.h"
#include <sys/resource.h>
#include <unistd.h>

double diffTimeval1(struct timeval *t1, struct timeval *t2) {
    double d = (t1->tv_sec - t2->tv_sec) + (1.0e-6 * (t1->tv_usec - t2->tv_usec));
    return d;
}

void getutime1(struct timeval *t) {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    *t = usage.ru_utime;
}

int main() {
    void *p1, *p2, *p3;
    arenaCheck();
    p1 = malloc(254);
    arenaCheck();
    p2 = malloc(25400);
    arenaCheck();
    p3 = malloc(254);
    printf("%8zx %8zx %8zx\n", p1, p2, p3);
    arenaCheck();
    free(p2);
    arenaCheck();
    free(p3);
    arenaCheck();
    free(p1);
    arenaCheck();
    {                /* measure time for 10000 mallocs */
        struct timeval t1, t2;
        int i;
        getutime1(&t1);
        for (i = 0; i < 100000; i++)
            if (malloc(4) == 0)
                break;
        getutime1(&t2);
        printf("%d malloc(4) required %f seconds\n", i, diffTimeval1(&t2, &t1));
    }
    return 0;
}
