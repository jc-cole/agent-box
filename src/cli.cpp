#include "cli.h"

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

typedef struct AddTaskOptions {
    std::string task_prompt = "";
    std::string id = "";
} AddTaskOptions;

typedef struct DeleteTaskOptions {
    std::string id = "";
} DeleteTaskOptions;

void register_task_command(CLI::App &app) {
    auto add_opts = std::make_shared<AddTaskOptions>();
    auto del_opts = std::make_shared<DeleteTaskOptions>();

    auto* task = app.add_subcommand("task");

    auto* add = task->add_subcommand("add", "Add a new task for agents to execute");

    add->add_option("--id", add_opts->id, "Specify the id of the new task");
    add->add_option("--prompt", add_opts->task_prompt, "Task prompt to be given to asignee agent for this task");

    add->callback([add_opts]() {
        agentbox_task_add(add_opts->id, add_opts->task_prompt);
    });

    auto* del = task->add_subcommand("delete", "Delete a task of specified id");

    del->add_option("--id", del_opts->id, "Specify the id of the task to delete")->required();

    del->callback([del_opts]() {
        agentbox_task_delete(del_opts->id);
    });


}

typedef struct RunOptions {
    std::string task_id = "";
    std::string agent_id = "";
} RunOptions;

void register_run_command(CLI::App &app) {
    auto* run = app.add_subcommand("run");

    auto run_opts = std::make_shared<RunOptions>();

    std::string task_id;
    run->add_option("task_id", run_opts->task_id, "Required task id")->required();
    std::string agent_id;
    run->add_option("--agent", run_opts->agent_id, "Required agent id")->required();

    run->callback([run_opts](){
        agentbox_run(run_opts->task_id, run_opts->agent_id);
    });
}