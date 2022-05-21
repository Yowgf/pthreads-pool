#include <utils/parse_args.hpp>
#include <utils/log.hpp>

namespace utils {

parsed_args parse_args(int argc, char** argv)
{
  std::string err_msg;
  log::log_level log_level = log::log_level{};
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
      log_level = log::log_level::DEBUG;
      std::cerr << "Found -d: " << argv[i];
      break;
    }
  }

  LOG("Received config (min_threads=%d max_threads=%d log_level=%d)",
      min_threads, max_threads, static_cast<int>(log_level));

  return parsed_args{std::move(err_msg), log_level, min_threads, max_threads};
}

}
