#ifndef THREAD_THREAD_H
#define THREAD_THREAD_H

#include <pthread.h>
#include <vector>

#include <task/task.hpp>

namespace thread {

using work_args = struct {
  const int tid;
  const size_t min_threads;
  const size_t max_threads;
};

class thread {
public:
  static unsigned        num_free_threads;
  static pthread_mutex_t num_free_threads_mutex;
  static pthread_cond_t  all_threads_free;

  thread(int tid, size_t min_threads, size_t max_threads) :
    tid(tid), min_threads(min_threads), max_threads(max_threads)
  {
    // Initialize pthreads objects
    pthread_attr_init(&my_thread_attrs);
    pthread_attr_setdetachstate(&my_thread_attrs, PTHREAD_CREATE_DETACHED);

    signal_free_thread(tid, max_threads);
  }

  static void signal_free_thread(int tid, size_t max_threads) {
    pthread_mutex_lock(&num_free_threads_mutex);
    num_free_threads++;
    if (num_free_threads >= max_threads) {
      pthread_cond_signal(&all_threads_free);
    }
    pthread_mutex_unlock(&num_free_threads_mutex);
  }

  static void signal_busy_thread(int tid) {
    pthread_mutex_lock(&num_free_threads_mutex);
    num_free_threads--;
    pthread_mutex_unlock(&num_free_threads_mutex);
  }

  static void parallel_work_quit(int tid, size_t max_threads)
  {
    printf("TE %d\n", tid);
    signal_free_thread(tid, max_threads);
    pthread_exit(nullptr);
  }

  static void* parallel_work(void* vdargs)
  {
    auto args = reinterpret_cast<work_args*>(vdargs);
    auto tid = args->tid;
    auto min_threads = args->min_threads;
    auto max_threads = args->max_threads;
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
      if (task::get_num_consumer_threads_waiting() >= min_threads) {
	break;
      }
    }

    parallel_work_quit(tid, max_threads);

    return nullptr;
  }

  void work()
  {
    signal_busy_thread(tid);

    pthread_create(&my_thread, &my_thread_attrs, &parallel_work,
		   static_cast<void*>(new work_args{tid, min_threads, max_threads}));
  }

private:
  const int tid;
  const size_t min_threads;
  const size_t max_threads;

  pthread_attr_t my_thread_attrs;
  pthread_t my_thread;
};

}

#endif
