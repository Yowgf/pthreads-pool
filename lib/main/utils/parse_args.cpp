#include <utils/parse_args.hpp>
#include <utils/log.hpp>

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
    return parsed_args{std::move(err_msg)};
  }

  // Parse min / max threads number
  min_threads = std::stoi(argv[1]);
  max_threads = std::stoi(argv[2]);

  // If -d is found anywhere, debug mode is activated.
  for (auto i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "-d") {
      debug = true;
    }
  }

  LOG(debug, "Received config (min_threads=%d max_threads=%d debug=%d)",
      min_threads, max_threads, debug);

  return parsed_args{std::move(err_msg), debug, min_threads, max_threads};
}

}
