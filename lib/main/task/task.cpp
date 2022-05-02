#include <task/task.hpp>

namespace task {

std::unique_ptr<task_descr_t> fetch_task(task::tdq& task_queue)
{
  // TODO: mutual exclusion with mutex and condition var


  auto td_ptr = task_queue.front(); task_queue.pop();

  return std::unique_ptr<task_descr_t>(td_ptr);
}

bool is_eow(task_descr_t* td)
{
  return td->pid == 0 && td->ms == 0;
}

}
