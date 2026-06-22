# include "run.h"

namespace fs = std::filesystem;
    
int agentbox_run(std::string &task_id, std::string &agent_id) {
    std::string repo_root;
    if (verify_init(".", repo_root, std::cerr) != 0) {
        return -1;
    }

    std::set<std::string> task_file_stems;

    fs::path tasks_path(std::format("{}/.agentbox/tasks", repo_root));

    for (const auto &entry : fs::directory_iterator(tasks_path)) {
        if (fs::is_regular_file(entry.status())) {
            task_file_stems.insert(entry.path().stem().string());
        }
    }

    if (!task_file_stems.contains(task_id)) {
        std::cerr << "error: no task with specified id\n";
    }

    std::string prompt_string;
    try {
        std::string task_config_file_path = std::format("{}/.agentbox/tasks/{}.toml", repo_root, task_id);
        auto task = toml::parse_file(task_config_file_path);

        prompt_string = task["prompt"].value_or("");

        if (prompt_string.empty()) {
            std::cerr << "warning: task prompt is empty\n";
        }

    } catch (const toml::parse_error& err) {
        std::cerr << std::format("error: failed to parse .agentbox/tasks/{}.toml\n", task_id);
        std::cerr << err << "\n";
    }


    std::string agent_launch_cmd;
    try {
        std::string config_file_path = std::format("{}/.agentbox/config.toml", repo_root);
        toml::table config = toml::parse_file(config_file_path);

        auto agent_table = config["agents"].as_table();

        if (!agent_table->contains(agent_id)) {
            std::cerr << "error: no config for agent \"" << agent_id << "\"";
            return -1;
        }

        auto agent_config = agent_table->get(agent_id)->as_table();

        if (!agent_config->contains("cmd")) {
            std::cerr << "error: no launch command specified for agent \"" << agent_id << "\"";
            return -1;
        } else {
            agent_launch_cmd = agent_config->get("cmd")->as_string()->value_or("");
        }

    } catch (const toml::parse_error& err) {
        std::cerr << "error: failed to parse config.toml\n";
        std::cerr << err << "\n";
    }

    if (create_task_worktree(task_id, agent_id) != 0) {
        std::cerr << "error: failed to create worktree for task\n";
        return -1;
    }

    std::string worktree_dir = std::format("{}/.agentbox/worktrees/{}-{}", repo_root, task_id, agent_id);

    std::string cd_and_launch_cmd = std::format("cd {}; {} \"{}\"", worktree_dir, agent_launch_cmd, prompt_string);

    std::cout << cd_and_launch_cmd << std::endl;

    int st = std::system(cd_and_launch_cmd.c_str());

    return WEXITSTATUS(st);
}