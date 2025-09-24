#include "Process.h"
#include <sstream>
#include <unistd.h>
#include <sys/resource.h>
#include <string.h>
#include <sys/wait.h>

std::vector<char> getEnvironmentVariablesBuffer(const Process::Env& env)
{
    std::vector<char> envChars;
    for (const auto& pair : env)
    {
        std::string entry = pair.first + "=" + pair.second + "\0";
        envChars.insert(envChars.end(), entry.begin(), entry.end());
    }
    envChars.push_back('\0'); // Null-terminate the environment variables
    return envChars;
}

//
// ProcessHandleImpl
//

ProcessHandleImpl::ProcessHandleImpl(pid_t pid):
    _pid(pid)
{
}

ProcessHandleImpl::~ProcessHandleImpl()
{
}

pid_t ProcessHandleImpl::id() const
{
    return _pid;
}

int ProcessHandleImpl::wait() const
{
    int status;
    int rc;
    do
    {
        rc = waitpid(_pid, &status, 0);
    }
    while (rc < 0 && errno == EINTR);

    if (rc != _pid)
    {
        std::ostringstream message;
        message <<  "Cannot wait for process : " << _pid;
        throw std::invalid_argument(message.str());
    }

    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    else
        return 256 + WTERMSIG(status);
}

int ProcessHandleImpl::tryWait() const
{
    int status;
    int rc;
    do
    {
        rc = waitpid(_pid, &status, WNOHANG);
    }
    while (rc < 0 && errno == EINTR);

    if (rc == 0)
        return -1;

    if (rc != _pid)
    {
        std::ostringstream message;
        message <<  "Cannot wait for process : " << std::to_string(_pid);
        throw std::invalid_argument(message.str());
    }

    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    else
        return 256 + WTERMSIG(status);
}


//
// ProcessHandle
//

ProcessHandle::ProcessHandle(const ProcessHandle& handle):
    _pImpl(handle._pImpl)
{
    _pImpl->duplicate();
}

ProcessHandle::~ProcessHandle()
{
    _pImpl->release();
}

ProcessHandle::ProcessHandle(ProcessHandleImpl* pImpl):
    _pImpl(pImpl)
{
    if (!_pImpl)
    {
        std::string errorMessage("NULL pointer: _pImpl");
        std::ostringstream message;
        message << errorMessage << " [in file \"" << __FILE__ << "\", line " << __LINE__ << "]";
        throw std::invalid_argument(message.str());
    }
}

ProcessHandle& ProcessHandle::operator=(const ProcessHandle& handle)
{
    if (this != &handle)
    {
        _pImpl->release();
        _pImpl = handle._pImpl;
        _pImpl->duplicate();
    }
    return *this;
}

ProcessHandle::PID ProcessHandle::id() const
{
    return _pImpl->id();
}

int ProcessHandle::wait() const
{
    return _pImpl->wait();
}

int ProcessHandle::tryWait() const
{
    return _pImpl->tryWait();
}

//
// Process
//

ProcessHandle Process::launch(const std::string& command, const Args& args, int options)
{
    std::string initialDirectory;
    Env env;
    return ProcessHandle(launchImpl(command, args, initialDirectory, 0, 0, 0, env, options));
}

ProcessHandle Process::launch(const std::string& command, const Args& args, const std::string& initialDirectory, int options)
{
    Env env;
    return ProcessHandle(launchImpl(command, args, initialDirectory, 0, 0, 0, env, options));
}

ProcessHandle Process::launch(const std::string& command, const Args& args, Pipe* inPipe, Pipe* outPipe, Pipe* errPipe, int options)
{
	std::string initialDirectory;
	Env env;
	return ProcessHandle(launchImpl(command, args, initialDirectory, inPipe, outPipe, errPipe, env, options));
}

ProcessHandle Process::launch(const std::string& command, const Args& args, const std::string& initialDirectory, Pipe* inPipe, Pipe* outPipe, Pipe* errPipe, int options)
{
	Env env;
	return ProcessHandle(launchImpl(command, args, initialDirectory, inPipe, outPipe, errPipe, env, options));
}

ProcessHandle Process::launch(const std::string& command, const Args& args, Pipe* inPipe, Pipe* outPipe, Pipe* errPipe, const Env& env, int options)
{
	std::string initialDirectory;
	return ProcessHandle(launchImpl(command, args, initialDirectory, inPipe, outPipe, errPipe, env, options));
}

ProcessHandle Process::launch(const std::string& command, const Args& args, const std::string& initialDirectory, Pipe* inPipe, Pipe* outPipe, Pipe* errPipe, const Env& env, int options)
{
	return ProcessHandle(launchImpl(command, args, initialDirectory, inPipe, outPipe, errPipe, env, options));
}

int Process::wait(const ProcessHandle& handle)
{
    return handle.wait();
}

int Process::tryWait(const ProcessHandle& handle)
{
    return handle.tryWait();
}

void Process::kill(ProcessHandle& handle)
{
    killImpl(*handle._pImpl);
}

void Process::kill(PID pid)
{
    killImpl(pid);
}

bool Process::isRunning(const ProcessHandle& handle)
{
    return isRunningImpl(*handle._pImpl);
}

bool Process::isRunning(PID pid)
{
    return isRunningImpl(pid);
}

void Process::requestTermination(PID pid)
{
    requestTerminationImpl(pid);
}

//
// ProcessImpl
//
ProcessImpl::PIDImpl ProcessImpl::idImpl()
{
    return getpid();
}

void ProcessImpl::timesImpl(long& userTime, long& kernelTime)
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    userTime = usage.ru_utime.tv_sec;
    kernelTime = usage.ru_stime.tv_sec;
}

void ProcessImpl::timesMicrosecondsImpl(Int64& userTime, Int64& kernelTime)
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    userTime = static_cast<Int64>(usage.ru_utime.tv_sec) * 1000000 + usage.ru_utime.tv_usec;
    kernelTime = static_cast<Int64>(usage.ru_stime.tv_sec) * 1000000 + usage.ru_stime.tv_usec;
}

ProcessHandleImpl* ProcessImpl::launchImpl(
    const std::string& command,
    const ArgsImpl& args,
    const std::string& initialDirectory,
    Pipe* inPipe,
    Pipe* outPipe,
    Pipe* errPipe,
    const EnvImpl& env,
    int options)
{
    return launchByForkExecImpl(command, args, initialDirectory, inPipe, outPipe, errPipe, env, options);
}

ProcessHandleImpl* ProcessImpl::launchByForkExecImpl(
    const std::string& command,
    const ArgsImpl& args,
    const std::string& initialDirectory,
    Pipe* inPipe,
    Pipe* outPipe,
    Pipe* errPipe,
    const EnvImpl& env,
    int options)
{
    const long CLOSE_FD_MAX = 100000;

    do
    {
        std::vector<char> envChars = getEnvironmentVariablesBuffer(env);
        std::vector<char*> argv(args.size() + 2);
        int i = 0;
        argv[i++] = const_cast<char*>(command.c_str());
        for (const auto& a: args)
        {
            argv[i++] = const_cast<char*>(a.c_str());
        }
        argv[i] = NULL;

        const char* pInitialDirectory = initialDirectory.empty() ? NULL : initialDirectory.c_str();

        int pid = fork();
        if (pid < 0)
        {
            std::ostringstream message;
            message <<  "Cannot fork process: " << strerror(errno);
            throw std::invalid_argument(message.str());
        }
        else if (pid == 0)
        {
            if (pInitialDirectory)
            {
                if (chdir(pInitialDirectory) != 0)
                {
                    break;
                }
            }

            char* p = &envChars[0];
            while (*p)
            {
                putenv(p);
                while (*p) ++p; // Move to the end of the string
                ++p; // Move to the next string
            }

            if (inPipe)
            {
                dup2(inPipe->readHandle(), STDIN_FILENO);
                inPipe->close(Pipe::CLOSE_BOTH);
            }
            if (options & PROCESS_CLOSE_STDIN)
            {
                close(STDIN_FILENO);
            }

            if (outPipe) dup2(outPipe->writeHandle(), STDOUT_FILENO);
            if (errPipe) dup2(errPipe->writeHandle(), STDERR_FILENO);
            if (outPipe) outPipe->close(Pipe::CLOSE_BOTH);
            if (options & PROCESS_CLOSE_STDOUT) close(STDOUT_FILENO);
            if (errPipe) errPipe->close(Pipe::CLOSE_BOTH);
            if (options & PROCESS_CLOSE_STDERR) close(STDERR_FILENO);

            long fdMax = sysconf(_SC_OPEN_MAX);
            if (fdMax > CLOSE_FD_MAX) fdMax = CLOSE_FD_MAX;
            for (long j = 3; j < fdMax; ++j)
            {
                close(j);
            }

            execvp(argv[0], &argv[0]);
            break;
        }

        if (inPipe) inPipe->close(Pipe::CLOSE_READ);
        if (outPipe) outPipe->close(Pipe::CLOSE_WRITE);
        if (errPipe) errPipe->close(Pipe::CLOSE_WRITE);
        return new ProcessHandleImpl(pid);

    } while (false);

    _exit(72);
}

void ProcessImpl::killImpl(ProcessHandleImpl& handle)
{
    killImpl(handle.id());
}

void ProcessImpl::killImpl(PIDImpl pid)
{
    if (kill(pid, SIGKILL) != 0)
    {
        switch (errno)
        {
        case ESRCH:
            throw std::invalid_argument("cannot kill, process does not exist : " + std::to_string(pid));
        case EPERM:
            throw std::invalid_argument("cannot kill, permission denied to kill process : " + std::to_string(pid));
        default:
            throw std::invalid_argument("cannot kill process : " + std::to_string(pid) + ": " + strerror(errno));
        }
    }
}

bool ProcessImpl::isRunningImpl(const ProcessHandleImpl& handle)
{
    return isRunningImpl(handle.id());
}

bool ProcessImpl::isRunningImpl(PIDImpl pid)
{
    if (kill(pid, 0) == 0)
    {
        return true; // Process exists
    }
    else if (errno == ESRCH)
    {
        return false; // Process does not exist
    }
    else if (errno == EPERM)
    {
        return true; // Process exists, but we don't have permission to signal it
    }
    else
    {
        std::ostringstream message;
        message << "Cannot check if process is running: " << strerror(errno);
        throw std::invalid_argument(message.str());
    }
}

void ProcessImpl::requestTerminationImpl(PIDImpl pid)
{
    if (kill(pid, SIGINT) != 0)
    {
        switch (errno)
        {
        case ESRCH:
            throw std::invalid_argument("cannot terminate, process does not exist : " + std::to_string(pid));
        case EPERM:
            throw std::invalid_argument("cannot terminate, permission denied to terminate process : " + std::to_string(pid));
        default:
            throw std::invalid_argument("cannot terminate, request termination of process : " + std::to_string(pid) + ": " + strerror(errno));
        }
    }
}
