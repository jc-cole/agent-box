
#include "cli/cli_init.h"
#include <iostream>

int main(int argc, char **argv) {
    CLI::App app{"agentbox - run coding agents in controlled sandboxes"};

    register_init_command(app);

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {

        int exit_code = app.exit(e);

        return exit_code;
    }

    return 0;
}