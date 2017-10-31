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
  
  void *p1, *p2, *p3, *p4;
    arenaCheck();
    p1 = malloc(56500);
    p2 = malloc(254);
    p3 = malloc(25400);
    p4 = malloc(254);
    arenaCheck();
    /*
      Old resize region will create a new region of size 1024, copy the contents of
      p4 into the new region and unallocate the old region.
    */
    p4 = oldResizeRegion(p4,1024);
    arenaCheck();
    freeRegion(p4);

    /* 
       New region resizer will take out a chunk out of p4 successor's block in order to 
       fulfill the reallocation request, instead of creating a new block. 
    */
    p4 = malloc(254);
    arenaCheck();
    p4 = realloc(p4,1024);
    arenaCheck();

    // Free p1 and p3 to have some free chunks in the middle for allocator testing 
    freeRegion(p1);
    arenaCheck();
    freeRegion(p3);
    arenaCheck();
    
    /* 
       Will allocate p3 with first fit, taking it out of the first free block it finds,
       in this case it will take 16000 out of the 56504 block left behind by p1. It then
       frees p3 again for re-testing with best fit. 
    */
    p3 = firstFitAllocRegion(16000);
    arenaCheck();
    freeRegion(p3);
    arenaCheck();

    /* 
       Will allocate p3 with best fit, in this case figuring out that it's better to take
       out a chunk out of the 25400 free block instead of the 56504 block since it better
       fits the request for 16000 bytes. 
    */
    p3 = malloc(16000);
    arenaCheck();
    freeRegion(p3);
    arenaCheck();

    /*
      Uses my extra reallocator function, which will try to merge a block with both its
      successor and predecessor in order to fulfill a reallocation request. In this case, 
      p2, of size 256 bytes,  will be reallocated to 58000 but it can't fulfill the request
      by just merging with either its successor, which has 25400 bytes available, or its 
      predecessor, which has 56504 bytes available. But it can fulfill the request by fully
      coalescing with its successor and then take out the remaining part from its predecessor.
    */
    p2 = resizeRegionExtra(p2, 58000);
    arenaCheck();

    freeRegion(p2);
    freeRegion(p4);

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
