#include <chrono>
#include <iomanip>
#include <iostream>
#include <pthread.h>

#include <pool/pool.hpp>
#include <utils/log.hpp>

namespace pool {

using clockT = std::chrono::duration<double>;

pool::pool(const utils::parsed_args& config)
{
  this->debug = config.debug;
  LOG(this->debug, "Debug mode is activated.");

  this->min_threads = static_cast<size_t>(config.min_threads);
  this->max_threads = static_cast<size_t>(config.max_threads);
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
  pthread_mutex_init(&thread::thread::free_threads_mutex, nullptr);
  pthread_mutex_init(&task::TQ::MUTEX, nullptr);
  pthread_cond_init(&task::TQ::NOT_EMPTY, nullptr);
  pthread_cond_init(&task::TQ::NOT_FULL, nullptr);

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

  LOG(this->debug, "Waiting for threads to finish.");
  thread::thread::wait_all_threads_done(this->max_threads);

  LOG(this->debug, "Successfully ended pool of threads.");
}

void pool::dispatch_thread()
{
  pthread_mutex_lock(&thread::thread::free_threads_mutex);
  if (task::TQ::NUM_CONSUMER_THREADS_WAITING <= 0) {
    // TODO: Make some NEW thread work
    
  }
  pthread_mutex_unlock(&thread::thread::free_threads_mutex);
}

// - Only master thread pushes tasks to the task queue
//
// - There has to be a global condition variable the worker threads can use to
//   indicate they have finished their task.
//
void pool::run()
{
  auto before = std::chrono::high_resolution_clock::now();

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
    dispatch_thread();
  }

  this->end(this->max_threads);

  clockT elapsed = std::chrono::high_resolution_clock::now() - before;
  double clkCount = elapsed.count();
  LOG(this->debug, "Execution time (seconds): %.6f", clkCount);
}

}
