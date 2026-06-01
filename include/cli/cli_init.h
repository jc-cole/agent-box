# ifndef CLI_INIT_H
# define CLI_INIT_H

#include "init.h"
#include "CLI11.hpp"
#include <memory>

void register_init_command(CLI::App &app);

# endif