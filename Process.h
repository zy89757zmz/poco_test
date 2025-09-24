#ifndef _PROCESS_H
#define _PROCESS_H


#include "Foundation.h"
#include "type.h"
#include "Pipe.h"
#include "RefCountedObject.h"
#include <vector>
#include <map>
#include <string>

enum ProcessOptions
{
    PROCESS_CLOSE_STDIN = 0x01,
    PROCESS_CLOSE_STDOUT = 0x02,
    PROCESS_CLOSE_STDERR = 0x04,
};

class Foundation_API ProcessHandleImpl: public RefCountedObject
{
public:
    ProcessHandleImpl(pid_t pid);
    ~ProcessHandleImpl() override;

    pid_t id() const;
    int wait() const;
    int tryWait() const;

private:
    std::atomic<pid_t> _pid;
};




class Foundation_API ProcessImpl
{
public:
    using PIDImpl = pid_t;
    using ArgsImpl = std::vector<std::string>;
    using EnvImpl = std::map<std::string, std::string>;

    static PIDImpl idImpl();
    static void timesImpl(long& userTime, long& kernelTime);
    static void timesMicrosecondsImpl(Int64& userTime, Int64& kernelTime);
    static ProcessHandleImpl* launchImpl(
        const std::string& command,
        const ArgsImpl& args,
        const std::string& initialDirectory,
        Pipe* inPipe,
        Pipe* outPipe,
        Pipe* errPipe,
        const EnvImpl& env,
        int options = 0);
    static void killImpl(ProcessHandleImpl& handle);
    static void killImpl(PIDImpl pid);
    static bool isRunningImpl(const ProcessHandleImpl& handle);
    static bool isRunningImpl(PIDImpl pid);
    static void requestTerminationImpl(PIDImpl pid);

private:
    static ProcessHandleImpl* launchByForkExecImpl(
        const std::string& command,
        const ArgsImpl& args,
        const std::string& initialDirectory,
        Pipe* inPipe,
        Pipe* outPipe,
        Pipe* errPipe,
        const EnvImpl& env,
        int options = 0);

};

class Foundation_API ProcessHandle
{
public:
    using PID = ProcessImpl::PIDImpl;
    ProcessHandle(const ProcessHandle& handle);
    ~ProcessHandle();

    ProcessHandle& operator = (const ProcessHandle& handle);

    PID id() const;
    int wait() const;
    int tryWait() const;

protected:
    ProcessHandle(ProcessHandleImpl* pImpl);

private:
    ProcessHandle();
    ProcessHandleImpl* _pImpl;
    friend class Process;
};

class Foundation_API Process: public ProcessImpl
{
public:
    using PID = ProcessImpl::PIDImpl;
    using Args = ProcessImpl::ArgsImpl;
    using Env = ProcessImpl::EnvImpl;

    static PID id();
    static void times(long& userTime, long& kernelTime);
    static void timesMicroseconds(Int64& userTime, Int64& kernelTime);
    static ProcessHandle launch(const std::string& command, const Args& args, int options = 0);
    static ProcessHandle launch(
        const std::string& command,
        const Args& args,
        const std::string& initialDirectory,
        int options = 0);
    static ProcessHandle launch(
        const std::string& command,
        const Args& args,
        Pipe* inPipe,
        Pipe* outPipe,
        Pipe* errPipe,
        int options = 0);
    static ProcessHandle launch(
        const std::string& command,
        const Args& args,
        const std::string& initialDirectory,
        Pipe* inPipe,
        Pipe* outPipe,
        Pipe* errPipe,
        int options = 0);
    static ProcessHandle launch(
        const std::string& command,
        const Args& args,
        Pipe* inPipe,
        Pipe* outPipe,
        Pipe* errPipe,
        const Env& env,
        int options = 0);
    static ProcessHandle launch(
        const std::string& command,
        const Args& args,
        const std::string& initialDirectory,
        Pipe* inPipe,
        Pipe* outPipe,
        Pipe* errPipe,
        const Env& env,
        int options = 0);

    static int wait(const ProcessHandle& handle);
    static int tryWait(const ProcessHandle& handle);
    static bool isRunning(const ProcessHandle& handle);
    static bool isRunning(PID pid);
    static void kill(ProcessHandle& handle);
    static void kill(PID pid);
    static void requestTermination(PID pid);

};


inline Process::PID Process::id()
{
    return ProcessImpl::idImpl();
}

inline void Process::times(long& userTime, long& kernelTime)
{
    ProcessImpl::timesImpl(userTime, kernelTime);
}

inline void Process::timesMicroseconds(Int64& userTime, Int64& kernelTime)
{
    ProcessImpl::timesMicrosecondsImpl(userTime, kernelTime);
}
#endif // _PROCESS_H