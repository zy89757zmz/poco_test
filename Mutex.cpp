#include "Mutex.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <sys/ipc.h>
#include <sys/sem.h>

//
// MutexImpl
//

MutexImpl::MutexImpl(const std::string& name):
    _name(name)
{
    std::string fileName = getFileName();

    int fd = open(fileName.c_str(), O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd == -1 && errno == ENOENT)
        fd = open(fileName.c_str(), O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd != -1)
        close(fd);
    else
        throw std::runtime_error("Failed to open mutex file: " + fileName);

    key_t key = ftok(fileName.c_str(), 'p');
    if (key == -1)
        throw std::runtime_error("Failed to generate key for mutex: " + fileName);

    _semid = semget(key, 1, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH | IPC_CREAT | IPC_EXCL);
    if (_semid >= 0)
    {
        union semun arg;
        arg.val = 1;
        semctl(_semid, 0, SETVAL, arg);
        _owned = true;
        return;
    }
    else if (errno == EEXIST)
    {
        _semid = semget(key, 1, 0);
        _owned = false;
        if (_semid >= 0)
            return;
    }

    throw std::runtime_error("Cannot create named mutex: (semget() failed) ");
}

MutexImpl::~MutexImpl()
{
    if (_owned)
    {
        union semun arg;
        semctl(_semid, 0, IPC_RMID, arg);
    }
}

void MutexImpl::lockImpl()
{
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = -1;
    op.sem_flg = SEM_UNDO;
    int err;
    do
    {
        err = semop(_semid, &op, 1);
    }
    while (err && errno == EINTR);

    if (err)
    {
        throw std::runtime_error("Cannot lock named mutex: (semop() failed) " + _name);
    }
}

bool MutexImpl::tryLockImpl()
{
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = -1;
    op.sem_flg = SEM_UNDO | IPC_NOWAIT;
    int err = semop(_semid, &op, 1);

    if (err == -1 && errno == EAGAIN)
        return false;

    if (err)
        throw std::runtime_error("Cannot try-lock named mutex: (semop() failed) " + _name);

    return true;
}

void MutexImpl::unlockImpl()
{
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = 1;
    op.sem_flg = SEM_UNDO;
    int err;
    do
    {
        err = semop(_semid, &op, 1);
    }
    while (err && errno == EINTR);

    if (err)
    {
        throw std::runtime_error("Cannot unlock named mutex: (semop() failed) " + _name);
    }
}

std::string MutexImpl::getFileName()
{
    std::string fn = "/tmp/";
    fn.append(_name);
	fn.append(".mutex");
    return fn;
}



//
// Mutex
//

Mutex::Mutex(const std::string& name):
    MutexImpl(name)
{
}

Mutex::~Mutex()
{
}