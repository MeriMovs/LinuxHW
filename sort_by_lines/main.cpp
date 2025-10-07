#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

bool isFileEnd(int fd) {
    char buffer[1];
    if (read(fd, buffer, 1) <= 0) {
        return true;
    }
    return false;
}

std::string getLine(int fd) {
    char buffer[1];
    std::string line;
    while (read(fd, buffer, 1) > 0) {
        line += buffer[0];
        if (buffer[0] == '\n') {
            return line;  
        }
    }
    return line;
}

void write_to(int fd, const std::string& str, bool del = false) {
  if(del) {
    lseek(fd, 0, SEEK_SET);
    ftruncate(fd, 0);
  }
  if(str == "\n") return;
  ssize_t bytes_written = write(fd, str.c_str(), str.size());
  if (bytes_written == -1) {
      perror("Error writing to file");
      exit(EXIT_FAILURE);
  }
}

int main() {
    int fd = open("short_text", O_RDWR);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int fd_3 = open("tmp.txt", O_RDWR | O_CREAT, 0644);
    if (fd_3 == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    /////////////////////////


    int lines_count = 2;
    bool calc_lines = true;
    std::vector<std::string> arr;
    std::string tmp;
    bool swapped = false;

    for (int i = 0; i < lines_count - 1; i++) {
        swapped = false;

        lseek(fd, 0, SEEK_SET);
        lseek(fd_3, 0, SEEK_SET);
        ftruncate(fd_3, 0);
        arr.push_back(getLine(fd));

        for (int j = 0; j < lines_count  - 1; j++) {
            arr.push_back(getLine(fd));
            tmp = arr[0];

            std::sort(arr.begin(), arr.end());
            write_to(fd_3, arr[0]);
            if(tmp != arr[0]) {
                swapped = true;
            }

            arr[0] = arr[1];
            arr.pop_back();

            if(calc_lines) {
                if (isFileEnd(fd)) {
                    calc_lines = false;
                    break;
                } else {
                    lines_count += 2;
                }
            }
        }

        write_to(fd_3, arr[0]);
        arr.pop_back();

        if(!swapped) { break;}

        std::swap(fd, fd_3);
    }

    close(fd);
    close(fd_3);
    return 0;
}
