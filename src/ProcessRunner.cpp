#include "ProcessRunner.hpp"
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <sched.h>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

namespace agentbox {

ProcessResult ProcessRunner::run(
    const std::vector<std::string>& args,
    const std::filesystem::path& cwd
) const {
    
    int stdout_pipe[2];
    int stderr_pipe[2];

    if (pipe(stdout_pipe) == -1) {
        std::error_code ec(errno, std::generic_category());
        throw ProcessError(ec.message());
    }

    if (pipe(stderr_pipe) == -1) {
        std::error_code ec(errno, std::generic_category());
        throw ProcessError(ec.message());
    }

    std::vector<const char*> c_string_vector;
    c_string_vector.reserve(args.size() + 1);

    for (const std::string& arg : args) {
        c_string_vector.push_back(arg.c_str());
    }

    c_string_vector.push_back(NULL);

    pid_t pid = fork();

    if (pid == -1) {
        std::error_code ec(errno, std::generic_category());
        throw ProcessError(ec.message());
    }

    if (pid == 0) {
        //child

        close(stdout_pipe[0]);
        close(stderr_pipe[0]);

        dup2(stdout_pipe[1], STDOUT_FILENO);
        dup2(stderr_pipe[1], STDERR_FILENO);

        char *const *argv = (char *const *) c_string_vector.data();

        execvp(argv[0], argv);

        std::error_code ec(errno, std::generic_category());
        throw ProcessError(ec.message());
    } else {

        close(stdout_pipe[1]);
        close(stderr_pipe[1]);

        ProcessResult result;

        read_stdout_stderr(
            stdout_pipe[0], 
            stderr_pipe[0], 
            result.stdout_text, 
            result.stderr_text
        );

        int st;
        if (waitpid(pid, &st, 0) == -1) {
            std::error_code ec(errno, std::generic_category());
            throw ProcessError(ec.message());
        }

        result.exit_code = WEXITSTATUS(st);

        return result;
    }
}

void ProcessRunner::read_stdout_stderr(
        int stdout_fd,
        int stderr_fd,
        std::string& stdout_text,
        std::string& stderr_text
    ) const {

    bool stdout_open = true;
    bool stderr_open = true;

    std::array<char, 4096> buffer{};

    while (stdout_open || stderr_open) {
        struct pollfd fds[2];

        fds[0].fd = stdout_open ? stdout_fd : -1;
        fds[0].events = POLLIN;

        fds[1].fd = stderr_open ? stderr_fd : -1;
        fds[1].events = POLLIN;

        int ready = poll(fds, 2, -1);

        if (ready == -1) {
            if (errno == EINTR) {
                continue;
            }
            std::error_code ec(errno, std::generic_category());
            throw ProcessError(ec.message());
        }

        if (stdout_open && (fds[0].revents & (POLLIN | POLLHUP))) {
            ssize_t n = read(stdout_fd, buffer.data(), buffer.size());

            if (n > 0) {
                stdout_text.append(buffer.data(), static_cast<size_t>(n));
            } else if (n == 0) {
                stdout_open = false;
                close(stdout_fd);
            } else if (errno != EINTR) {
                std::error_code ec(errno, std::generic_category());
                throw ProcessError(ec.message());
            }
        }

        if (stderr_open && (fds[1].revents & (POLLIN | POLLHUP))) {
            ssize_t n = read(stderr_fd, buffer.data(), buffer.size());

            if (n > 0) {
                stderr_text.append(buffer.data(), static_cast<size_t>(n));
            } else if (n == 0) {
                stderr_open = false;
                close(stderr_fd);
            } else if (errno != EINTR) {
                std::error_code ec(errno, std::generic_category());
                throw ProcessError(ec.message());
            }
        }
    }

}

} // namespace agentbox