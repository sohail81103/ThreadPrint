#include <iostream>
#include <cstring>

#include "JobQueue.h"
#include "ThreadPool.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {

    // Shared job queue
    JobQueue job_queue;

    // Start three worker threads
    ThreadPool thread_pool(job_queue, 3);

    int next_job_id = 1;

    // Create TCP socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    sockaddr_in server_address{};

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8080);

    // Bind socket
    if (bind(
        server_fd,
        (struct sockaddr*)&server_address,
        sizeof(server_address)
    ) < 0) {

        std::cerr << "Binding failed\n";
        close(server_fd);
        return 1;
    }

    // Listen for clients
    if (listen(server_fd, 5) < 0) {

        std::cerr << "Listening failed\n";
        close(server_fd);
        return 1;
    }

    std::cout << "ThreadPrint server listening on port 8080...\n";

    while (true) {

        sockaddr_in client_address{};
        socklen_t client_length = sizeof(client_address);

        int client_fd = accept(
            server_fd,
            (struct sockaddr*)&client_address,
            &client_length
        );

        if (client_fd < 0) {
            std::cerr << "Client connection failed\n";
            continue;
        }

        char buffer[1024] = {};

        ssize_t bytes_received = recv(
            client_fd,
            buffer,
            sizeof(buffer) - 1,
            0
        );

        if (bytes_received > 0) {

            buffer[bytes_received] = '\0';

            // Create print job
            PrintJob job(next_job_id++, buffer);

            // Submit job to queue
            if (job_queue.push(job)) {

                std::cout << "Submitted Job "
                          << job.job_id << '\n';

                const char* response =
                    "Print job successfully queued";

                send(
                    client_fd,
                    response,
                    std::strlen(response),
                    0
                );
            }
        }

        close(client_fd);
    }

    close(server_fd);

    return 0;
}