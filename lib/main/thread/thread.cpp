#include <task/task.hpp>
#include <thread/thread.hpp>

namespace thread {

// Initialize static members
unsigned thread::num_free_threads = 0;
pthread_mutex_t thread::num_free_threads_mutex;

}
