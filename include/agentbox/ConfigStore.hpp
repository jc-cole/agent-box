#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include "toml.hpp"

#include "Repo.hpp"
#include "Options.hpp"
#include "Error.hpp"
#include "ProjectConfig.hpp"

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

        ProjectConfig load() const;

        void create_default_config(const InitOptions& opts);

    private:

        void verify_config(toml::table& config_table) const;

        const Repo& repo_;

        const std::string log_dir_key = "log_dir";
        const std::string task_dir_key = "task_dir";
        const std::string worktree_dir_key = "worktree_dir";
};

}  // namespace agentbox
