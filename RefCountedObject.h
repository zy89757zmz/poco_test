#ifndef _REFCOUNTEDOBJECT_H
#define _REFCOUNTEDOBJECT_H
#include "Foundation.h"
#include "AtomicCounter.h"

class Foundation_API RefCountedObject
{
public:
    RefCountedObject();

    void duplicate() const;
    void release() const noexcept;

    int referenceCount() const;

protected:
    virtual ~RefCountedObject();

private:
    RefCountedObject(const RefCountedObject&);
    RefCountedObject& operator = (const RefCountedObject&);

    mutable AtomicCounter _counter;
};

inline int RefCountedObject::referenceCount() const
{
    return _counter.value();
}

inline void RefCountedObject::duplicate() const
{
    ++_counter;
}

inline void RefCountedObject::release() const noexcept
{
    if (--_counter == 0)
    {
        delete this;
    }
}
#endif // _REFCOUNTEDOBJECT_H