

#ifndef Foundation_Bugcheck_INCLUDED
#define Foundation_Bugcheck_INCLUDED


#include <string>
#include "Foundation.h"

namespace Poco {

class Foundation_API Bugcheck
{
public:
    [[noreturn]] static void assertion(const char* cond, const char* file, LineNumber line, const char* test = nullptr);

    [[noreturn]] static void bugcheck(const char* msg, const char* file, LineNumber line);

protected:
    static std::string what(const char* msg, const char* file, LineNumber line, const char* text = nullptr);
};


}

#define poco_assert(cond) \
	if (!(cond)) Poco::Bugcheck::assertion(#cond, __FILE__, __LINE__); else (void) 0

#define poco_bugcheck_msg(msg) \
    Poco::Bugcheck::bugcheck(msg, __FILE__, __LINE__)

#endif