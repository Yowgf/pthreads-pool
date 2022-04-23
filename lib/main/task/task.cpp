#include <task/task.hpp>

namespace task {

std::unique_ptr<task_descr_t> fetch_task(std::istream& is)
{
  int pid = 0;
  int ms = 0;
  is >> pid >> ms;
  return std::make_unique<task_descr_t>(pid, ms);
}

bool is_eow(task_descr_t* td)
{
  return td->pid == 0 && td->ms == 0;
}

}
