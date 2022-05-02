#ifndef TASK_TASK_H
#define TASK_TASK_H

#include <memory>
#include <queue>

#include <task/process.hpp>

namespace task {

// Task descriptor queue.
using tdq = std::queue<task::task_descr_t*>;

// fetch_task gets a task from the queue and returns it.
std::unique_ptr<task_descr_t> fetch_task(task::tdq& task_queue);

// is_eow checks if task indicates end of work.
bool is_eow(task_descr_t* td);

}

#endif
