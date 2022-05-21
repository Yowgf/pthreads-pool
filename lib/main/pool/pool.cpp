#include <iostream>
#include <pthread.h>

#include <pool/pool.hpp>
#include <utils/log.hpp>

namespace pool {

pool::pool(const utils::parsed_args& config)
{
  this->debug = config.debug;
  LOG(this->debug, "Debug mode is activated.");

  this->min_threads = static_cast<size_t>(config.min_threads);
  this->max_threads = static_cast<size_t>(config.max_threads);
  this->num_active_threads = 0;
}

pool::~pool()
{
  pthread_cond_destroy(&task::TQ::NOT_EMPTY);
  pthread_cond_destroy(&task::TQ::NOT_FULL);
}

void pool::init()
{
  LOG(this->debug, "Initializing pool of threads.");

  // Initialize mutexes and condition variables.
  pthread_mutex_init(&thread::thread::num_free_threads_mutex, nullptr);
  pthread_mutex_init(&task::TQ::MUTEX, nullptr);
  pthread_cond_init(&task::TQ::NOT_EMPTY, nullptr);
  pthread_cond_init(&task::TQ::NOT_FULL, nullptr);

  task::TQ::MAX_SIZE = 0xFFFF;

  // Initialize thread objects. The threads are not dispatched immediately.
  for (decltype(max_threads) i = 0; i < max_threads; ++i) {
    this->threads.push_back(thread::thread(i, min_threads, max_threads));
  }
  // Dispatch minimum number of threads. They should stay alive waiting for
  // tasks to be consumed.
  for (decltype(min_threads) i = 0; i < min_threads; ++i) {
    this->threads.at(i).work();
  }

  LOG(this->debug, "Successfully initialized pool of threads.");
}

// end fills task queue with EOW (end of work) task descriptors.
void pool::end(size_t max_threads)
{
  LOG(this->debug, "Ending pool of threads.");

  for (decltype(max_threads) i = 0; i < max_threads; ++i) {
    auto eow = task::task_descr_t::create_eow();
    task::produce_task(eow);
  }

  // Wait until all threads are finished.
  pthread_mutex_lock(&thread::thread::num_free_threads_mutex);
  LOG(this->debug, "Waiting for threads to finish.");
  while (thread::thread::num_free_threads < this->max_threads) {
    pthread_cond_wait(&thread::thread::all_threads_free,
		      &thread::thread::num_free_threads_mutex);
  }
  pthread_mutex_unlock(&thread::thread::num_free_threads_mutex);

  LOG(this->debug, "Successfully ended pool of threads.");
}

// - Only master thread pushes tasks to the task queue
//
// - There has to be a global condition variable the worker threads
void pool::run()
{
  while (true) {
    // Read task descriptor from input stream and dispatch the task.
    auto new_td = task::task_descr_t::create_from_stream(std::cin);
    if (new_td == nullptr) {
      break;
    }
    LOG(this->debug, "Producing task with pid=%d ms=%d",
	new_td->pid, new_td->ms);
    task::produce_task(new_td);

    // Create threads if necessary
    
  }

  this->end(this->max_threads);
}

}
