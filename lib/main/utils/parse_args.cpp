#include <utils/parse_args.hpp>

namespace utils {

parsed_args parse_args(int argc, char** argv)
{
  std::string err_msg;

  bool debug = false;
  for (auto i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "-d") {
      debug = true;
    }
  }

  return parsed_args{std::move(err_msg), debug};
}

}
