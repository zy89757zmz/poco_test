#include "Event.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdexcept>
#include <sys/ipc.h>
#include <sys/sem.h>


//
// EventImpl
//

EventImpl::EventImpl(const std::string& name):
    _name(name)
{
    std::string fileName = getFileName();

    _createdId = false;
    int fd = open(fileName.c_str(), O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd == -1 && errno == ENOENT)
        fd = open(fileName.c_str(), O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd != -1)
        close(fd);
    else
        throw std::runtime_error("Failed to open event file" + fileName);

    key_t key = ftok(fileName.c_str(), 'p');
    if (key == -1)
        throw std::runtime_error("Failed to generate key for event: " + fileName);

    _semid = semget(key, 1, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH | IPC_CREAT | IPC_EXCL);
    if (_semid >= 0)
    {
        union semun arg;
        arg.val = 0; // Initialize the semaphore to 0
        semctl(_semid, 0, SETVAL, arg);
        _createdId = true;
        return;
    }
    else if (errno == EEXIST)
    {
        _semid = semget(key, 1, 0);
        _createdId = false;
        if (_semid >= 0)
            return;
    }

    throw std::runtime_error("Cannot create named event: (semget() failed) " + _name);
}

EventImpl::~EventImpl()
{
    if (_createdId)
    {
        union semun arg;
        semctl(_semid, 0, IPC_RMID, arg);
    }
}

void EventImpl::setImpl()
{
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = 1; // Increment the semaphore to signal the event
    op.sem_flg = SEM_UNDO;
    if (semop(_semid, &op, 1) != 0)
    {
        throw std::runtime_error("Cannot set named event: (semop() failed) " + _name);
    }
}

void EventImpl::waitImpl()
{
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = -1; // Wait for the semaphore to be sign
    op.sem_flg = SEM_UNDO;
    int err;
    do
    {
        err = semop(_semid, &op, 1);
    }
    while (err && errno == EINTR);
    if (err)
    {
        throw std::runtime_error("Cannot wait for named event: (semop() failed) " + _name);
    }
}

std::string EventImpl::getFileName()
{
    std::string fileName = "/tmp/" + _name + ".event";
    return fileName;
}

//
// Event
//

Event::Event(const std::string& name):
    EventImpl(name)
{
}

Event::~Event()
{
}