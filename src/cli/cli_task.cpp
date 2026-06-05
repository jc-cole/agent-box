#include "cli/cli_task.h"
#include "task.h"

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