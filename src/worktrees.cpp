# include "worktrees.h"
#include <filesystem>

namespace fs = std::filesystem;

int create_task_worktree(const std::string &task_id, const std::string &agent_id) {
    // case 1: branch and worktree already exist -> return 0
    // case 2: branch with name exists, no worktree -> make worktree off of existing branch and return 0
    // case 3: neither branch nor worktree exist -> create and return 0
    // case 4: filesystem/git error when making branch or worktree -> return -1


    std::string repo_root;
    if (verify_init(".", repo_root, std::cerr) != 0) {
        return -1;
    }
    
    std::string target_branch = std::format("{}-{}", task_id, agent_id);
    std::string worktrees_dir = std::format("{}/.agentbox/worktrees/{}", repo_root, target_branch);
    std::string current_branch = "$(git branch --show-current)";

    std::string worktree_branch_cmd = std::format(
        "git worktree add -b {} {} {} 2>&1",
        target_branch,
        worktrees_dir,
        current_branch
    );

    FILE *fp_1 = popen(
        worktree_branch_cmd.c_str(),
        "r"
    );

    if (fp_1 == NULL) {
        return -1;
    }

    int st = WEXITSTATUS(pclose(fp_1));

    if (st == 0) {
        return 0;
    }

    std::string worktree_no_branch_cmd = std::format(
        "git worktree add {} {} 2>&1",
        worktrees_dir,
        target_branch
    );

    FILE *fp_2 = popen(
        worktree_no_branch_cmd.c_str(),
        "r"
    );

    if (fp_2 == NULL) {
        return -1;
    }

    st = WEXITSTATUS(pclose(fp_2));

    if (st == 0) {
        return 0;
    }

    return -1;
}

int delete_task_worktree(const std::string &task_id, const std::string &agent_id) {
    // current version does not delete corresponding remote branch

    FILE *fp;
    int st;

    std::string repo_root;
    if (verify_init(".", repo_root, std::cerr) != 0) {
        return -1;
    }

    std::string target_branch = std::format("{}-{}", task_id, agent_id);
    std::string worktrees_dir = std::format("{}/.agentbox/worktrees/{}", repo_root, target_branch);

    if (!fs::exists(worktrees_dir)) {
        std::cerr << "delete task worktree internal error: dir " << worktrees_dir << " does not exist.";
        return -1;

    }

    fp = popen(std::format("git worktree remove --force {} 2>&1", worktrees_dir).c_str(), "r");

    if (fp == NULL) {
        std::cerr << "popen: git worktree remove failed to execute\n";
        return -1;
    }

    st = WEXITSTATUS(pclose(fp));

    if (st != 0) {
        std::cerr << "warning: failed to delete worktree at " << worktrees_dir << std::endl;
    }

    fp = popen("git branch --show-current 2>&1", "r");

    if (fp == NULL) {
        std::cerr << "popen: git branch --show-current failed to execute\n";
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

    pclose(fp);

    if (command_output == target_branch) {
        std::cout << "warning: cannot delete branch " << target_branch << " since it is currently checked out.\n";
        return 0;
    }

    fp = popen(std::format("git branch -D {}", target_branch).c_str(), "r");

    st = WEXITSTATUS(pclose(fp));

    if (st != 0) {
        std::cout << "error: failed to delete branch " << target_branch << std::endl;
        return -1;
    }

    return 0;

}