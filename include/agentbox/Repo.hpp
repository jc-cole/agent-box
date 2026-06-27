#pragma once

#include "Git.hpp"
#include "Error.hpp"
#include <filesystem>

namespace agentbox {

class Repo {
    public:
        explicit Repo(const Git& git);

        const std::filesystem::path& root() const;
        std::filesystem::path agentbox_dir() const;
        std::filesystem::path config_path() const;

        bool initialized() const;
        void require_initialized() const;
        void require_not_initialized() const;

    private:
        const Git& git_;
        std::filesystem::path root_;

};

}  // namespace agentbox
