#include "client.hpp"
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <random>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>

void startClient(const char* serverAddress, int port, unsigned int seed, int timeSkip) {
    // Initialize random number generator
    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<long long> dist(0, 61917364224); // 10^12

    // Create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    // Setup server structure
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, serverAddress, &server.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        return;
    }

    // Main loop
    while (true) {
        // Generate random number
        long long randomNumber = dist(rng);
        std::cout << "Sending (client " << seed << "): " << randomNumber << std::endl;

        // Send number to server
        if (send(sock, &randomNumber, sizeof(randomNumber), 0) < 0) {
            std::cerr << "Send failed" << std::endl;
            break;
        }
        
        // Setup for select
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(sock, &readfds);

        // Setup timeout
        struct timeval tv;
        tv.tv_sec = timeSkip;  // Time skip as seconds
        tv.tv_usec = 0;        // 0 microseconds

        // Wait for the socket to be ready to read or for timeout
        int activity = select(sock + 1, &readfds, NULL, NULL, &tv);

        if (activity < 0) {
            std::cerr << "Select error" << std::endl;
            break;
        }

        // If socket is ready to be read
        if (FD_ISSET(sock, &readfds)) {
            char response[20] = {0};  // Buffer for response
            int bytesReceived = recv(sock, response, sizeof(response) - 1, 0);
            if (bytesReceived < 0) {
                std::cerr << "Receive failed" << std::endl;
                break;
            } else if (bytesReceived == 0) {
                std::cout << "Server closed the connection" << std::endl;
                break;
            }

            // Print response
            std::cout << "Server Response (client " << seed << "): " << response << std::endl;

            // Delay for the remaining time
            struct timespec ts;
            ts.tv_sec = timeSkip;
            nanosleep(&ts, NULL);  // Sleep for timeSkip seconds
        }
    }

    // Cleanup
    close(sock);
}

bool validateArguments(int argc, char* argv[]) {
    // Check if the number of arguments is correct
    if (argc < 4 || argc > 5) {
        std::cerr << "Usage: " << argv[0] << " <server address> <port> <seed> [time skip]" << std::endl;
        return false;
    }

    // Check if the seed is valid
    unsigned int seed = static_cast<unsigned int>(std::atoll(argv[3]));

    if (seed == 0) {
        std::cerr << "Seed must be a non-zero integer" << std::endl;
        return false;
    }

    // Check if the time skip is valid
    if (argc == 5) {
        int timeSkip = std::atoi(argv[4]);

        if (timeSkip < 0) {
            std::cerr << "Time skip must be non-negative" << std::endl;
            return false;
        }
    }

    return true;
}

int main(int argc, char *argv[]) {
    // Validate command-line arguments
    if (!validateArguments(argc, argv)) {
        return 1;
    }

    const char* serverAddress = argv[1];
    int port = std::stoi(argv[2]);
    unsigned int seed = static_cast<unsigned int>(std::atoll(argv[3]));
    int timeSkip = argc == 5 ? std::atoi(argv[4]) : 0;

    startClient(serverAddress, port, seed, timeSkip);

    return 0;
}