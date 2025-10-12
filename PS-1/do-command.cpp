#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <iostream>
#include <ctime>

void do_command(char** argv) {
    std::clock_t start = std::clock();

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {// - child
        int res = execvp(argv[1], &argv[1]);
        if (res == -1) {
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }
    } else { //-parent
        int status;
        waitpid(pid, &status, 0);

        std::clock_t end = std::clock();
        double duration = 1000.0 * (end - start) / CLOCKS_PER_SEC;

        std::cout << "Command completed with " << status << " exit code and took " << duration << " ms" << std::endl;

    }


}

int main(int argc, char** argv) {
    if (argc < 2) {
        perror("Wrong args");
        exit(EXIT_FAILURE);
    }

    do_command(argv);
    return 0;
}



