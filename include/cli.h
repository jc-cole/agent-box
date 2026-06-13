# ifndef CLI_H
# define CLI_H

#include "init.h"
#include "task.h"
#include <CLI11.hpp>
#include <memory>

void register_init_command(CLI::App &app);

void register_task_command(CLI::App &app);

# endif