#ifndef THREAD_THREAD_H
#define THREAD_THREAD_H

#include <pthread.h>
#include <vector>

#include <task/task.hpp>

namespace thread {

class thread {
public:
  static unsigned num_free_threads;
  static pthread_mutex_t num_free_threads_mutex;

  thread(int tid, int min_threads) :
    tid(tid), min_threads(min_threads)
  {
    pthread_mutex_lock(&num_free_threads_mutex);
    num_free_threads++;
    pthread_mutex_unlock(&num_free_threads_mutex);
  }

  void signal_free_thread() {
    pthread_mutex_lock(&num_free_threads_mutex);
    num_free_threads++;
    pthread_mutex_unlock(&num_free_threads_mutex);
  }

  void signal_busy_thread() {
    pthread_mutex_lock(&num_free_threads_mutex);
    num_free_threads--;
    pthread_mutex_unlock(&num_free_threads_mutex);
  }

  void parallel_work_quit()
  {
    printf("TE %d\n", tid);
    signal_free_thread();
    pthread_exit(nullptr);
  }

  void parallel_work(void* vdargs)
  {
    printf("TB %d\n", tid);

    while (true) {
      // Fetch task
      auto td = task::consume_task();

      // Check if should quit before processing.
      if (task::is_eow(td.get())) {
	break;
      }

      // Process task
      task::process(td.get());

      // If there are more threads waiting for a task than the minimum number of
      // threads, this thread should not wait. It should quit.
      if (task::get_num_consumer_threads_waiting() >= this->min_threads) {
	break;
      }
    }

    parallel_work_quit();
  }

  void work()
  {
    signal_busy_thread();

    // TODO: add pthread fork call here
    parallel_work(nullptr);
  }

private:
  const int tid;
  const size_t min_threads;
};

}

#endif
