

#include "Exception.h"
#include <typeinfo>
#include <sstream>
#include <exception>

namespace Poco {

Exception::Exception(int code): _pNested(0), _code(code)
{
}

Exception::Exception(const std::string& msg, int code): _msg(msg), _pNested(0), _code(code)
{
}

Exception::Exception(const std::string& msg, const std::string& arg, int code): _msg(msg), _pNested(0), _code(code)
{
    if (!arg.empty())
    {
        _msg.append(": ");
        _msg.append(arg);
    }
}

Exception::Exception(const std::string& msg, const Exception& nested, int code): _msg(msg), _pNested(nested.clone()), _code(code)
{
}

Exception::Exception(const Exception& exc):
	std::exception(exc),
    _msg(exc._msg),
    _code(exc._code)
{
	_pNested = exc._pNested ? exc._pNested->clone() : 0;
}

Exception::~Exception() noexcept
{
    delete _pNested;
}

Exception& Exception::operator = (const Exception& exc)
{
    if (&exc != this)
    {
        Exception* newPNested = exc._pNested ? exc._pNested->clone() : 0;
        delete _pNested;
        _msg = exc._msg;
        _pNested = newPNested;
        _code = exc._code;
    }
    return *this;
}

const char* Exception::name() const noexcept
{
    return "Exception";
}

const char* Exception::className() const noexcept
{
    return typeid(*this).name();
}

const char* Exception::what() const noexcept
{
    return name();
}

std::string Exception::displayText() const
{
    std::string txt = name();
    if (!_msg.empty())
    {
        txt.append(": ");
        txt.append(_msg);
    }
    return txt;
}

void Exception::extendedMessage(const std::string& arg)
{
    if ((!arg.empty()) && (!_msg.empty()))
    {
        _msg.append(": ");
        _msg.append(arg);
    }
}

Exception* Exception::clone() const
{
    return new Exception(*this);
}

void Exception::rethrow() const
{
    throw *this;
}

POCO_IMPLEMENT_EXCEPTION(LogicException, Exception, "Logic exception")
POCO_IMPLEMENT_EXCEPTION(AssertionViolationException, LogicException, "Assertion violation")

POCO_IMPLEMENT_EXCEPTION(BugcheckException, LogicException, "Bugcheck")



POCO_IMPLEMENT_EXCEPTION(RuntimeException, Exception, "Runtime exception")

POCO_IMPLEMENT_EXCEPTION(PoolOverflowException, RuntimeException, "Pool overflow")

}