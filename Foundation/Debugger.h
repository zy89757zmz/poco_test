

#ifndef Foundation_Debugger_INCLUDED
#define Foundation_Debugger_INCLUDED

#include "Foundation.h"
#include <string>

namespace Poco {

class Foundation_API Debugger
{
public:

    static void enter(const std::string& msg, const char* file, LineNumber line);
};

}

#endif
