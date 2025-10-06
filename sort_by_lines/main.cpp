#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>


int open_fd(int name_index) {
    char filename[20];
    snprintf(filename, sizeof(filename), "%d.txt", name_index);
    int fd = open(filename, O_RDWR);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    return fd;
}

std::string read_from(int fd) {
    std::string res;
    char bf[4096];
    ssize_t bytes_read = read(fd, bf, sizeof(bf));
    while (bytes_read > 0) {
        res.append(bf, bytes_read);
        bytes_read = read(fd, bf, sizeof(bf));
    }
    if (bytes_read == -1) {
        perror("Error reading from file");
        exit(EXIT_FAILURE);
    }

    // std::cout << "read " << res << "\n";
    return res;
}

void write_to(int fd, const std::string& str, bool del = true) {
  if(del) {
    lseek(fd, 0, SEEK_SET);
    ftruncate(fd, 0);
  }
  ssize_t bytes_written = write(fd, str.c_str(), str.size());
  if (bytes_written == -1) {
      perror("Error writing to file");
      exit(EXIT_FAILURE);
  }
}

int main() {
    int fd = open("text", O_RDWR);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char filename[20];
    int i = 0;
    snprintf(filename, sizeof(filename), "%d.txt", i);
    int fd2 = open(filename, O_WRONLY | O_CREAT, 0664);
    if (fd2 == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char buffer[1];
    std::string line;
    while (read(fd, buffer, 1) > 0) {
        line += buffer[0];
        if (buffer[0] == '\n') {
            write(fd2, line.c_str(), line.size());
            close(fd2);
            line.clear();
            snprintf(filename, sizeof(filename), "%d.txt", ++i);
            fd2 = open(filename, O_WRONLY | O_CREAT, 0664);
            if (fd2 == -1) {
                perror("Error opening file");
                exit(EXIT_FAILURE);
            }
        }
    }
    if (!line.empty()) {
        write(fd2, line.c_str(), line.size());
    }
    close(fd2);

    if (i <= 1) {
        close(fd);
        return 0;
    }

    std::vector<std::string> arr;
    int fd3, fd4;
    for (int n = 0; n < i - 1; n++) {
        for (int j = 0; j < i - n - 1; j++) {
            arr.clear();
            fd3 = open_fd(j);
            fd4 = open_fd(j + 1);
            arr.push_back(read_from(fd3));
            arr.push_back(read_from(fd4));

            std::sort(arr.begin(), arr.end());
            write_to(fd3, arr[0]);
            write_to(fd4, arr[1]);

            close(fd3);
            close(fd4);
        }
    }

// -- final file --
    std::string str;
    lseek(fd, 0, SEEK_SET);
    ftruncate(fd, 0);
    for (int j = 0; j <= i; j++) {
      fd2 = open_fd(j);
      str = read_from(fd2);

      write_to(fd, str, false);
      close(fd2);

    }

    close(fd);
    return 0;
}