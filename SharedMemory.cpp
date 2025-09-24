#include "SharedMemory.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <stdexcept>
#include <unistd.h>

//
//SharedMemoryImpl
//

SharedMemoryImpl::SharedMemoryImpl(const std::string& name, std::size_t size, AccessMode mode, const void* addrHint, bool server) :
    _size(size),
    _fd(-1),
    _address(nullptr),
    _access(mode),
    _name("/"),
    _fileMapped(false),
    _server(server)
{
    _name.append(name);

    int flags = _server ? O_CREAT : 0;
    if (_access == AM_WRITE)
        flags |= O_RDWR;
    else
        flags |= O_RDONLY;

    _fd = shm_open(_name.c_str(), flags, S_IRUSR | S_IWUSR);
    if (_fd == -1)
    {
        throw std::runtime_error("Failed to open shared memory: " + _name);
    }

    if (_server && -1 == ftruncate(_fd, _size))
    {
        ::close(_fd);
        _fd = -1;
        shm_unlink(_name.c_str());
        throw std::runtime_error("Failed to set size for shared memory: " + _name);
    }
    map(addrHint);
}

SharedMemoryImpl::SharedMemoryImpl(const File& file, AccessMode mode, const void* addrHint) :
    _size(0),
    _fd(-1),
    _address(nullptr),
    _access(mode),
    _name(file.path()),
    _fileMapped(true),
    _server(false)
{
    if (!file.exists() || !file.isFile())
    {
        throw std::runtime_error("Invalid file for shared memory: " + file.path());
    }

    _size = file.getSize();
    int flags = O_RDONLY;
    if (_access == AM_WRITE)
        flags = O_RDWR;
    _fd = open(_name.c_str(), flags);
    if (_fd == -1)
    {
        throw std::runtime_error("Failed to open file for shared memory: " + _name);
    }
    map(addrHint);
}

SharedMemoryImpl::~SharedMemoryImpl()
{
    unmap();
    close();
}

void SharedMemoryImpl::map(const void* addrHint)
{
    int access = PROT_READ;
    if (_access == AM_WRITE)
        access |= PROT_WRITE;

    void* addr = mmap(const_cast<void*>(addrHint), _size, access, MAP_SHARED, _fd, 0);
    if (addr == MAP_FAILED)
    {
        throw std::runtime_error("Failed to map shared memory: " + _name);
    }

    _address = static_cast<char*>(addr);
}

void SharedMemoryImpl::unmap()
{
    if (_address)
    {
        munmap(_address, _size);
    }
}

void SharedMemoryImpl::close()
{
    if (_fd != -1)
    {
        ::close(_fd);
        _fd = -1;
    }
    if (!_fileMapped && _server)
    {
        shm_unlink(_name.c_str());
    }
}

//
//SharedMemory
//

SharedMemory::SharedMemory() :
    _pImpl(nullptr)
{
}

SharedMemory::SharedMemory(const std::string& name, std::size_t size, AccessMode mode, const void* addrHint, bool server) :
    _pImpl(new SharedMemoryImpl(name, size, mode, addrHint, server))
{
}

SharedMemory::SharedMemory(const File& file, AccessMode mode, const void* addrHint) :
    _pImpl(new SharedMemoryImpl(file, mode, addrHint))
{
}

SharedMemory::SharedMemory(const SharedMemory& other) :
    _pImpl(other._pImpl)
{
    if (_pImpl)
        _pImpl->duplicate();
}

SharedMemory::~SharedMemory()
{
    if (_pImpl)
        _pImpl->release();
}

SharedMemory& SharedMemory::operator=(const SharedMemory& other)
{
    SharedMemory tmp(other);
    swap(tmp);
    return *this;
}

char* SharedMemory::begin() const
{
    if (_pImpl)
        return _pImpl->begin();

    return nullptr;
}

char* SharedMemory::end() const
{
    if (_pImpl)
        return _pImpl->end();

    return nullptr;
}
