#include "Repo.hpp"
#include "Error.hpp"

namespace agentbox {
    Repo::Repo(
        const Git& git
    ): git_(git) {
        root_ = git_.repo_root();
    }

    const std::filesystem::path& Repo::root() const {
        return root_;
    }

    std::filesystem::path Repo::agentbox_dir() const {
        return std::filesystem::path(root_).append(".agentbox");
    }

    std::filesystem::path Repo::config_path() const {
        return std::filesystem::path(root_)
            .append(".agentbox")
            .append("config.toml");
    }

    bool Repo::initialized() const {
        throw Error(ErrorCode::NotImplementedError, "method not implemented.");
        return false;
    }

    void Repo::require_initialized() const {
        throw Error(ErrorCode::NotImplementedError, "method not implemented.");
    }

    void Repo::require_not_initialized() const {
        throw Error(ErrorCode::NotImplementedError, "method not implemented.");
    }

}  // namespace agentbox
