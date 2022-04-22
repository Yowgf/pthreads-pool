#ifndef UTILS_ERROR_H
#define UTILS_ERROR_H

class error {
public:
  error(std::string&& msg) : msg(msg)
  {}

  std::string msg;
};

#endif
