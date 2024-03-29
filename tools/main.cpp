#include <iostream>

#include <pool/pool.hpp>
#include <utils/parse_args.hpp>
#include <utils/log.hpp>

int main(int argc, char** argv) {
  auto parsed_args = utils::parse_args(argc, argv);

  if (parsed_args.err != nullptr) {
    std::cerr << "Error parsing program arguments: " << parsed_args.err->msg
	      << std::endl;
    return 1;
  }

  log::set_log_level(parsed_args.log_level);

  pool::pool p{parsed_args};
  p.init();
  p.run();

  return 0;
}
