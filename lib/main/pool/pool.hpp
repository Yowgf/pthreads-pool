#ifndef POOL_POOL_H
#define POOL_POOL_H

#include <vector>

#include <task/task.hpp>
#include <thread/thread.hpp>
#include <utils/parse_args.hpp>

namespace pool {

class pool {
public:
  pool(const utils::parsed_args& config);
  ~pool();

  void init();
  void run();

private:
  size_t min_threads;
  size_t max_threads;

  std::vector<thread::thread> threads;

  void dispatch_thread();
  void end(size_t max_threads);
};

}

#endif
