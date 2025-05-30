#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> tokenize(const std::string& line) {
    std::vector<std::string> tokens;
    std::istringstream tokenStream(line);
    std::string token;
    while (tokenStream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}
