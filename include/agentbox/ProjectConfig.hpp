#pragma once

#include <filesystem>

struct PathsConfig {
    std::filesystem::path tasks_dir;
    std::filesystem::path logs_dir;
    std::filesystem::path worktrees_dir;
};

struct ProjectConfig {
   PathsConfig paths;
   // std::vector<Agent> agents
};