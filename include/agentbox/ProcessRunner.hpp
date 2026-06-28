#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <stdexcept>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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
};

} // namespace agentbox