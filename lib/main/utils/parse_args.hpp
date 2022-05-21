#ifndef UTILS_PARSE_ARGS_H
#define UTILS_PARSE_ARGS_H

#include <memory>
#include <string>

#include <utils/error.hpp>
#include <utils/log.hpp>

namespace utils {

class parsed_args {
public:
  parsed_args(std::string&& err_msg,
	      log::log_level log_level = log::log_level{},
	      int min_threads = 0,
	      int max_threads = 40) :
    log_level(log_level), min_threads(min_threads), max_threads(max_threads)
  {
    if (err_msg != "") {
      this->err = std::make_unique<error>(std::forward<std::string>(err_msg));
      return;
    }
  }

  std::unique_ptr<error> err;
  log::log_level log_level;
  int min_threads;
  int max_threads;
};

// parse_args is a factory function for parsed_args.
parsed_args parse_args(int argc, char** argv);

}

#endif
