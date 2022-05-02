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

  void init();
  void run();
  void end(task::tdq&, int max_threads);

private:
  bool debug;
  int min_threads;
  int max_threads;
};

}

#endif
