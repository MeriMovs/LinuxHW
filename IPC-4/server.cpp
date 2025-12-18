#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 4004
#define BUF_SIZE 1024

struct ClientConnection 
{
    int socket_fd;
};

std::vector<ClientConnection*> active_clients;
pthread_mutex_t clients_list_mutex = PTHREAD_MUTEX_INITIALIZER;

void send_to_all_clients(const std::string &txt, int s_socket = -1) 
{
    pthread_mutex_lock(&clients_list_mutex);
    for (auto &client : active_clients) 
    {
        if (client->socket_fd != s_socket) 
        {
            send(client->socket_fd, txt.c_str(), txt.size(), 0);
        }
    }
    pthread_mutex_unlock(&clients_list_mutex);
}

void register_new_client(ClientConnection *new_client) 
{
    pthread_mutex_lock(&clients_list_mutex);
    active_clients.push_back(new_client);
    pthread_mutex_unlock(&clients_list_mutex);
}

void unregister_client(int client_socket) 
{
    pthread_mutex_lock(&clients_list_mutex);
    for (auto it = active_clients.begin(); it != active_clients.end(); ++it) 
    {
        if ((*it)->socket_fd == client_socket) 
        {
            delete *it;
            active_clients.erase(it);
            break;
        }
    }
    pthread_mutex_unlock(&clients_list_mutex);
}

void *messages(void *client_arg) 
{
    ClientConnection *client = (ClientConnection*)client_arg;
    int sock = client->socket_fd;

    char buff[BUF_SIZE];
    while (true) 
    {
        int bytes_r = recv(sock, buff, sizeof(buff)-1, 0);
        if (bytes_r <= 0) break;

        buff[bytes_r] = '\0';
        std::string msg(buff);

        if (msg.substr(0,5) == "/exit") break;

        std::string un = "-> " + msg;
        send_to_all_clients(un, sock);
    }

    close(sock);
    unregister_client(sock);
    pthread_exit(nullptr);
}

int main() 
{
    int l_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (l_socket < 0) { 
        perror("Failed to create socket"); 
        return 1; 
    }

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(l_socket, (sockaddr*)&server_address, sizeof(server_address)) < 0) { 
        perror("Failed to bind socket"); 
        return 1; 
    }
    
    if (listen(l_socket, 10) < 0) { 
        perror("Failed to listen"); 
        return 1; 
    }

    std::cout << "Server launched on port " << PORT << std::endl;

    while (true) 
    {
        sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int client_sock = accept(l_socket, (sockaddr*)&client_addr, &len);
        if (client_sock < 0) continue;

        ClientConnection *new_client = new ClientConnection{client_sock};
        register_new_client(new_client);

        pthread_t tid;
        pthread_create(&tid, nullptr, messages, new_client);
        pthread_detach(tid);
    }

    close(l_socket);
    return 0;
}