#include <utils/log.hpp>


namespace log {

log_level LOG_LEVEL;

void set_log_level(log_level new_level) {
  LOG_LEVEL = new_level;
}

}
