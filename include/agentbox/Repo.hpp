#pragma once

#include <filesystem>

class Repo {
    public:
        explicit Repo(std::filesystem::path start_dir);

        const std::filesystem::path& root() const;
        std::filesystem::path agentbox_dir() const;
        std::filesystem::path config_path() const;
        std::filesystem::path tasks_dir() const;
        std::filesystem::path worktrees_dir() const;

        bool initialized() const;
        void require_initialized() const;
        void require_not_initialized() const;

    private:
        std::filesystem::path root_;
};