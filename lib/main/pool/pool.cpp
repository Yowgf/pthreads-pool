#include <iostream>
#include <pthread.h>

#include <pool/pool.hpp>
#include <utils/log.hpp>

namespace pool {

pool::pool(const utils::parsed_args& config)
{
  this->debug = config.debug;
  LOG(this->debug, "Debug mode is activated.");

  this->min_threads = config.min_threads;
  this->max_threads = config.max_threads;
}

void pool::init()
{
  // Initialize mutexes and condition variables.
  pthread_mutex_init(&thread::thread::numthreads_mutex, nullptr);
  pthread_mutex_init(&thread::thread::numfree_mutex, nullptr);

  // Create the minimum number of threads necessary for the pool to be
  // considered ready.
  for (int i = 0; i < this->min_threads; ++i) {
    thread::thread::create();
  }
}

// end sends EOW (end of work) task to all threads
void pool::end(task::tdq& q, int max_threads)
{
  for (int i = 0; i < max_threads; ++i) {
    auto eow = task::task_descr_t::create_eow();
    q.push(eow);
  }
}

void pool::run()
{
  task::tdq task_queue;

  task_descr_t* td = nullptr;
  while (true) {
    // Read task descriptor from input stream.
    auto new_td = task::task_descr_t::create_from_stream(std::cin);
    if (new_td == nullptr) {
      // TODO: This seems wrong
      break;
    }
    task_queue.push(new_td);
  }

  this->end(task_queue, this->max_threads);
}

}
