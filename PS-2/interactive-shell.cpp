#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>

std::vector<std::string> split(const std::string& str, char d) {
    std::vector<std::string> res;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, d)) {
        if (!token.empty()) {
            res.push_back(token);
        }
    }
    return res;
}

int main() {
    std::string cmnd;
    std::cout << "> ";
    std::getline(std::cin, cmnd);
    while (cmnd != "exit") {
        if (cmnd.empty()) { continue; }

        std::vector<std::string> args = split(cmnd, ' ');

        std::vector<char*> c_args;
        for (std::string& arg : args) {
            c_args.push_back(arg.data());
        }
        c_args.push_back(nullptr);

        int pid = fork();
        if (pid == -1) {
            perror("fail");
            continue;
        }
        else if (pid == 0) {
            int res = execvp(c_args[0], c_args.data());
            if (res == -1) {
                perror("fail");
                exit(EXIT_FAILURE);
            }
        }
        else {
            int status;
            waitpid(pid, &status, 0);
        }

        std::cout << "> ";
        std::getline(std::cin, cmnd);
    }

    return 0;
}