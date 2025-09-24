#include "File.h"
#include <stdexcept>


//
// FileImpl
//

//
// File
//

File::File()
{
}

File::File(const std::string& path) : FileImpl(path)
{
}

File::File(const char* path) : FileImpl(std::string(path))
{
}

File::File(const Path& path) : FileImpl(path.toString())
{
}

File::File(const File& file) : FileImpl(file.getPathImpl())
{
}

File::~File()
{
}

File& File::operator = (const File& file)
{
    setPathImpl(file.getPathImpl());
    return *this;
}

File& File::operator = (const std::string& path)
{
    setPathImpl(path);
    return *this;
}

File& File::operator = (const char* path)
{
    if (!(path))
    {
        throw std::runtime_error("NULL pointer: " + std::string(__FILE__) + ":" + std::to_string(__LINE__));
    }
    setPathImpl(path);
    return *this;
}

File& File::operator = (const Path& path)
{
    setPathImpl(path.toString());
    return *this;
}
