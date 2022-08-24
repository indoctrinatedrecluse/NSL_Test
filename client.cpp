#include <iostream>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main (int argc, char *argv[]) {
    int socket_desc, client_sock, c, read_size;
    struct sockaddr_in server, client;
    float position[3];

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        perror ("Could not create socket");
    }
    puts ("Socket created");

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8880);

    // Bind the socket
    if (bind(socket_desc, (struct sockaddr*)&server, sizeof(server)) < 0) {
 
        // print the error message
        perror ("Could not bind socket");
        return 1;
    }
    puts ("Socket bound");

    // listen to the socket
    listen(socket_desc, 3);
 
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
 
    // accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr*)&client, (socklen_t*)&c);
 
    if (client_sock < 0) {
        perror ("Accept failed");
        return 1;
    }
 
    puts ("Connection accepted");

    // Receive a message from client
    while ((read_size = recv(client_sock, &position, 3*sizeof(float), 0)) > 0) {
        write(client_sock, &position, 3*sizeof(float));
    }

    if (read_size == 0) {
        puts("Client disconnected");
    }
    else if (read_size == -1) {
        perror("Receive failed");
    }
 
    return 0;
}