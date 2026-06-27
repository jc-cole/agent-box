#include "ConfigStore.hpp"
#include "Error.hpp"
#include "ProjectConfig.hpp"
#include "Repo.hpp"
#include "toml.hpp"
#include <filesystem>

namespace agentbox {

    ConfigStore::ConfigStore(const Repo& repo) : repo_(repo) {}

    ProjectConfig ConfigStore::load() const {

        toml::table config_table;

        try {
            config_table = toml::parse_file(repo_.config_path().string());
        } catch (const toml::parse_error& err) {
            throw Error(ErrorCode::ConfigParseFailed, err.what());
        }


        verify_config(config_table);
    }

    void ConfigStore::verify_config(toml::table& config_table) const {
        std::filesystem::path log_path = config_table[log_dir_key].value_or("");

        if (!std::filesystem::exists(log_path)) {
            throw Error(
                ErrorCode::ConfigReadFailed, 
                std::format(
                    "Failed to locate log directory \"{}\" from required config \"{}\"", 
                    log_path.string(), 
                    log_dir_key
                )
            );
        }

        std::filesystem::path task_path = config_table[task_dir_key].value_or("");

        if (!std::filesystem::exists(task_path)) {
            throw Error(
                ErrorCode::ConfigReadFailed, 
                std::format(
                    "Failed to locate task directory \"{}\" from required config \"{}\"", 
                    task_path.string(), 
                    task_dir_key
                )
            );
        }

        std::filesystem::path worktree_path = config_table[worktree_dir_key].value_or("");

        if (!std::filesystem::exists(worktree_path)) {
            throw Error(
                ErrorCode::ConfigReadFailed, 
                std::format(
                    "Failed to locate worktree directory \"{}\" from required config \"{}\"", 
                    worktree_path.string(), 
                    worktree_dir_key
                )
            );
        }
    }

}  // namespace agentbox
