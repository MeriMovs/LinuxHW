#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

int main() {
  int fd = open("text", O_RDONLY);
  if (fd == -1) {
    perror("Ошибка при открытии файла");
    exit(EXIT_FAILURE);
  }

  char filename[20];
  int i = 1;
  snprintf(filename, sizeof(filename), "%d.txt", i);

  int fd2 = open(filename, O_WRONLY | O_CREAT, 0664);
  if (fd2 == -1) {
    perror("Ошибка при открытии файла");
    exit(EXIT_FAILURE);
  }

  char buffer[1];
  std::string line;
  

// ------  read line by line and write in sep. files ----------
  while (read(fd, buffer, 1) > 0) {
    line += buffer[0];
    if (buffer[0] == '\n') {
      // -- write line --
      write(fd2, line.c_str(), line.size());
      close(fd2);
      line = "";

      // -- open new file --
      snprintf(filename, sizeof(filename), "%d.txt", ++i);
      fd2 = open(filename, O_WRONLY | O_CREAT, 0664);
      if (fd2 == -1) {
        perror("Ошибка при открытии файла");
        exit(EXIT_FAILURE);
      }

    }
  }

  close(fd);
  return 0;
  
}


//Есть очень большой файл (например, несколько гигабайт), содержащий строки текста (каждая заканчивается \n).
//Нужно получить отсортированную версию файла по строкам (лексикографически, как sort в Linux).
//Условие: Размер файла может значительно превышать доступный объём оперативной памяти. файл не помещается целиком в память (например, RAM ≤ 50 MB, а файл = 2 GB).

//Есть параметр R (critical RAM size) - максимально допустимый объём памяти, который программа может использовать.