#include <iostream>

#include <pool/pool.hpp>
#include <pool/process.hpp>
#include <utils/log.hpp>

namespace pool {

pool::pool(const utils::parsed_args& config)
{
  this->debug = config.debug;
  LOG(this->debug, "Debug mode is activated.");
}

void pool::init()
{}

std::unique_ptr<task_descr_t> fetch_task(std::istream& is)
{
  int pid = 0;
  int ms = 0;
  is >> pid >> ms;
  return std::make_unique<task_descr_t>(pid, ms);
}

bool is_eow(task_descr_t* td) {
  return td->pid == 0 && td->ms == 0;
}

// TODOs:
//
// - master thread initializes the run, and worker threads execute the rest
//
// - pthread_exit instead of return
//
void pool::run()
{
  while (true) {
    auto task = fetch_task(std::cin);
    if (is_eow(task.get())) {
      return;
    }

    processa(task.get());

    return;
  }
}

}
