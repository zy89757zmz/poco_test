
#ifndef Foundation_Thread_Thread_INCLUDED
#define Foundation_Thread_Thread_INCLUDED

#include "Foundation.h"
#include <sched.h>
#include <cstdint>
#include <string>
#include <pthread.h>
#include <memory>
#include <mutex>

namespace Poco {

class Foundation_API Thread
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

    /* Creates a thread. Call start() to start it.
     *  The optional sigMask parameter specifies which signals should be blocked.
     *  To block a specific signal, set the corresponding bit in the sigMask.
     *  Multiple bits can be set in the mask to block multiple signals if needed.
     */
    Thread(uint32_t sigMask = 0);

    /* Creates a thread. Call start() to start it.
     *  The optional sigMask parameter specifies which signals should be blocked.
     *  To block a specific signal, set the corresponding bit in the sigMask.
     *  Multiple bits can be set in the mask to block multiple signals if needed.
     */
    Thread(const std::string& name, uint32_t sigMask = 0);


    ~Thread();

    /* Returns the unique thread ID of the thread */
    int id() const;

    /* Returns the native thread ID of the thread */
    pthread_t tid() const;

    /* Returns the name of the thread */
    std::string getName() const;


private:

    struct ThreadData
    {
        ThreadData(): thread{}
        {}

        pthread_t thread;
        std::string name;
    };
    
    std::unique_ptr<ThreadData> _pData;
    int _id;


};

inline int Thread::id() const
{
    return _id;
}

inline pthread_t Thread::tid() const
{
    return _pData->thread;
}

inline std::string Thread::getName() const
{
    return _pData->name;
}


} // namespace Poco

#endif // Foundation_Thread_Thread_INCLUDED