# ifndef TASK_H
# define TASK_H

# include <string>
# include "toml.hpp"
# include <fstream>
# include <filesystem>
# include <set>
# include <format>
# include "worktrees.h"

int agentbox_task_add(std::string &task_id, std::string &prompt);

int agentbox_task_delete(std::string &task_id);

# endif