#include <task/task.hpp>
#include <thread/thread.hpp>

namespace thread {

// Initialize static members
std::set<int>   thread::free_threads;
pthread_mutex_t thread::free_threads_mutex;
pthread_cond_t  thread::all_threads_free;

}
