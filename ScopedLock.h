#ifndef _SCOPEDLOCK_H
#define _SCOPEDLOCK_H


template <typename T>
class ScopedLock
{
public:
    explicit ScopedLock(T& mutex)
        : _mutex(mutex)
    {
        _mutex.lock();
    }

    ScopedLock(T& mutex, long milliseconds)
        : _mutex(mutex)
    {
        _mutex.lock(milliseconds);
    }

    ~ScopedLock()
    {
        try
        {
            _mutex.unlock();
        }
        catch (const std::exception& e)
        {
            std::cerr << "ScopedLock destructor : " << e.what() << '\n';
        }
    }

    ScopedLock() = delete;
    ScopedLock(const ScopedLock&) = delete;
    ScopedLock& operator=(const ScopedLock&) = delete;

private:
    T& _mutex;
};

template <typename T>
class ScopedLockWithUnlock
{
public:
    explicit ScopedLockWithUnlock(T& mutex)
        : _pMutex(&mutex)
    {
        if (_pMutex)
        {
            _pMutex->lock();
            _locked = true;
        }
    }

    ScopedLockWithUnlock(T& mutex, long milliseconds)
        : _pMutex(&mutex)
    {
        if (_pMutex)
        {
            _pMutex->lock(milliseconds);
            _locked = true;
        }
    }

    ~ScopedLockWithUnlock()
    {
        try
        {
            unlock();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    ScopedLockWithUnlock() = delete;
    ScopedLockWithUnlock(const ScopedLockWithUnlock&) = delete;
    ScopedLockWithUnlock& operator=(const ScopedLockWithUnlock&) = delete;

    void lock()
    {
        if (_pMutex && !_locked)
        {
            _pMutex->lock();
            _locked = true;
        }
    }

    void unlock()
    {
        if (_pMutex && _locked)
        {
            _pMutex->unlock();
            _locked = false;
        }
    }

private:
    T* _pMutex;
    bool _locked = false;
};

#endif // _SCOPEDLOCK_H