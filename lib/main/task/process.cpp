#include <cstdio>

#include <task/process.hpp>

namespace task {

void process(task_descr_t* td)
{
    struct timespec zzz;

    zzz.tv_sec = 0;
    zzz.tv_nsec = td->ms * 1000000L; // 0 <= ms <= 999

    printf("IP #%d\n", td->pid);
    nanosleep(&zzz,NULL);
    printf("FP #%d\n", td->pid);
}

}
