#include "Pipe.h"
#include <unistd.h>
#include <stdexcept>
#include <string.h>

//
// Pipe
//

Pipe::Pipe():
    _pImpl(new PipeImpl())
{
}

Pipe::Pipe(const Pipe& pipe):
    _pImpl(pipe._pImpl)
{
    _pImpl->duplicate();
}

Pipe::~Pipe()
{
    _pImpl->release();
}

Pipe& Pipe::operator=(const Pipe& pipe)
{
    if (this != &pipe)
    {
        _pImpl->release();
        _pImpl = pipe._pImpl;
        _pImpl->duplicate();
    }
    return *this;
}

void Pipe::close(CloseMode mode)
{
    switch (mode)
    {
    case CLOSE_READ:
        _pImpl->closeRead();
        break;
    case CLOSE_WRITE:
        _pImpl->closeWrite();
        break;
    default:
        _pImpl->closeRead();
        _pImpl->closeWrite();
        break;
    }
}

//
// PipeImpl
//

PipeImpl::PipeImpl()
{
    int fds[2];
    int rc = pipe(fds);
    if (rc != 0)
    {
        throw std::runtime_error("Failed to create pipe: " + std::string(strerror(errno)));
    }
    _readfd = fds[0];
    _writefd = fds[1];
}

PipeImpl::~PipeImpl()
{
    closeRead();
    closeWrite();
}

int PipeImpl::writeBytes(const void* buffer, int length)
{
    if (_writefd < 0)
    {
        throw std::runtime_error("Write handle is closed");
    }

    int n;
    do
    {
        n = write(_writefd, buffer, length);
    } while (n < 0 && errno == EINTR);

    if (n >= 0)
        return n;
    else
        throw std::runtime_error("Failed to write to pipe: " + std::string(strerror(errno)));
}

int PipeImpl::readBytes(void* buffer, int length)
{
    if (_readfd < 0)
    {
        throw std::runtime_error("Read handle is closed");
    }

    int n;
    do
    {
        n = read(_readfd, buffer, length);
    } while (n < 0 && errno == EINTR);

    if (n >= 0)
        return n;
    else
        throw std::runtime_error("Failed to read from pipe: " + std::string(strerror(errno)));
}

PipeImpl::Handle PipeImpl::readHandle() const
{
    return _readfd;
}

PipeImpl::Handle PipeImpl::writeHandle() const
{
    return _writefd;
}

void PipeImpl::closeRead()
{
    if (_readfd >= 0)
    {
        close(_readfd);
        _readfd = -1;
    }
}

void PipeImpl::closeWrite()
{
    if (_writefd >= 0)
    {
        close(_writefd);
        _writefd = -1;
    }
}