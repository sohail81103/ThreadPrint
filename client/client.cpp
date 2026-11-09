#include <iostream>
#include <cstring>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {

    // 1. Create client socket
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (client_fd == -1) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    // 2. Configure server address
    sockaddr_in server_address{};

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);

    inet_pton(
        AF_INET,
        "127.0.0.1",
        &server_address.sin_addr
    );

    // 3. Connect to server
    if (connect(
        client_fd,
        (struct sockaddr*)&server_address,
        sizeof(server_address)
    ) < 0) {

        std::cerr << "Connection failed\n";
        close(client_fd);
        return 1;
    }

    std::cout << "Connected to ThreadPrint server!\n";

    // 4. Send message
    const char* message = "Hello from ThreadPrint client!";

    send(
        client_fd,
        message,
        std::strlen(message),
        0
    );

    // 5. Receive server response
    char buffer[1024] = {};

    ssize_t bytes_received = recv(
        client_fd,
        buffer,
        sizeof(buffer) - 1,
        0
    );

    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        std::cout << "Server response: " << buffer << '\n';
    }

    // 6. Close connection
    close(client_fd);

    return 0;
}