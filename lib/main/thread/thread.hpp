#ifndef THREAD_THREAD_H
#define THREAD_THREAD_H

#include <task/task.hpp>

namespace thread {

class thread {
public:
  void work(task::task_descr_t* td);
};

}

#endif
