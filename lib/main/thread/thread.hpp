#ifndef THREAD_THREAD_H
#define THREAD_THREAD_H

#include <pthread.h>
#include <vector>

#include <task/task.hpp>

namespace thread {

class thread;

struct work_args {
  std::unique_ptr<thread> self;
  task::tdq& task_queue;
};

class thread {
public:
  static unsigned num_threads;
  static std::vector<std::unique_ptr<thread>> free_threads;

  // Mutexes and condition variables are initialized externally by pool of
  // threads, _exactly once_.
  static pthread_mutex_t numthreads_mutex;
  static pthread_mutex_t freethreads_mutex;

  thread(int tid, int min_threads) :
    tid(tid), min_threads(min_threads)
  {}

  static void create(int tid, int min_threads)
  {
    pthread_mutex_lock(&numthreads_mutex);
    num_threads++;
    pthread_mutex_unlock(&numthreads_mutex);

    auto new_thread = std::make_unique<thread>(tid, min_threads);
    free_threads[tid] = std::move(new_thread);
  }

  void parallel_work_quit()
  {
    printf("TE %d\n", tid);
    pthread_mutex_lock(&numthreads_mutex);
    num_threads++;
    pthread_mutex_unlock(&numthreads_mutex);
    free_threads[tid] = nullptr;
    pthread_exit(nullptr);
  }

  void parallel_work(void* vdargs)
  {
    auto args = reinterpret_cast<work_args*>(vdargs);
    auto self{std::move(args->self)};
    auto tq{args->task_queue};

    printf("TB %d\n", tid);

    while (true) {
      // Fetch task
      auto td = task::fetch_task(tq);

      // Check if should quit
      if (task::is_eow(td.get())) {
	break;
      }

      // Process task
      task::process(td.get());

      // Check if should quit
      if (free_threads.size() >= this->min_threads) {
	free_threads[tid] = std::move(self);
      } else {
	break;
      }
    }

    parallel_work_quit();
  }

  void work(task::tdq& task_queue)
  {
    // Indicate that the thread is no longer free
    auto self = free_threads[tid];
    free_threads[tid] = nullptr;

    parallel_work((void*) new work_args{std::move(self), task_queue});
  }

private:
  const int tid;
  const int min_threads;
};

}

#endif
