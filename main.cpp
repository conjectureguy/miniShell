#include <vector>
#include <filesystem>
#include <readline/readline.h>
#include <readline/history.h>

std::vector<std::string> preprocess_globs(const std::vector<std::string>& tokens);
std::vector<std::string> tokenize(const std::string& line);
void run_piped(std::vector<std::string> tokens);

int main() {
    namespace fs = std::filesystem;
    while (true) {
        std::string s = "\033[32m(miniShell) \033[0m\033[1;34m" + std::string(fs::current_path()) + "\033[0m> ";
        const char* input = readline(s.c_str());
        if (!input) break;
        if (*input) add_history(input);
        std::string line(input);
        const std::vector<std::string> tokens = tokenize(line);
        const std::vector<std::string> expanded_tokens = preprocess_globs(tokens);
        run_piped(expanded_tokens);
    }
}

/*
 * DONE:
 * take input, parse it
 * fork() and execvp()
 * implement cd and exit
 * implement io redirection -> DONE using RAII style class
 * pipes
 * command history
 * handled wildcards
 *
 * REMAINING:
 * fork bomb detection file locking checking
 */
