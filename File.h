#ifndef _FILE_H
#define _FILE_H
#include "Foundation.h"
#include "type.h"
#include <string>
#include <vector>

class Path;

class FileImpl
{
protected:
    enum Options
    {
        OPT_FAIL_ON_OVERWRITE_IMPL = 0x01,
    };

    using FileSizeImpl = UInt64;

    FileImpl();
    FileImpl(const std::string& path);
    virtual ~FileImpl();
    void swapImpl(FileImpl& file);
    void setPathImpl(const std::string& path);
    const std::string& getPathImpl() const;
    std::string getExecutablePathImpl() const;

    bool existsImpl() const;
    bool canReadImpl() const;
    bool canWriteImpl() const;
    bool canExecuteImpl(const std::string& absolutePath) const;
    bool isFileImpl() const;
    bool isDirectoryImpl() const;
    bool isLinkImpl() const;
    bool isDeviceImpl() const;
    bool isHiddenImpl() const;
    Timestamp createdImpl() const;
    Timestamp getLastModifiedImpl() const;
    void setLastModifiedImpl(const Timestamp& ts);
    FileSizeImpl getSizeImpl() const;
    void setSizeImpl(FileSizeImpl size);
    void setWritableImpl(bool flag = true);
    void setExecutableImpl(bool flag = true);
    void copyToImpl(const std::string& path, int options = 0) const;
    void renameToImpl(const std::string& path, int options = 0);
    void linkToImpl(const std::string& path, int type) const;
    void removeImpl();
    bool createFileImpl();
    bool createDirectoryImpl();
    FileSizeImpl totalSpaceImpl() const;
    FileSizeImpl usableSpaceImpl() const;
    FileSizeImpl freeSpaceImpl() const;
    static void handleLastErrorImpl(int err, const std::string& path);
    static void handleLastErrorImpl(const std::string& path);

private:
    std::string _path;

    friend class DirectoryIteratorImpl;
	friend class LinuxDirectoryWatcherStrategy;
	friend class BSDDirectoryWatcherStrategy;
};

inline const std::string& FileImpl::getPathImpl() const
{
    return _path;
}




class Foundation_API File: private FileImpl
{
public:
    using FileSize = FileSizeImpl;

    enum LinkType
    {
        LINK_HARD     = 0,
        LINK_SYMBOLIC = 1,
    };

    enum Options
    {
        OPT_FAIL_ON_OVERWRITE = 0x01,
    };

    File();
    File(const std::string& path);
    File(const char* path);
    File(const Path& path);
    File(const File& file);

    ~File() override;
    File& operator=(const File& file);
    File& operator=(const std::string& path);
    File& operator=(const char* path);
    File& operator=(const Path& path);

    void swap(File& file) noexcept;

    const std::string& path() const;

    std::string absolutePath() const;

    bool exists() const;

    bool existsAnywhere() const;

    bool canRead() const;

    bool canWrite() const;

    bool canExecute() const;

    bool isFile() const;

    bool isLink() const;

    bool isDirectory() const;

    bool isDevice() const;

    bool isHidden() const;

    Timestamp created() const;

    Timestamp getLastModified() const;

    File& setLastModified(const Timestamp& ts);

    FileSize getSize() const;

    File& setSize(FileSize size);

    File& setWriteable(bool flag = true);

    File& setReadOnly(bool flag = true);

    File& setExecutable(bool flag = true);

    void copyTo(const std::string& path, int options = 0) const;

    void moveTo(const std::string& path, int options = 0);

    void renameTo(const std::string& path, int options = 0);

    void linkTo(const std::string& path, LinkType type = LINK_SYMBOLIC) const;

    void remove(bool recursive = false);

    bool createFile();

    bool createDirectory();

    void createDirectories();

    void list(std::vector<std::string>& files) const;

    void list(std::vector<File>& files) const;

    FileSize totalSpace() const;

    FileSize usableSpace() const;

    FileSize freeSpace() const;

    bool operator == (const File& file) const;
    bool operator != (const File& file) const;
    bool operator <  (const File& file) const;
    bool operator >  (const File& file) const;
    bool operator <= (const File& file) const;
    bool operator >= (const File& file) const;

    static void handleLastError(const std::string& path);

protected:
    void copyDirectory(const std::string& path, int options = 0) const;
};

inline const std::string& File::path() const
{
    return getPathImpl();
}

inline bool File::operator == (const File& file) const
{
    return getPathImpl() == file.getPathImpl();
}

inline bool File::operator != (const File& file) const
{
    return getPathImpl() != file.getPathImpl();
}

inline bool File::operator < (const File& file) const
{
    return getPathImpl() < file.getPathImpl();
}

inline bool File::operator > (const File& file) const
{
    return getPathImpl() > file.getPathImpl();
}

inline bool File::operator <= (const File& file) const
{
    return getPathImpl() <= file.getPathImpl();
}

inline bool File::operator >= (const File& file) const
{
    return getPathImpl() >= file.getPathImpl();
}

inline void swap(File& f1, File& f2) noexcept
{
    f1.swap(f2);
}

#endif // _FILE_H