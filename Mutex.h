#ifndef _MUTEX_H
#define _MUTEX_H

#include "Foundation.h"
#include "ScopedLock.h"
#include <string>

union semun
{
    int                         val;
    struct semid_ds*            buf;
    unsigned short int*         array;
    struct seminfo*             __buf;
};


class Foundation_API MutexImpl
{
protected:
    MutexImpl(const std::string& name);
    ~MutexImpl();

    void lockImpl();
    bool tryLockImpl();
    void unlockImpl();

private:
    std::string getFileName();

    std::string _name;
    int _semid;
    bool _owned;
};

class Foundation_API Mutex: private MutexImpl
{
public:
    using ScopedLock = ScopedLock<Mutex>;
    using ScopedLockWithUnlock = ScopedLockWithUnlock<Mutex>;

    Mutex(const std::string& name);
    ~Mutex();

    void lock();
    void tryLock();
    void unlock();

private:
    Mutex() = default;
    Mutex(const Mutex&) = default;
    Mutex& operator=(const Mutex&) = default;
};

inline void Mutex::lock()
{
    lockImpl();
}

inline void Mutex::tryLock()
{
    tryLockImpl();
}

inline void Mutex::unlock()
{
    unlockImpl();
}

#endif // _MUTEX_H