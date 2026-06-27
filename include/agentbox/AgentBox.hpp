#pragma once

#include "Options.hpp"

#include "ConfigStore.hpp"
#include "Git.hpp"
#include "Repo.hpp"

#include "ProjectConfig.hpp"
#include "Layout.hpp"

#include "TaskStore.hpp"
#include "WorktreeManager.hpp"

namespace agentbox {

class AgentBox {
    public:
        explicit AgentBox(std::filesystem::path start_dir);

        void init(const InitOptions& opts);
        void run(const RunOptions& opts);
        void task_add(const TaskAddOptions& opts);
        void task_delete(const TaskDeleteOptions& opts);

    private:
        Repo repo_;
        ConfigStore config_store_;

        ProjectConfig config_;

        Layout layout_;

        Git git_;
};

}  // namespace agentbox
