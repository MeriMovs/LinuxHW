#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 4004
#define BUF_SIZE 1024

int m_socket;

void* thread_foo(void*) 
{
    char buff[BUF_SIZE];
    while (true) {
        int b_r = recv(m_socket, buff, sizeof(buff)-1, 0);
        if (b_r <= 0) break;
        buff[b_r] = '\0';
        std::cout << buff;
    }
    exit(0);
}

int main() 
{
    sockaddr_in s_addr;
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket < 0) { 
        perror("Failed to create socket"); 
        return 1; 
    }

    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(SERVER_PORT);
    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(m_socket, (sockaddr*)&s_addr, sizeof(s_addr)) < 0) { 
        perror("Failed to connect to server"); 
        return 1; 
    }

    pthread_t r_t_id;
    pthread_create(&r_t_id, nullptr, thread_foo, nullptr);

    std::string message;
    while (true) 
    {
        std::getline(std::cin, message);
        if (message.substr(0,5) == "/exit") break;
        message += "\n";
        send(m_socket, message.c_str(), message.size(), 0);
    }

    close(m_socket);
    return 0;
}