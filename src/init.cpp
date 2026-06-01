#include "init.h"
#include <format>

namespace fs = std::filesystem;

static int find_repo_root(fs::path &dir_path, std::string &repo_root_path) {

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
        std::cout << "error: path " << dir << " does not exist\n";
        return -1;
    }

    if (!fs::is_directory(dir_path)) {
        std::cout << "error: " << dir << " is not a directory\n";
        return -1;
    }

    std::string repo_root_path;
    int status = find_repo_root(dir_path, repo_root_path);
    if (status != 0) {
        std::cout << "error: " << dir << " is not in a git repository\n";
        std::cout << "hint: run \"git init\" first or pass a path inside a repository\n";
        return -1;
    }

    return 0;
}