#include <task/task.hpp>
#include <thread/thread.hpp>

namespace thread {

// Static members
unsigned thread::num_threads = 0;
unsigned thread::num_free_threads = 0;

pthread_mutex_t thread::numthreads_mutex;
pthread_mutex_t thread::numfree_mutex;

void thread::work(task::tdq& task_queue)
{
  while (true) {
    auto td = task::fetch_task(task_queue);

    if (task::is_eow(td.get())) {
      return;
    }

    task::process(td.get());

    return;
  }
}

}
