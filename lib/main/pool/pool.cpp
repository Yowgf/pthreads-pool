#include <iostream>
#include <queue>
#include <vector>

#include <pool/pool.hpp>
#include <task/task.hpp>
#include <thread/thread.hpp>
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
{}

// TODOs:
//
// - master thread initializes the run, and worker threads execute the rest
//
// - pthread_exit instead of return
//
void pool::run()
{
  thread::thread t;

  auto td = task::fetch_task(std::cin);
  t.work(td.get());
}

}
