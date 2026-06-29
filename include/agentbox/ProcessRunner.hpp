#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <stdexcept>
#include <format>
#include <system_error>
#include <array>
#include <poll.h>


#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

namespace agentbox {

struct ProcessResult {
    int exit_code;
    std::string stdout_text;
    std::string stderr_text;

    bool ok() const {
        return exit_code == 0;
    }
};

class ProcessError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class ProcessRunner {
public:
    ProcessResult run(
        const std::vector<std::string>& args,
        const std::filesystem::path& cwd
    ) const;

private:
    void read_stdout_stderr(
        int stdout_fd,
        int stderr_fd,
        std::string& stdout_text,
        std::string& stderr_text
    ) const;
};

} // namespace agentbox