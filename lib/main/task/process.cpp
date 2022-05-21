#include <cstdio>
#include <time.h>

#include <task/process.hpp>

namespace task {

//===---------------------------------===
// DO NOT TOUCH __ DEFINED BY PROFESSOR
//===---------------------------------===
void process(task_descr_t* td)
{
    struct timespec zzz;

    zzz.tv_sec  = td->ms/1000;
    zzz.tv_nsec = (td->ms%1000) * 1000000L;

    printf("IP #%d\n", td->pid);
    nanosleep(&zzz,NULL);
    printf("FP #%d\n", td->pid);
}

}
