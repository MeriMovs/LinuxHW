#include <iostream>
#include <unistd.h>
#include <fcntl.h>


int main() {
    int rwPipefd[2];
    int wrPipefd[2];

    int rwPipe = pipe(rwPipefd);
    int wrPipe = pipe(wrPipefd);

    pid_t child = fork();
 
    if(child == -1){
        std::cerr << "Failed to create a child: \n";
        exit(EXIT_FAILURE);
    }

    if(child == 0){ //child
        std::cout << "Child " << std::endl;

        close(rwPipefd[0]);
        close(wrPipefd[1]);

    }
    else if(child > 0){ // parent
        std::cout << "parent" << std::endl;

        close(rwPipefd[1]);
        close(wrPipefd[0]);
    }

    return 0;
}