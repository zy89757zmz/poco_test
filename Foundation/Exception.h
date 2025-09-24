

#ifndef Foundation_Exception_INCLUDED
#define Foundation_Exception_INCLUDED

#include "Foundation.h"
#include <stdexcept>

namespace Poco {

class Foundation_API Exception: public std::exception
{
public:
    Exception(const std::string& msg, int code = 0);

    Exception(const std::string& msg, const std::string& arg, int code = 0);

    Exception(const std::string& msg, const Exception& nested, int code = 0);

    Exception(const Exception& exc);

    ~Exception() noexcept override;

    Exception& operator = (const Exception& exc);

    virtual const char* name() const noexcept;

    virtual const char* className() const noexcept;

    const char* what() const noexcept override;

    const Exception* nested() const;

    const std::string& message() const;

    int code() const;

    std::string displayText() const;

    virtual Exception* clone() const;

    virtual void rethrow() const;

protected:
    Exception(int code = 0);

    void message(const std::string& msg);

    void extendedMessage(const std::string& arg);

private:
    std::string _msg;
    Exception* _pNested;
    int _code;
};

inline const Exception* Exception::nested() const
{
    return _pNested;
}

inline const std::string& Exception::message() const
{
    return _msg;
}

inline void Exception::message(const std::string& msg)
{
    _msg = msg;
}

inline int Exception::code() const
{
    return _code;
}


#define POCO_DECLARE_EXCEPTION_CODE(API, CLS, BASE, CODE)                                \
    class API CLS: public BASE                                                           \
    {                                                                                    \
    public:                                                                              \
        CLS(int code = CODE);                                                            \
        CLS(const std::string& msg, int code = CODE);                                    \
        CLS(const std::string& msg, const std::string& arg, int code = CODE);            \
        CLS(const std::string& msg, const Poco::Exception& exc, int code = CODE);        \
        CLS(const CLS& exc);                                                             \
        ~CLS() noexcept;                                                                 \
        CLS& operator = (const CLS& exc);                                                \
        const char* name() const noexcept;                                               \
        const char* className() const noexcept;                                          \
        Poco::Exception* clone() const;                                                  \
        void rethrow() const;                                                            \
    };


#define POCO_DECLARE_EXCEPTION(API, CLS, BASE) \
    POCO_DECLARE_EXCEPTION_CODE(API, CLS, BASE, 0)

#define POCO_IMPLEMENT_EXCEPTION(CLS, BASE, NAME)                                                                       \
    CLS::CLS(int code): BASE(code)                                                                                      \
    {                                                                                                                   \
    }                                                                                                                   \
    CLS::CLS(const std::string& msg, int code): BASE(msg, code)                                                         \
    {                                                                                                                   \
    }                                                                                                                   \
    CLS::CLS(const std::string& msg, const std::string& arg, int code): BASE(msg, arg, code)                            \
    {                                                                                                                   \
    }                                                                                                                   \
    CLS::CLS(const std::string& msg, const Poco::Exception& exc, int code): BASE(msg, exc, code)                        \
    {                                                                                                                   \
    }                                                                                                                   \
    CLS::CLS(const CLS& exc): BASE(exc)                                                                                 \
    {                                                                                                                   \
    }                                                                                                                   \
    CLS::~CLS() noexcept                                                                                                \
    {                                                                                                                   \
    }                                                                                                                   \
    CLS& CLS::operator = (const CLS& exc)                                                                               \
    {                                                                                                                   \
        BASE::operator = (exc);                                                                                         \
        return *this;                                                                                                   \
    }                                                                                                                   \
    const char* CLS::name() const noexcept                                                                              \
    {                                                                                                                   \
        return NAME;                                                                                                    \
    }                                                                                                                   \
    const char* CLS::className() const noexcept                                                                         \
    {                                                                                                                   \
        return typeid(*this).name();                                                                                    \
    }                                                                                                                   \
    Poco::Exception* CLS::clone() const                                                                                 \
    {                                                                                                                   \
        return new CLS(*this);                                                                                          \
    }                                                                                                                   \
    void CLS::rethrow() const                                                                                           \
    {                                                                                                                   \
        throw *this;                                                                                                    \
    }


POCO_DECLARE_EXCEPTION(Foundation_API, LogicException, Exception)
POCO_DECLARE_EXCEPTION(Foundation_API, AssertionViolationException, LogicException)

POCO_DECLARE_EXCEPTION(Foundation_API, BugcheckException, LogicException)



POCO_DECLARE_EXCEPTION(Foundation_API, RuntimeException, Exception)

POCO_DECLARE_EXCEPTION(Foundation_API, PoolOverflowException, RuntimeException)


}

#endif