#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

int main(int argc, char** argv) {
	if(argc != 2) {
        perror("Wrong args");
        exit(EXIT_FAILURE);
    }

//-- open  --
	int fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        perror("Ошибка при открытии файла");
        exit(EXIT_FAILURE);
    }


// -------------
	int size = 4096;
    char bf_read[size];
    char bf_write[size];
    for (int i = 0; i < 4096; i++) {
        bf_write[i] = '\0';
    }
    std::cout << "-";


// -- calc file size --
    ssize_t bytes_read = read(fd, bf_read, size);
    ssize_t file_size = bytes_read;
	while (bytes_read  > 0) {
        bytes_read = read(fd, bf_read, size);
        file_size += bytes_read;
    }

// --------------------


    std::cout << "-";
    lseek(fd, 0, SEEK_SET);
    while(file_size != 0) {
        if(size > file_size) {
            size = file_size;
        }
        int bytes_wr = write(fd, bf_write, size);
        file_size -= bytes_wr;
    }



    close(fd);
    return 0;
	
}
