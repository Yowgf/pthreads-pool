#ifndef TASK_TASK_H
#define TASK_TASK_H

#include <iostream>
#include <memory>

#include <task/process.hpp>

namespace task {

std::unique_ptr<task_descr_t> fetch_task(std::istream& is);
bool is_eow(task_descr_t* td);

}

#endif
