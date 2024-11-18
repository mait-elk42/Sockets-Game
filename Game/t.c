#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(6667),
        .sin_addr.s_addr = inet_addr("127.0.0.1")
    };

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connect failed");
        close(client_fd);
        return EXIT_FAILURE;
    }

    // Poll setup
    struct pollfd pfds[1]; // 0: client socket
    pfds[0].fd = client_fd;
    pfds[0].events = POLLIN; // Monitor socket for incoming data

    printf("Connected to server. Waiting for messages...\n");

    char buffer[1024];
    int hello_counter = 0; // Counter to limit how often "hello" is printed
    while (1) {
        int poll_count = poll(pfds, 1, 100); // Timeout set to 100ms
        if (poll_count == -1) {
            perror("Poll failed");
            break;
        }

        // Check client socket for server messages
        if (pfds[0].revents & POLLIN) {
            memset(buffer, 0, sizeof(buffer));
            ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
            if (bytes_received <= 0) {
                printf("Server disconnected\n");
                break;
            }
            printf("Server: %s\n", buffer);
        }

        // Print "hello" every 500ms
        hello_counter++;
        if (hello_counter >= 5) { // 5 x 100ms = 500ms
            printf("hello\n");
			send(client_fd, "hello bro", 9, 0);
            hello_counter = 0;
        }
    }

    close(client_fd);
    return EXIT_SUCCESS;
}
