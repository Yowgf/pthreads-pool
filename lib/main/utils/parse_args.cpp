#include <sstream>

#include <utils/parse_args.hpp>

namespace utils {

parsed_args parse_args(int argc, char** argv)
{
  std::string err_msg;
  bool debug = false;
  int min_threads = 0;
  int max_threads = 0;

  constexpr int min_args = 3;
  if (argc < min_args) {
    err_msg = std::string("need at least ") + std::to_string(min_args-1) +
      " arguments.";
    return parsed_args{std::move(err_msg), debug};
  }

  std::stringstream ss;

  // Parse min / max threads number
  ss << argv[1] << argv[2];
  ss >> min_threads >> max_threads;
  ss.flush();

  for (auto i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "-d") {
      debug = true;
    }
  }

  return parsed_args{std::move(err_msg), debug};
}

}
