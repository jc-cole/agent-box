#pragma once

#include <string>
#include <filesystem>

namespace agentbox {

typedef struct InitOptions {
    std::filesystem::path path = ".";
} InitOptions;

typedef struct TaskAddOptions {
    std::string task_prompt = "";
    std::string id = "";
} TaskAddOptions;

typedef struct TaskDeleteOptions {
    std::string id = "";
} TaskDeleteOptions;


typedef struct RunOptions {
    std::string task_id = "";
    std::string agent_id = "";
} RunOptions;

}  // namespace agentbox
