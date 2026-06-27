#include "Layout.hpp"
#include "ProjectConfig.hpp"

namespace agentbox {

Layout::Layout(const Repo& repo, const ProjectConfig& config): repo_(repo), config_(config) {}

std::filesystem::path Layout::tasks_dir() const {
    return config_.paths.tasks_dir;
}
std::filesystem::path Layout::logs_dir() const {
    return config_.paths.logs_dir;
}
std::filesystem::path Layout::worktrees_dir() const {
    return config_.paths.worktrees_dir;
}

} // namespace agentbox