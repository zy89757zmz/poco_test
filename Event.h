#ifndef _EVENT_H
#define _EVENT_H

#include "Foundation.h"
#include <string>

union semun
{
    int                         val;
    struct semid_ds*            buf;
    unsigned short int*         array;
    struct seminfo*             __buf;
};


class Foundation_API EventImpl
{
protected:
    EventImpl(const std::string& name);
    ~EventImpl();

    void setImpl();
    void waitImpl();

private:
    std::string getFileName();

    std::string _name;
    int _semid;
    bool _createdId;
};

class Foundation_API Event : private EventImpl
{
public:
    Event(const std::string& name);
    ~Event();

    void set();

    void wait();

private:
    Event() = default;
    Event(const Event&) = default;
    Event& operator=(const Event&) = default;
};

inline void Event::set()
{
    setImpl();
}

inline void Event::wait()
{
    waitImpl();
}


#endif // _EVENT_H