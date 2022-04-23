#include <task/task.hpp>
#include <thread/thread.hpp>

namespace thread {

void thread::work(task::task_descr_t* td)
{
  while (true) {
    if (task::is_eow(td)) {
      return;
    }

    task::process(td);

    return;
  }
}

}
