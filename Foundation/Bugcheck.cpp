

#include "Bugcheck.h"
#include "Exception.h"
#include "Debugger.h"
#include <sstream>

namespace Poco {

void Bugcheck::assertion(const char* cond, const char* file, LineNumber line, const char* text)
{
    std::string message("Assertion violation: ");
    message += cond;
    if (text)
    {
        message += " (";
        message += text;
        message += ")";
    }
    Debugger::enter(message, file, line);
    throw AssertionViolationException(what(cond, file, line, text));
}

void Bugcheck::bugcheck(const char* msg, const char* file, LineNumber line)
{
    std::string m("Bugcheck");
    if (msg)
    {
        m.append(": ");
        m.append(msg);
    }
    Debugger::enter(m, file, line);
    throw BugcheckException(what(msg, file, line));
}

std::string Bugcheck::what(const char* msg, const char* file, LineNumber line, const char* text)
{
	std::ostringstream str;
	if (msg) str << msg << " ";
    if (text != NULL) str << "(" << text << ") ";
	str << "in file \"" << file << "\", line " << line;
	return str.str();
}
}