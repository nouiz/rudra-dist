#include <stdexcept>

#ifndef RUDRA_UTIL_EXCEPTION_H_
#define RUDRA_UTIL_EXCEPTION_H_

namespace rudra {

  class Exception : public std::logic_error {

  public:
  Exception(const std::string &msg) :
    std::logic_error(msg)
  {
  }

};

}
#endif
