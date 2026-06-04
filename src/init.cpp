#include "init.h"


namespace fs = std::filesystem;

static int find_repo_root(fs::path &dir_path, fs::path &repo_root_path) {

    std::string command_string = std::format(
        "git -C {} rev-parse --show-toplevel 2>&1",
        dir_path.string()
    );

    FILE *fp = popen(
        command_string.c_str(),
        "r"
    );

    if (fp == NULL) {
        return -1;
    }

    std::string command_output;
    char buffer[1024];

    while (size_t bytes_read = std::fread(buffer, 1, sizeof(buffer), fp)) {
        command_output.append(buffer, bytes_read);
    }

    int st = pclose(fp);

    // std::cout << "this is what the git command gave us: " << command_output;
    // std::cout << "status code: " << st << std::endl;

    if (st == 0) {
        repo_root_path = command_output;
    }

    return st;
}

int agentbox_init(const std::string dir) {
    fs::path dir_path = dir;

    if (!fs::exists(dir_path)) {
        std::cerr << "error: path " << dir << " does not exist\n";
        return -1;
    }

    if (!fs::is_directory(dir_path)) {
        std::cerr << "error: " << dir << " is not a directory\n";
        return -1;
    }

    fs::path repo_root_path;
    int status = find_repo_root(dir_path, repo_root_path);
    if (status != 0) {
        std::cerr << "error: " << dir << " is not in a git repository\n";
        std::cerr << "hint: run \"git init\" first or pass a path inside a repository\n";
        return -1;
    }

    std::vector<fs::path> init_dirs = {
        ".agentbox/tasks",
        ".agentbox/logs",
        ".agentbox/worktrees"
    };

    std::vector<fs::path> init_files = {
        ".agentbox/config.toml"
    };

    try {
        for (fs::path &dir : init_dirs) {
            fs::create_directories(dir);
        }
    } catch (fs::filesystem_error &e) {
        std::cerr << "Filesystem error: " << e.what() << '\n';
    }

    try {
        for (fs::path &f : init_files) {
            std::ofstream(f.string());
        }
    } catch (fs::filesystem_error &e) {
        std::cerr << "Filesystem error: " << e.what() << '\n';
        return -1;
    }

    std::ofstream config(".agentbox/config.toml");

    if (!config.is_open()) {
        std::cerr << "Error opening config file.\n";
        return -1;
    }

    auto default_config = toml::table{
        { "project", toml::table{
                {"default_test", "make test"}
            }
        },
        { "workspace", toml::table{
                {"worktree_dir", ".agentbox/worktrees"},
                {"log_dir", ".agentbox/logs"},
                {"task_dir", ".agentbox/tasks"},
            }
        },
        { "agents", toml::table{
                {"codex", toml::table{
                        {"cmd", "codex"},
                        {"args", toml::array{}},
                        {"mode", "host"}
                    }
                }
            }
        },
    };

    config << default_config;

    return 0;
}