#include "task.h"
#include "init.h"

namespace fs = std::filesystem;

int agentbox_task_add(std::string &task_id, std::string &prompt) {
    
    std::string repo_root;
    if (verify_init(".", repo_root, std::cout) != 0) {
        return -1;
    }

    std::set<std::string> task_file_stems;

    fs::path tasks_path(std::format("{}/.agentbox/tasks", repo_root));

    if (fs::exists(tasks_path) && fs::is_directory(tasks_path)) {
        for (const auto &entry : fs::directory_iterator(tasks_path)) {
            if (fs::is_regular_file(entry.status())) {
                task_file_stems.insert(entry.path().stem().string());
            }
        }
    } else {
        std::cout << fs::exists(tasks_path) << std::endl;
        std::cout << fs::is_directory(tasks_path) << std::endl;
    }

    std::string new_task_id;
    if (task_id.empty()) {
        int task_count = 1;
        new_task_id = std::format("task-{}", task_count);
        while (task_file_stems.contains(new_task_id)) {
            task_count++;
            new_task_id = std::format("task-{}", task_count);
        }
    } else {
        if (task_file_stems.contains(task_id)) {
            std::cerr << "error: task with specified id already exists\n";
            return -1;
        }
        new_task_id = task_id;
    }

    const auto task_data = toml::table {
        {"id", new_task_id},
        {"prompt", prompt}
    };

    std::ofstream new_task(std::format("{}/.agentbox/tasks/{}.toml", repo_root, new_task_id));

    new_task << task_data;

    return 0;

}