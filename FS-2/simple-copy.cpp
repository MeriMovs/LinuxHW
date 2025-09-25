#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
	if(argc != 3) {
        perror("Wrong args");
        exit(EXIT_FAILURE);
    }

//-- open from --
	int fd1 = open(argv[1], O_RDONLY);
    if (fd1 == -1) {
        perror("Ошибка при открытии файла");
        exit(EXIT_FAILURE);
    }

//-- open to --
    int fd2 = open(argv[2], O_WRONLY | O_CREAT, 0664);
    if (fd2 == -1) {
        perror("Ошибка при открытии файла");
        exit(EXIT_FAILURE);
    }

// -------------
	int size = 4096;
    char bf[size];

    // -- read / write --
    ssize_t bytes_read = read(fd1, bf, size);
	while (bytes_read  > 0) {
        write(fd2, bf, bytes_read);
        bytes_read = read(fd1, bf, size);
    }
    
    close(fd1);
    close(fd2);
    return 0;
	
}
