// Написать программу, которая выполняет простой HTTP-запрос к веб-серверу "http://httpforever.com/", 
// получает ответ и сохраняет его в файл с именем "httpforever.html". 

// - Используя сокет, установите соединение с указанным веб-сервером, (нужно найти ip веб-сервера).
// - Отправьте HTTP-запрос (GET /) на сервер с помощью сокета. (Текст запроса оставлю ниже)
// - Получите ответ от сервера и сохраните его в файл.
// - Убедитесь что все прошло успешно

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    const char* hostname = "httpforever.com";
    const char* request = "GET / HTTP/1.1\r\nHost: httpforever.com\r\nConnection: close\r\n\r\n";
    
    struct hostent* host = gethostbyname(hostname);
    if (!host) {
        std::cerr << "failed to get IP\n";
        return 1;
    }
    
    char* ip = inet_ntoa(*((struct in_addr*)host->h_addr));
    
    // --
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Ошибка создания сокета\n";
        return 1;
    }
    
    // --
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(80);
    server.sin_addr.s_addr = inet_addr(ip);
    
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "failed to connect\n";
        close(sock);
        return 1;
    }
    
    send(sock, request, strlen(request), 0);
    
    int fd = open("httpforever.html", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        std::cerr << "failed to open file\n";
        close(sock);
        return 1;
    }
    
    char buffer[4096];
    int bytes;
    
    while ((bytes = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        write(fd, buffer, bytes);
    }
    
    close(fd);
    close(sock);
    
    return 0;
}