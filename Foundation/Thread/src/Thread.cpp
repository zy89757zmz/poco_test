#include "Thread.h"
#include <pthread.h>
#include <signal.h>
#include <limits.h>

namespace Poco {

    Thread::Thread(uint32_t sigMask)
    {
        // Constructor implementation
    }

    Thread::Thread(const std::string& name, uint32_t sigMask)
    {
        // Constructor implementation
    }

    Thread::~Thread()
    {
        // Destructor implementation
    }

    void Thread::setName(const std::string& name)
    {
        std::string realName = name;
        if (name.size() > static_cast<std::size_t>(std::min(MAX_THREAD_NAME_LENGTH, 15)))
        {
            std::size_t half = static_cast<std::size_t>(std::min(MAX_THREAD_NAME_LENGTH, 15) - 1) / 2;
            std::string halfName(name, 0, half);
            halfName.append("~");
            halfName.append(name, name.size() - half);
            realName = halfName;
        }

        _pData->name = realName;
    }

    int Thread::mapPrio(int prio, int policy)
    {
        int minPrio = sched_get_priority_min(policy);
        int maxPrio = sched_get_priority_max(policy);

        switch (prio)
        {
        case PRIO_LOWEST:
            return minPrio;
        case PRIO_LOW:
            return minPrio + (maxPrio - minPrio) / 4;
        case PRIO_NORMAL:
            return minPrio + (maxPrio - minPrio) / 2;
        case PRIO_HIGH:
            return minPrio + (maxPrio - minPrio) * 3 / 4;
        case PRIO_HIGHEST:
            return maxPrio;
        default:
            return -1;
        }
    }

    void Thread::setPriority(Priority prio)
    {
        if (prio != _pData->priority)
        {
            _pData->priority = prio;
            _pData->policy = POLICY_DEFAULT;

            if (isRunning())
            {
                struct sched_param param;
                param.sched_priority = mapPrio(prio, _pData->policy);

                int errorCode = pthread_setschedparam(_pData->thread, _pData->policy, &param);
                if (errorCode != 0)
                {
                    throw std::system_error(std::error_code(errorCode, std::generic_category()), "cannot set thread priority");
                }
            }
        }
    }

    void Thread::setStackSize(int size)
    {
        if ((size != 0) && (size < PTHREAD_STACK_MIN))
        {
            size = PTHREAD_STACK_MIN;
        }
        
        _pData->stackSize = size;
    }

    void* Thread::runnableEntry(void* pThread)
    {
        _currentThreadHolder.set(reinterpret_cast<Thread*>(pThread));

        sigset_t sigMask;
        sigemptyset(&sigMask);
        sigaddset(&sigMask, SIGQUIT);
        sigaddset(&sigMask, SIGTERM);
        sigaddset(&sigMask, SIGPIPE);
        pthread_sigmask(SIG_BLOCK, &sigMask, nullptr);

        Thread* pThreadImpl = reinterpret_cast<Thread*>(pThread);

        try
        {
            pThreadImpl->_pData->pRunnableTarget->run();
        }
        catch (...)
        {
            // TODO: log the exception
        }
    }

    void Thread::start(std::shared_ptr<Runnable> pTarget)
    {
        if (isRunning())
        {
            throw std::runtime_error("thread already running");
        }

        pthread_attr_t attr;
        pthread_attr_init(&attr);

        if (_pData->stackSize != 0)
        {
            int errorCode = pthread_attr_setstacksize(&attr, _pData->stackSize);
            if (errorCode != 0)
            {
                pthread_attr_destroy(&attr);
                throw std::system_error(std::error_code(errorCode, std::generic_category()), "cannot set thread stack size");
            }
        }

        {
            _pData->pRunnableTarget = pTarget;
            int errorCode = pthread_create(&_pData->thread, &attr, runnableEntry, this);
            if (errorCode != 0)
            {
                _pData->pRunnableTarget = nullptr;
                pthread_attr_destroy(&attr);
                throw std::system_error(std::error_code(errorCode, std::generic_category()), "cannot create thread");
            }
        }

        _pData->started = true;
        _pData->joined = false;
        pthread_attr_destroy(&attr);
    }
    
} // namespace Poco