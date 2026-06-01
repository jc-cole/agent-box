#include "cli/cli_init.h"

typedef struct InitOptions {
    std::string path = ".";
} InitOptions;

void register_init_command(CLI::App &app) {
    auto opts = std::make_shared<InitOptions>();

    auto* init = app.add_subcommand("init", "Init Agentbox in a git repo");

    init->add_option("--dir", opts->path, "Target repo for initialization");

    init->callback([opts]() {
        agentbox_init(opts->path);
    });

}