#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#include <iostream>
#include <pthread.h>

namespace log {

enum class log_level {
  FATAL   = 0,
  WARNING = 1,
  INFO    = 2,
  DEBUG   = 3
};

extern log_level LOG_LEVEL;
extern void set_log_level(log_level new_level);

// Normal logging function. Accepts printf-like formatting. msg must be a raw
// string.
#define LOG(msg, ...)\
  if (log::LOG_LEVEL >= log::log_level::INFO) {\
    fprintf(stderr, "[INFO] (%ld) " msg "\n", pthread_self(), ##__VA_ARGS__);\
  }

// Log attribute att.
#define LOGATT(att)\
  if (log::LOG_LEVEL >= log::log_level::INFO) {\
    std::cerr << #att << ": " << (att) << '\n';\
  }

// The same as LOG, but prints file name and line number.
#define DEBUG(msg,...)\
  if (log::LOG_LEVEL >= log::log_level::DEBUG) {\
    fprintf(stderr, "[DEBUG] (%s:%d) " msg "\n",\
            __FILE__, __LINE__, ##__VA_ARGS__);\
  }

}

#endif
