#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <string>
#include <regex>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

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

std::vector<std::string> split_by_str(const std::string& str, const std::string& d) {
    std::vector<std::string> res;
    size_t pos = str.find(d);
    
    if (pos == std::string::npos) {
        res.push_back(str);
    } else {
        res.push_back(str.substr(0, pos));
        res.push_back(str.substr(pos + d.length()));
    }
    return res;
}

int run(std::string cmd, int output_fd = 1) {
    const char* char_cmd = cmd.data();
    int pid = fork();
    if (pid == 0) {
        if (output_fd != 1) {
            dup2(output_fd, 1);
            close(output_fd);
        }

        std::vector<std::string> args = split(cmd, ' ');
        std::vector<char*> argv;
        for (const auto& arg : args) {
            argv.push_back(const_cast<char*>(arg.c_str()));
        }
        argv.push_back(nullptr);
        execvp(argv[0], argv.data());
        std::cerr << "Fail execution of " << cmd << std::endl;
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Parent
        int status_code;
        if(wait(&status_code) == -1) {
            std::cerr << "Fail waiting for a child " << cmd << std::endl;
            exit(EXIT_FAILURE);
        }
        if (WIFEXITED(status_code)) {
            return WEXITSTATUS(status_code);
        }
    } else {
        std::cerr << "Fork fail "
                  << cmd << std::endl;
        exit(EXIT_FAILURE);
    }
    return 0;
}

int foo(std::string& cmnd) {
    if (cmnd.find("&&") != std::string::npos) {
            std::vector<std::string> args = split_by_str(cmnd, " && ");
            if (foo(args[0]) == 0) {
                foo(args[1]);
            }
        } else if (cmnd.find("||") != std::string::npos) {
            std::vector<std::string> args = split_by_str(cmnd, " || ");
            if (foo(args[0]) != 0) {
                foo(args[1]);
            }
        } else if (cmnd.find(';') != std::string::npos) {
            std::vector<std::string> args = split_by_str(cmnd, "; ");
            foo(args[0]);
            foo(args[1]);
        } else if (cmnd.find(">>") != std::string::npos) {
            std::vector<std::string> args = split_by_str(cmnd, " >> ");
            int fd = open(args[1].c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1) {
                std::cerr << "Failed to open file for appending: \n";
                exit(EXIT_FAILURE);
            }
            run(args[0], fd);
            close(fd);
        } else if (cmnd.find('>') != std::string::npos) {
            std::vector<std::string> args = split_by_str(cmnd, " > ");
            int fd = open(args[1].c_str(), O_WRONLY | O_CREAT, 0644);
            if (fd == -1) {
                std::cerr << "Failed to open file for appending: \n";
                exit(EXIT_FAILURE);
            }
            run(args[0], fd);
            close(fd);
        } else {
            return run(cmnd);
        }

        return 0;
}

int main() {
    std::string cmnd; // = "ls -lh && echo aaa >> a.txt; pwd; cat a.txt";
    std::cout << "> ";
    std::getline(std::cin, cmnd);
    while (cmnd != "exit") {
        if (cmnd.empty()) { continue; }

        foo(cmnd);

        std::cout << "> ";
        std::getline(std::cin, cmnd);
    }

    return 0;
}
