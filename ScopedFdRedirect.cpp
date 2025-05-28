//
// Created by rahul on 5/23/25.
//

#include "ScopedFdRedirect.h"

#include <fcntl.h>
#include <stdexcept>
#include <unistd.h>

ScopedFdRedirect::ScopedFdRedirect(std::vector<std::string> &tokens)
    : m_saved_in_fd(dup(STDIN_FILENO)),
    m_saved_out_fd(dup(STDOUT_FILENO)),
    m_saved_err_fd(dup(STDERR_FILENO)),
    m_changed_in(false),
    m_changed_out(false),
    m_changed_err(false)
{
    if (m_saved_in_fd  < 0 || m_saved_out_fd < 0 || m_saved_err_fd < 0)
    {
      throw std::runtime_error("dup() failed to backup std fds");
    }
    for (int i = 0; i < tokens.size(); ) {
        if (tokens[i] == "<" && i+1 < tokens.size()) {
            const int in_fd = open(tokens[i+1].c_str(), O_RDONLY);
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
            tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
            m_changed_in = true;
        }
        else if (tokens[i] == ">" && i+1 < tokens.size()) {
            const int out_fd = open(tokens[i+1].c_str(), O_WRONLY);
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
            tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
            m_changed_out = true;
        }
        else if (tokens[i] == "2>" && i+1 < tokens.size()) {
            const int err_fd = open(tokens[i+1].c_str(), O_WRONLY);
            dup2(err_fd, STDERR_FILENO);
            close(err_fd);
            tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
            m_changed_err = true;
        }
        else i++;
    }
}

ScopedFdRedirect::~ScopedFdRedirect() {
    if (m_changed_in) {
        dup2(m_saved_in_fd, STDIN_FILENO);
    }
    if (m_changed_out) {
        dup2(m_saved_out_fd, STDOUT_FILENO);
    }
    if (m_changed_err) {
        dup2(m_saved_err_fd, STDERR_FILENO);
    }
    close(m_saved_in_fd);
    close(m_saved_out_fd);
    close(m_saved_err_fd);
}

