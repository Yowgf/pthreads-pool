#ifndef TASK_TASK_H
#define TASK_TASK_H

#include <memory>
#include <pthread.h>
#include <queue>

#include <task/process.hpp>

namespace task {

// Task descriptor queue.
using tdq = std::queue<task_descr_t*>;

class TQ {
public:
  static tdq             QUEUE;
  // This mutex should be used to access any of the TASK_QUEUE_* variables.
  static pthread_mutex_t MUTEX;
  // Cond vars.
  static pthread_cond_t  NOT_EMPTY;
  static pthread_cond_t  NOT_FULL;
  static size_t          NUM_CONSUMER_THREADS_WAITING;
  // MAX_SIZE is initialized by pool of threads and should be read-only
  // afterwards.
  static size_t          MAX_SIZE;
};

// produce_task and consume_task implement the producer-consumer pattern.
void produce_task(task_descr_t* new_task);
std::unique_ptr<task_descr_t> consume_task();

size_t get_num_consumer_threads_waiting();

// is_eow checks if task indicates end of work.
bool is_eow(task_descr_t* td);

}

#endif
