#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include "Foundation.h"
#include "RefCountedObject.h"
#include <string>

enum AccessMode
{
    AM_READ = 0,
    AM_WRITE = 1,
};

class File;

class Foundation_API SharedMemoryImpl: public RefCountedObject
{
public:
    SharedMemoryImpl(const std::string& name, std::size_t size, AccessMode mode, const void* addrHint, bool server);
    SharedMemoryImpl(const File& file, AccessMode mode, const void* addrHint);

    char* begin() const;
    char* end() const;

protected:
    void map(const void* addrHint);
    void unmap();
    void close();
    ~SharedMemoryImpl() override;

private:
    SharedMemoryImpl() = default;
    SharedMemoryImpl(const SharedMemoryImpl&) = default;
    SharedMemoryImpl& operator=(const SharedMemoryImpl&) = default;

    std::size_t              _size;
    int                      _fd;
    char*                    _address;
    AccessMode               _access;
    std::string              _name;
    bool                     _fileMapped;
    bool                     _server;
};

inline char* SharedMemoryImpl::begin() const
{
    return _address;
}

inline char* SharedMemoryImpl::end() const
{
    return _address + _size;
}





class Foundation_API SharedMemory
{
public:
    SharedMemory();

    SharedMemory(const std::string& name, std::size_t size, AccessMode mode, const void* addrHint = nullptr, bool server = true);
    SharedMemory(const File& file, AccessMode mode, const void* addrHint = nullptr);
    SharedMemory(const SharedMemory& other);

    ~SharedMemory();

    SharedMemory& operator=(const SharedMemory& other);

    void swap(SharedMemory& other) noexcept;

    char* begin() const;
    char* end() const;

private:
    SharedMemoryImpl* _pImpl;
};


inline void SharedMemory::swap(SharedMemory& other) noexcept
{
    std::swap(_pImpl, other._pImpl);
}


#endif // SHARED_MEMORY_H