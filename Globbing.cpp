#include <string>
#include <glob.h>
#include <vector>

bool needs_globbing(const std::string &tok) {
    return tok.find_first_of("*?[") != std::string::npos;
}

std::vector<std::string> expand_glob(const std::string &pattern) {
    glob_t results;
    std::vector<std::string> out;

    int flags = GLOB_TILDE | GLOB_MARK;
    int ret = glob(pattern.c_str(), flags, nullptr, &results);
    if (ret == 0) {
        for (size_t i = 0; i < results.gl_pathc; ++i) {
            out.emplace_back(results.gl_pathv[i]);
        }
    }
    globfree(&results);
    if (out.empty()) out.push_back(pattern);
    return out;
}

std::vector<std::string> preprocess_globs(const std::vector<std::string>& tokens) {
    std::vector<std::string> expanded;
    for (auto &token : tokens) {
        if (needs_globbing(token)) {
            auto matches = expand_glob(token);
            expanded.insert(expanded.end(), matches.begin(), matches.end());
        } else {
            expanded.push_back(token);
        }
    }
    return expanded;
}
