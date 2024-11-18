#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"  // Change to the server's IP address
#define SERVER_PORT 6667       // Change to the server's port
#define BUFFER_SIZE 1024       // Buffer size for messages

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];

    // Step 1: Create the socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Step 2: Define the server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);

    // Convert IP address to binary form and set it
    if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Step 3: Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    printf("Connected to the server.\n");

    // Step 4: Send a message to the server
    char *message = "Hello, server!";
    if (send(client_socket, message, strlen(message), 0) < 0) {
        perror("Failed to send message");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    printf("Message sent to server: %s\n", message);

    // Step 5: Receive the server's response
    ssize_t num_bytes = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    if (num_bytes < 0) {
        perror("Failed to receive response");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    buffer[num_bytes] = '\0';  // Null-terminate the received data
    printf("Message received from server: %s\n", buffer);

    // Step 6: Close the socket
    close(client_socket);
    printf("Connection closed.\n");

    return 0;
}
