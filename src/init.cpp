#include "init.h"


namespace fs = std::filesystem;

static const std::vector<fs::path> init_dirs = {
    ".agentbox/tasks",
    ".agentbox/logs",
    ".agentbox/worktrees"
};

static const fs::path config_file = ".agentbox/config.toml";

const auto default_config = toml::table{
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


static int find_repo_root(const fs::path &dir_path, fs::path &repo_root_path) {

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

    if (!command_output.empty() && command_output.back() == '\n') {
        command_output.pop_back();
    }

    int st = pclose(fp);


    if (st == 0) {
        repo_root_path = command_output;
    }

    return st;
}

static int verify_in_repo(const fs::path &dir, fs::path &repo_root_path, std::ostream &output_stream) {

    if (!fs::exists(dir)) {
        output_stream << "error: path " << dir << " does not exist\n";
        return -1;
    }

    if (!fs::is_directory(dir)) {
        output_stream << "error: " << dir << " is not a directory\n";
        return -1;
    }

    int status = find_repo_root(dir, repo_root_path);
    if (status != 0) {
        output_stream << "error: " << dir << " is not in a git repository\n";
        output_stream << "hint: run \"git init\" first or pass a path inside a repository\n";
        return -1;
    }

    return 0;
}

int verify_init(const std::string dir, std::ostream &output_stream) {
    std::string repo_root;
    return verify_init(dir, repo_root, output_stream);
}

int verify_init(const std::string dir, std::string &repo_root, std::ostream &output_stream) {
    fs::path dir_path = dir;

    fs::path repo_root_path;
    std::ostream cnull(nullptr); 
    if (verify_in_repo(dir, repo_root_path, cnull) != 0) {
        output_stream << "error: agentbox has not been initialized in this git repository nor a parent's\n";
        output_stream << "hint: run \"agentbox init\" in a valid git repository\n";
        return -1;
    }

    repo_root = repo_root_path.string();

    for (const fs::path &dir : init_dirs) {
        if (!fs::exists(dir)) {
            output_stream << "error: agentbox has not been initialized in this git repository nor a parent's\n";
            output_stream << "hint: run \"agentbox init\" in a valid git repository\n";
            return -1;
        }
    }

    if (!fs::exists(config_file)) {
        output_stream << "error: agentbox has not been initialized in this git repository nor a parent's\n";
        output_stream << "hint: run \"agentbox init\" in a valid git repository\n";
        return -1;
    }

    return 0;
}

int agentbox_init(const std::string dir) {
    fs::path dir_path = dir;

    fs::path repo_root_path;
    if (verify_in_repo(dir, repo_root_path, std::cerr) != 0) {
        return -1;
    }

    std::ostream cnull(nullptr); 
    if (verify_init(dir_path, cnull) == 0) {
        std::cout << "error: agentbox has already been initialized in this repo\n";
        std::cout << "hint: to re-initialize and delete existing metadata, delete \".agentbox\" first\n";
    }


    try {
        for (const fs::path &dir : init_dirs) {
            fs::create_directories(dir);
        }
    } catch (fs::filesystem_error &e) {
        std::cerr << "Filesystem error: " << e.what() << '\n';
    }

    std::ofstream config_stream(config_file);

    if (!config_stream.is_open()) {
        std::cerr << "error: failed to open config file\n";
        return -1;
    }

    config_stream << default_config << '\n';

    return 0;
}