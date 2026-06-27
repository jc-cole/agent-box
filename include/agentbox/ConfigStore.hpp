#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include "toml.hpp"

#include "Repo.hpp"
#include "Options.hpp"

namespace agentbox {

struct Agent {
    std::string id;
    std::string name;
    std::string command;
    std::vector<std::string> args;
};

class ConfigStore {
    public:
        explicit ConfigStore(const Repo& repo);

        std::vector<Agent> load_agents() const;
        Agent get_agent(const std::string& agent_id) const;

        std::filesystem::path task_dir() const;
        std::filesystem::path log_dir() const;
        std::filesystem::path worktree_dir() const;

        void create_default_config(const InitOptions& opts);

    private:
        const Repo& repo_;
        const std::string log_dir_key = "log_dir";
        const std::string task_dir_key = "task_dir";
        const std::string worktree_dir_key = "worktree_dir";
        const std::filesystem::path REPO_CONFIG_PATH = ".agentbox/config.toml";
};

}  // namespace agentbox
