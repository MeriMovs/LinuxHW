#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
	if(argc != 2) {
        perror("Wrong args");
        exit(EXIT_FAILURE);
    }

//-- open  --
	int fd = open(argv[1], O_RDONLY);
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

// -- calc file size --
    ssize_t bytes_read = read(fd, bf_read, size);
    ssize_t file_size = bytes_read;
	while (bytes_read  > 0) {
        bytes_read = read(fd, bf_read, size);
        file_size += bytes_read;
    }

    close(fd);


// -- костыль = reopen / zero --

    fd = open(argv[1], O_WRONLY, 0644);
    if (fd == -1) {
        perror("Ошибка при открытии файла");
        exit(EXIT_FAILURE);
    }

    write(fd, bf_write, file_size);

    close(fd);
    return 0;
	
}
