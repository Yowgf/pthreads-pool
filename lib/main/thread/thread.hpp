#ifndef THREAD_THREAD_H
#define THREAD_THREAD_H

#include <pthread.h>
#include <set>

#include <task/task.hpp>
// #include <utils/log.hpp>

namespace thread {

struct work_args {
  const int tid;
  const size_t min_threads;
  const size_t max_threads;
};

class thread {
public:
  static std::set<int>   free_threads;
  static pthread_mutex_t free_threads_mutex;
  static pthread_cond_t  all_threads_free;

  static void wait_all_threads_done(size_t max_threads) {
    pthread_mutex_lock(&free_threads_mutex);
    while (free_threads.size() < max_threads) {
      pthread_cond_wait(&all_threads_free, &free_threads_mutex);
    }
    pthread_mutex_unlock(&free_threads_mutex);
  }

  static void signal_free_thread(int tid, size_t max_threads) {
    pthread_mutex_lock(&free_threads_mutex);
    free_threads.insert(tid);
    if (free_threads.size() >= max_threads) {
      pthread_cond_signal(&all_threads_free);
    }
    pthread_mutex_unlock(&free_threads_mutex);
  }

  static void signal_busy_thread(int tid) {
    pthread_mutex_lock(&free_threads_mutex);
    free_threads.erase(tid);
    pthread_mutex_unlock(&free_threads_mutex);
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

  thread(int tid, size_t min_threads, size_t max_threads) :
    tid(tid), min_threads(min_threads), max_threads(max_threads)
  {
    // Initialize pthreads objects
    pthread_attr_init(&my_thread_attrs);
    pthread_attr_setdetachstate(&my_thread_attrs, PTHREAD_CREATE_DETACHED);

    pthread_mutex_lock(&free_threads_mutex);
    free_threads.insert(tid);
    pthread_mutex_unlock(&free_threads_mutex);
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
