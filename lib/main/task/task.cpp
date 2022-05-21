#include <task/task.hpp>

namespace task {

// Initialize static variables.
tdq             TQ::QUEUE;
pthread_mutex_t TQ::MUTEX;
pthread_cond_t  TQ::NOT_EMPTY;
pthread_cond_t  TQ::NOT_FULL;
size_t          TQ::NUM_CONSUMER_THREADS_WAITING;
size_t          TQ::MAX_SIZE;  

std::unique_ptr<task_descr_t> consume_task()
{
  pthread_mutex_lock(&TQ::MUTEX);

  TQ::NUM_CONSUMER_THREADS_WAITING++;

  while (TQ::QUEUE.size() <= 0) {
    pthread_cond_wait(&TQ::NOT_EMPTY, &TQ::MUTEX);
  }

  TQ::NUM_CONSUMER_THREADS_WAITING--;

  auto td_ptr = TQ::QUEUE.front(); TQ::QUEUE.pop();

  pthread_cond_signal(&TQ::NOT_FULL);
  pthread_mutex_unlock(&TQ::MUTEX);

  return std::unique_ptr<task_descr_t>(td_ptr);
}

void produce_task(task_descr_t* new_task)
{
  pthread_mutex_lock(&TQ::MUTEX);

  while (TQ::QUEUE.size() >= TQ::MAX_SIZE) {
    pthread_cond_wait(&TQ::NOT_FULL, &TQ::MUTEX);
  }

  TQ::QUEUE.push(new_task);

  pthread_cond_signal(&TQ::NOT_EMPTY);
  pthread_mutex_unlock(&TQ::MUTEX);
}

size_t get_num_consumer_threads_waiting()
{
  pthread_mutex_lock(&TQ::MUTEX);
  auto num_threads_waiting = TQ::NUM_CONSUMER_THREADS_WAITING;
  pthread_mutex_unlock(&TQ::MUTEX);

  return num_threads_waiting;
}

bool is_eow(task_descr_t* td)
{
  return td->pid == 0 && td->ms == 0;
}

}
