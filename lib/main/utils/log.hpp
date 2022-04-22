#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#include <iostream>

// Normal logging function. Accepts printf-like formatting.
#define LOG(isSupposedToPrint, msg, ...)\
  if (isSupposedToPrint) {\
    fprintf(stderr, "[INFO] " msg "\n", ##__VA_ARGS__);\
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
