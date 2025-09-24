#ifndef _ATOMCOUNTER_H
#define _ATOMCOUNTER_H
#include "Foundation.h"
#include <atomic>


class Foundation_API AtomicCounter
{
public:
    using ValueType = int;

    AtomicCounter();
    explicit AtomicCounter(ValueType initalValue);
    AtomicCounter(const AtomicCounter& counter);

    ~AtomicCounter();

    AtomicCounter& operator = (const AtomicCounter& counter);
    AtomicCounter& operator = (ValueType value);

    operator ValueType () const;

    ValueType value() const;

    ValueType operator ++ ();
    ValueType operator ++ (int);
    ValueType operator -- ();
    ValueType operator -- (int);
    bool operator ! () const;

private:
    std::atomic<int> _counter;
};


inline AtomicCounter::operator AtomicCounter::ValueType () const
{
    return _counter.load();
}

inline AtomicCounter::ValueType AtomicCounter::value() const
{
    return _counter.load();
}

inline AtomicCounter::ValueType AtomicCounter::operator ++ ()
{
    return ++_counter;
}

inline AtomicCounter::ValueType AtomicCounter::operator ++ (int)
{
    return _counter++;
}

inline AtomicCounter::ValueType AtomicCounter::operator -- (int)
{
    return _counter--;
}

inline AtomicCounter::ValueType AtomicCounter::operator -- ()
{
    return --_counter;
}

inline bool AtomicCounter::operator ! () const
{
    return _counter.load() == 0;
}


#endif // _ATOMCOUNTER_H