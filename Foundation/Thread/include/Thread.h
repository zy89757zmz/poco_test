
#ifndef Foundation_Thread_Thread_INCLUDED
#define Foundation_Thread_Thread_INCLUDED

#include "Foundation.h"
#include "Runnable.h"
#include <sched.h>
#include <cstdint>
#include <string>
#include <pthread.h>
#include <memory>
#include <mutex>
#include <atomic>

#define MAX_THREAD_NAME_LENGTH 15

namespace Poco {

namespace {

class RunnableHolder : public Runnable
{
public:
    RunnableHolder(Runnable& target):
        _target(target)
    {
    }

    ~RunnableHolder() override = default;

    void run() override
    {
        _target.run();
    }

private:
    Runnable& _target;
};
}

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

    /* Sets the name of the thread 
     * Note that it only take effect before start method invoked
     */
    void setName(const std::string& name);

    /* Returns the priority of the thread */
    Priority getPriority() const;

    /* Sets the priority of the thread */
    void setPriority(Priority prio);

    /* Sets the thread's stack size in bytes */
    void setStackSize(int size);

    /* Returns the thread's stack size in bytes */
    int getStackSize() const;

    void start(std::shared_ptr<Runnable> pTarget);


private:
    /* Returns true if the thread is currently running */
    bool isRunning() const;

    static int mapPrio(int prio, int policy = SCHED_OTHER);

    static void* runnableEntry(void* pThread);


private:

    class CurrentThreadHolder
    {
    public:
        CurrentThreadHolder()
        {
            int errorCode = pthread_key_create(&_key, nullptr);
            if (errorCode != 0)
            {
                throw std::system_error(std::error_code(errorCode, std::generic_category()), "cannot create thread local storage");
            }
        }

        ~CurrentThreadHolder()
        {
            pthread_key_delete(_key);
        }

        Thread* get() const
        {
            return reinterpret_cast<Thread*>(pthread_getspecific(_key));
        }

        void set(Thread* pThread)
        {
            int errorCode = pthread_setspecific(_key, pThread);
            if (errorCode != 0)
            {
                throw std::system_error(std::error_code(errorCode, std::generic_category()), "cannot set thread local storage");
            }
        }

    private:
        pthread_key_t _key;
    };

    struct ThreadData
    {
        ThreadData():
            thread{},
            priority(PRIO_NORMAL),
            policy(POLICY_DEFAULT),
            stackSize(0),
            started(false),
            joined(false)
        {}

        std::shared_ptr<Runnable> pRunnableTarget;
        pthread_t thread;
        std::string name;
        Priority priority;
        int policy;
        std::size_t stackSize;
        std::atomic<bool> started;
        std::atomic<bool> joined;
    };
    
    std::unique_ptr<ThreadData> _pData;
    static CurrentThreadHolder _currentThreadHolder;
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

inline Thread::Priority Thread::getPriority() const
{
    return _pData->priority;
}

inline bool Thread::isRunning() const
{
    return (_pData->pRunnableTarget.get() != nullptr);
}

inline int Thread::getStackSize() const
{
    return static_cast<int>(_pData->stackSize);
}





}// namespace Poco

#endif // Foundation_Thread_Thread_INCLUDED