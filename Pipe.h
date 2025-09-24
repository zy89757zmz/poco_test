#ifndef _PIPE_H
#define _PIPE_H

#include "Foundation.h"
#include "RefCountedObject.h"

class Foundation_API PipeImpl : public RefCountedObject
{
public:
    using Handle = int;

    PipeImpl();
    ~PipeImpl() override;
    int writeBytes(const void* buffer, int length);
    int readBytes(void* buffer, int length);
    Handle readHandle() const;
    Handle writeHandle() const;
    void closeRead();
    void closeWrite();

private:
    int _readfd;
    int _writefd;
};

class Foundation_API Pipe
{
public:
    using Handle = PipeImpl::Handle;

    enum CloseMode
    {
        CLOSE_READ = 0x01,
        CLOSE_WRITE = 0x02,
        CLOSE_BOTH = 0x03
    };

    Pipe();
    Pipe(const Pipe& pipe);

    ~Pipe();
    Pipe& operator = (const Pipe& pipe);
    int writeBytes(const void* buffer, int length);
    int readBytes(void* buffer, int length);
    Handle readHandle() const;
    Handle writeHandle() const;
    void close(CloseMode mode = CLOSE_BOTH);

private:
    PipeImpl* _pImpl;
};

inline int Pipe::writeBytes(const void* buffer, int length)
{
    return _pImpl->writeBytes(buffer, length);
}

inline int Pipe::readBytes(void* buffer, int length)
{
    return _pImpl->readBytes(buffer, length);
}

inline Pipe::Handle Pipe::readHandle() const
{
    return _pImpl->readHandle();
}

inline Pipe::Handle Pipe::writeHandle() const
{
    return _pImpl->writeHandle();
}


#endif // _PIPE_H