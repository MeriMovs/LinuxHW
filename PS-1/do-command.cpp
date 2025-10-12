#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        perror("Wrong args");
        exit(EXIT_FAILURE);
    }

    int res = execvp(argv[1], &argv[1]);
    if (res == -1) {
        perror("fail");
        exit(EXIT_FAILURE);
    }
}