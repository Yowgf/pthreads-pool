#ifndef POOL_PROCESS_H
#define POOL_PROCESS_H

//===---------------------===
// DEFINED BY PROFESSOR
//===---------------------===

#include <time.h>

namespace pool {

class task_descr_t {
public:
  task_descr_t(int pid, int ms) : pid(pid), ms(ms) {}

  int pid;
  int ms;
};

void processa(task_descr_t* tp)
{
    struct timespec zzz;

    zzz.tv_sec = 0;
    zzz.tv_nsec = tp->ms * 1000000L; // 0 <= ms <= 999

    printf("IP #%d\n", tp->pid);
    nanosleep(&zzz,NULL);
    printf("FP #%d\n", tp->pid);
}

}

#endif
