# include "worktrees.h"
# include "init.h"

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

    FILE *fp = popen(
        worktree_branch_cmd.c_str(),
        "r"
    );

    if (fp == NULL) {
        return -1;
    }

    int st = WEXITSTATUS(pclose(fp));

    if (st == 0) {
        return 0;
    }

    std::string worktree_no_branch_cmd = std::format(
        "git worktree add {} {} 2>&1",
        target_branch,
        worktrees_dir
    );

    fp = popen(
        worktree_branch_cmd.c_str(),
        "r"
    );

    if (fp == NULL) {
        return -1;
    }

    st = WEXITSTATUS(pclose(fp));

    if (st == 0) {
        return 0;
    }

    return -1;
}