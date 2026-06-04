# ifndef INIT_H
# define INIT_H

# include <iostream>
# include <string>
# include <filesystem>
# include <stdio.h>
# include <format>
# include <vector>
# include <fstream>
# include "toml.hpp"

int verify_init(const std::string dir, std::ostream &output_stream);

int agentbox_init(const std::string dir);

# endif