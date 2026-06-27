#pragma once

#include <filesystem>
#include "Repo.hpp"
#include "ProjectConfig.hpp"

namespace agentbox {

class Layout {
    
public:
    Layout(const Repo& repo, const ProjectConfig& config);

    std::filesystem::path tasks_dir() const;
    std::filesystem::path logs_dir() const;
    std::filesystem::path worktrees_dir() const;

private:
    const Repo& repo_;
    const ProjectConfig& config_;
};

} // namespace agentbox