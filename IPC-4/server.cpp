#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 10

int clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void broadcast(char* message, int sender_fd) {
    pthread_mutex_lock(&clients_mutex);
    for(int i = 0; i < client_count; i++) {
        if(clients[i] != sender_fd) {
            send(clients[i], message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void remove_client(int fd) {
    pthread_mutex_lock(&clients_mutex);
    for(int i = 0; i < client_count; i++) {
        if(clients[i] == fd) {
            for(int j = i; j < client_count - 1; j++) {
                clients[j] = clients[j + 1];
            }
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void* handle_client(void* arg) {
    int client_fd = *(int*)arg;
    free(arg);
    char buffer[1024];
    ssize_t valread;
    
    while((valread = read(client_fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[valread] = '\0';
        printf("Client %d: %s", client_fd, buffer);
        broadcast(buffer, client_fd);
    }
    
    printf("Client %d disconnected\n", client_fd);
    remove_client(client_fd);
    close(client_fd);
    return NULL;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                   &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (listen(server_fd, 10) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    printf("Server listening on port %d\n", PORT);
    
    while(1) {
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
            perror("accept");
            continue;
        }
        
        pthread_mutex_lock(&clients_mutex);
        if(client_count < MAX_CLIENTS) {
            clients[client_count++] = new_socket;
            pthread_mutex_unlock(&clients_mutex);
            
            printf("New client connected: %d\n", new_socket);
            
            int* client_fd = (int*)malloc(sizeof(int));
            *client_fd = new_socket;
            pthread_t tid;
            pthread_create(&tid, NULL, handle_client, client_fd);
            pthread_detach(tid);
        } else {
            pthread_mutex_unlock(&clients_mutex);
            close(new_socket);
        }
    }
    
    close(server_fd);
    return 0;
}