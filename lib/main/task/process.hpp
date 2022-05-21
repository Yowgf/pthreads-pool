#ifndef TASK_PROCESS_H
#define TASK_PROCESS_H

#include <iostream>

namespace task {

class task_descr_t {
public:
  task_descr_t(int pid, int ms) :
    pid(pid), ms(ms)
  {}

  static task_descr_t* create_eow()
  {
    return new task_descr_t{0, 0};
  }

  static task_descr_t* create_from_stream(std::istream& is)
  {
    int pid = 0;
    int ms = 0;

    // We indicate end of stream by returning a null pointer.
    if (!(is >> pid)) return nullptr;
    if (!(is >> ms)) return nullptr;

    return new task_descr_t{pid, ms};
  }

  int pid;
  int ms;
};

//===---------------------===
// DEFINED BY PROFESSOR
//===---------------------===
void process(task_descr_t* td);

}

#endif
