#ifndef UTILS_PARSE_ARGS_H
#define UTILS_PARSE_ARGS_H

#include <memory>
#include <string>

#include <utils/error.hpp>

namespace utils {

class parsed_args {
public:
  parsed_args(std::string&& err_msg,
	      bool debug) :
    debug(debug)
  {
    if (err_msg != "") {
      this->err = std::make_unique<error>(std::forward<std::string>(err_msg));
      return;
    }
  }

  std::unique_ptr<error> err;
  bool debug;
};

// parse_args is a factory function for parsed_args.
parsed_args parse_args(int argc, char** argv);

}

#endif
