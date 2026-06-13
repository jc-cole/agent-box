
#include "cli/cli_init.h"
#include "cli/cli_task.h"
#include "worktrees.h"
#include <iostream>

int main(int argc, char **argv) {
    CLI::App app{"agentbox - run coding agents in controlled sandboxes"};

    register_init_command(app);
    register_task_command(app);

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {

        int exit_code = app.exit(e);

        return exit_code;
    }

    create_task_worktree("task-2", "codex");

    return 0;
}