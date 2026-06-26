#pragma once

#include <stdexcept>
#include <string>
#include <optional>

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
    InternalError
};

class AgentBoxError : public std::runtime_error {
    public:
        AgentBoxError(ErrorCode code, std::string message)
            : std::runtime_error(std::move(message)), code_(code) {}

        ErrorCode code() const {
            return code_;
        }

    private:
        ErrorCode code_;
};