#include "cli/cli_init.h"

void register_init_command(CLI::App &app) {
    auto* init = app.add_subcommand("init", "Init Agentbox in a git repo");

    std::string dir_to_init = ".";

    init->add_option("--dir", dir_to_init, "Target repo for initialization");

    init->callback([=]() {
        agentbox_init(dir_to_init);
    });

}