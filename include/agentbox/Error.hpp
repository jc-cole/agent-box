#pragma once

#include <stdexcept>
#include <string>
#include <utility>
#include <format>

namespace agentbox {

enum class ErrorCode {
    NotGitRepo,
    NotInitialized,
    AlreadyInitialized,

    ProcessSpawnFailed,
    ProcessNonZeroExit,
    GitCommandFailed,

    ConfigReadFailed,
    ConfigParseFailed,
    TaskNotFound,
    TaskAlreadyExists,

    FilesystemError,
    InternalError,
    NotImplementedError
};

class Error : public std::runtime_error {
    public:
        Error(ErrorCode code, std::string message): 
            std::runtime_error(std::move(message)), 
            code_(code) {}

        ErrorCode code() const noexcept {
            return code_;
        }

    private:
        ErrorCode code_;
};

}  // namespace agentbox
