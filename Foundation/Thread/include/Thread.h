
#ifndef Foundation_Thread_Thread_INCLUDED
#define Foundation_Thread_Thread_INCLUDED

#include "Foundation.h"
#include <sched.h>

namespace Poco {

class Foundation_API ThreadImpl
{
public:
    enum Priority
    {
        PRIO_LOWEST,
        PRIO_LOW,
        PRIO_NORMAL,
        PRIO_HIGH,
        PRIO_HIGHEST
    };

    enum Policy
    {
        POLICY_DEFAULT = SCHED_OTHER
    };

    ThreadImpl();
    ~ThreadImpl();

};
}

#endif // Foundation_Thread_Thread_INCLUDED