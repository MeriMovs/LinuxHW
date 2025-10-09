#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

bool isFileEnd(int fd) {
    int curr_pos = lseek(fd, 0, SEEK_CUR);
    char buffer[1];
    ssize_t bytes_read = read(fd, buffer, 1);
    lseek(fd, curr_pos, SEEK_SET);
    return bytes_read <= 0;
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

void write_to(int fd, const std::string& str) {
    if (str.empty()) return; // Skip empty strings
    ssize_t bytes_written = write(fd, str.c_str(), str.size());
    if (bytes_written == -1) {
        perror("Error writing to file");
        exit(EXIT_FAILURE);
    }
}

int count_lines(int fd) {
    int curr_pos = lseek(fd, 0, SEEK_CUR);
    lseek(fd, 0, SEEK_SET);
    int lines = 0;
    char buffer[1];
    while (read(fd, buffer, 1) > 0) {
        if (buffer[0] == '\n') {
            lines++;
        }
    }
    if (lseek(fd, -1, SEEK_END) >= 0) {
        read(fd, buffer, 1);
        if (buffer[0] != '\n') {
            lines++;
        }
    }
    lseek(fd, curr_pos, SEEK_SET);
    return lines;
}

int main() {
    int fd = open("text", O_RDWR);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int fd_3 = open("tmp.txt", O_RDWR | O_CREAT, 0644);
    if (fd_3 == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int lines_count = count_lines(fd);
    if (lines_count < 1) {
        close(fd);
        close(fd_3);
        return 0;
    }

    std::vector<std::string> arr;
    std::string tmp;
    bool swapped = false;

    for (int i = 0; i < lines_count - 1; i++) {
        swapped = false;
        lseek(fd, 0, SEEK_SET);
        lseek(fd_3, 0, SEEK_SET);
        ftruncate(fd_3, 0);
        arr.clear();
        arr.push_back(getLine(fd));

        for (int j = 0; j < lines_count - 1 - i; j++) {
            if (isFileEnd(fd)) break;
            arr.push_back(getLine(fd));
            tmp = arr[0];
            std::sort(arr.begin(), arr.end());
            write_to(fd_3, arr[0]);
            if (tmp != arr[0]) {
                swapped = true;
            }
            arr[0] = arr[1];
            arr.pop_back();
        }

        if (!arr.empty()) {
            write_to(fd_3, arr[0]);
        }

        while (!isFileEnd(fd)) {
            std::string remaining = getLine(fd);
            if (!remaining.empty()) {
                write_to(fd_3, remaining);
            }
        }

        if (!swapped) break;
        std::swap(fd, fd_3);
    }

    close(fd);
    close(fd_3);
    return 0;
}