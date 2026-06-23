#pragma once

#include <string>

typedef struct InitOptions {
    std::string path = ".";
} InitOptions;

typedef struct AddTaskOptions {
    std::string task_prompt = "";
    std::string id = "";
} AddTaskOptions;

typedef struct DeleteTaskOptions {
    std::string id = "";
} DeleteTaskOptions;


typedef struct RunOptions {
    std::string task_id = "";
    std::string agent_id = "";
} RunOptions;