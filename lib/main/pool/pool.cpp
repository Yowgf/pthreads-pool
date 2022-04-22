#include <pool/pool.hpp>
#include <utils/log.hpp>

namespace pool {

pool::pool(const utils::parsed_args& config)
{
  this->debug = config.debug;
  LOG(this->debug, "Debug mode is activated.");
}

void pool::init()
{}

void pool::run()
{}

}
