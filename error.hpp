#ifndef ERROR_HPP
#define ERROR_HPP
#include <ostream>
#include <string>
class Error : public std::exception
{
  const std::string message;

public:
  Error() = default;
  Error(const std::string &error_disc) : message(error_disc){};
  Error(const Error &x) : message(x.message){};
  friend std::ostream &operator<<(std::ostream &os, const Error &error)
  {
    os << "message: " << error.message;
    return os;
  }
};
#endif 