#ifndef POOL_POOL_H
#define POOL_POOL_H

#include <utils/parse_args.hpp>

namespace pool {

class pool {
public:
  pool(const utils::parsed_args& config);

  void init();
  void run();

private:
  bool debug;
  int min_threads;
  int max_threads;
};

}

#endif
