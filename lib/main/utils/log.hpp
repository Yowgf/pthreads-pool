#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#include <iostream>
#include <pthread.h>

// Normal logging function. Accepts printf-like formatting. msg must be a raw
// string.
#define LOG(isSupposedToPrint, msg, ...)\
  if (isSupposedToPrint) {\
    fprintf(stderr, "[INFO] (%ld) " msg "\n", pthread_self(), ##__VA_ARGS__);\
  }

// Log attribute att.
#define LOGATT(isSupposedToPrint, att)          \
  if (isSupposedToPrint) {                      \
    std::cerr << #att << ": " << (att) << '\n';      \
  }

// The same as LOG, but prints file name and line number.
#define DEBUG(isSupposedToPrint, msg,...)\
  if (isSupposedToPrint) {\
    fprintf(stderr, "[DEBUG] (%s:%d) " msg "\n",\
            __FILE__, __LINE__, ##__VA_ARGS__);\
  }

#endif
