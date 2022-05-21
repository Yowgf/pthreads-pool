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

  for (decltype(this->max_threads) i = 0; i < this->max_threads; ++i) {
    this->threads.push_back(thread::thread(i, this->min_threads));
  }
}

pool::~pool()
{
  pthread_cond_destroy(&task::TQ::NOT_EMPTY);
  pthread_cond_destroy(&task::TQ::NOT_FULL);
}

void pool::init()
{
  // Initialize mutexes and condition variables.
  pthread_mutex_init(&thread::thread::num_free_threads_mutex, nullptr);
  pthread_mutex_init(&task::TQ::MUTEX, nullptr);
  pthread_cond_init(&task::TQ::NOT_EMPTY, nullptr);
  pthread_cond_init(&task::TQ::NOT_FULL, nullptr);

  task::TQ::MAX_SIZE = 0xFFFF;
}

// end fills task queue with EOW (end of work) task descriptors.
void pool::end(size_t max_threads)
{
  for (decltype(max_threads) i = 0; i < max_threads; ++i) {
    auto eow = task::task_descr_t::create_eow();
    task::produce_task(eow);
  }

  // TODO: wait until all threads are finished.
}

// - Only master thread pushes tasks to the task queue
//
// - There has to be a global condition variable the worker threads
void pool::run()
{
  while (true) {
    // Read task descriptor from input stream.
    auto new_td = task::task_descr_t::create_from_stream(std::cin);
    if (new_td == nullptr) {
      break;
    }
    task::produce_task(new_td);
  }

  this->end(this->max_threads);
}

}
