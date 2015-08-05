

#ifndef RUDRA_UTIL_CHECKING_H_
#define RUDRA_UTIL_CHECKING_H_

#include "../util/Exception.h"
#include <sstream>

namespace rudra {

  // This function is just a convenience for setting breakpoints in a debugger.
  // It is automatically called with any error
  void breakpoint();




  // This class is a container for checking-related stuff
  class Checking
  {
  public:

    typedef enum {
      USER_ERROR,
      ASSERT,
      RETURN_CODE,
      CUDA_E,
      CUBLAS,
      CUDNN
    } ERROR_KIND;
  
    static void reportError(ERROR_KIND         errorKind,
                            const std::string &msg,
                            const char        *file,
                            int                line) throw(Exception);
  }; 
}



#define RUDRA_CHECK_USER(expr, msg) {                                                                               \
                           bool  e = (expr);                                                                        \
                           if (!e) {                                                                                \
                             std::ostringstream msgStream; msgStream << msg;                                        \
                             Checking::reportError(Checking::USER_ERROR, msgStream.str(), __FILE__, __LINE__);   \
                           }                                                                                        \
                         }

#define RUDRA_CHECK(expr, msg) {                                                                                    \
                           bool  e = (expr);                                                                        \
                           if (!e) {                                                                                \
                             std::ostringstream msgStream; msgStream << "ASSERT " << msg;                           \
                             Checking::reportError(Checking::ASSERT, msgStream.str(), __FILE__, __LINE__);          \
                           }                                                                                        \
                         }


#define RUDRA_VAR(expr) "  " << #expr << " = " << (expr)
#endif /* RUDRA_UTIL_CHECKING_H_ */
