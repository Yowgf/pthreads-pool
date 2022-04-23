#ifndef TASK_PROCESS_H
#define TASK_PROCESS_H

//===---------------------===
// DEFINED BY PROFESSOR
//===---------------------===

#include <time.h>

namespace task {

class task_descr_t {
public:
  task_descr_t(int pid, int ms) :
    pid(pid), ms(ms)
  {}

  int pid;
  int ms;
};

void process(task_descr_t* td);

}

#endif
