#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
#include <unistd.h>
#include <sys/wait.h>
#include <algorithm>
#include <cstdio>
#include <readline/readline.h>
#include <readline/history.h>
#include <chrono>

#include "../include/ScopedFdRedirect.h"

namespace fs = std::filesystem;

void run_commands(const std::vector<std::string>& tokens) {
    if (tokens.empty())
        return;

    if (tokens[0] == "exit")
        exit(0);
    if (tokens[0] == "cd") {
        if (tokens.size() == 1) {
            if (const char* home = std::getenv("HOME")) {
                fs::current_path(home);
            }
            return;
        }
        const fs::path newDir = tokens[1];
        try {
            fs::current_path(newDir);
        }
        catch (const fs::filesystem_error& e) {
            std::cerr << e.what() << '\n';
        }
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::perror("fork");
    }
    else if (pid == 0) {
        // child process
        std::vector<const char*> args;
        for (const auto& token : tokens)
            args.push_back(token.c_str());
        args.push_back(nullptr);
        execvp(args[0], const_cast<char * const *>(args.data()));
        perror("execvp");
        exit(1);
    }
    else {
        waitpid(pid, nullptr, 0);
    }
}

void run_piped(std::vector<std::string> tokens) {
    auto it = std::ranges::find(tokens, std::string("|"));
    if (it == tokens.end()) {
        ScopedFdRedirect redirect(tokens);
        run_commands(tokens);
        return;
    }

    std::vector<std::string> left(tokens.begin(), it);
    std::vector<std::string> right(it + 1, tokens.end());

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        std::perror("pipe");
        return;
    }

    pid_t pid1 = fork();
    if (pid1 < 0) {
        std::perror("fork");
    }
    else if (pid1 == 0) {
        // child process
        // pipefd[0] -> for reading
        // pipefd[1] -> for writing
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        ScopedFdRedirect redirect(left);
        run_commands(left);
        exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 < 0) {
        std::perror("fork");
    }
    else if (pid2 == 0) {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        ScopedFdRedirect redirect(right);
        run_commands(right);
        exit(0);
    }
    // only parent can access this
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, nullptr, 0);
    waitpid(pid2, nullptr, 0);
}