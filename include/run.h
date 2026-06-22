# ifndef RUN_H
# define RUN_H

# include "init.h"
# include <set>
# include <filesystem>
# include <string_view>
# include "toml.hpp"
# include "worktrees.h"
# include <format>

int agentbox_run(std::string &task_id, std::string &agent_id);

# endif