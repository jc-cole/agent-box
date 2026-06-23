#pragma once

#include <filesystem>

class Git {
    public: 
        explicit Git(std::filesystem::path working_dir);

        std::string repo_root() const;

        void create_worktree(
            const std::filesystem::path& path,
            const std::string& branch_name
        );

        void remove_worktree(
            const std::filesystem::path& path,
            const std::string& branch_name
        );
    private:
        std::filesystem::path working_dir_;
};

