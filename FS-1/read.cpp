#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
	if(argc != 2) {
        perror("Wrong args");
        exit(EXIT_FAILURE);
    }

    //-- open --
	int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Ошибка при открытии файла");
        exit(EXIT_FAILURE);
    }

	int size = 4096;
    char bf[size];

    // -- read / cout --
    ssize_t bytes_read = read(fd, bf, size);
	while (bytes_read  > 0) {
        write(1, bf, bytes_read);
        bytes_read = read(fd, bf, size);
    }
    
    close(fd);  
    return 0;
	
}
