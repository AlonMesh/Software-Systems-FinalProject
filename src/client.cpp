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

void startClient(const char* serverAddress, int port, unsigned int seed, int timeSkip) {
    // Seed the random number generator
    std::mt19937_64 rng(seed);

    // Generate random numbers between 0 and 40,000,000 (Orignally 10^12, changed due to performance issues in last exercise)
    std::uniform_int_distribution<long long> dist(0, 40000000LL);

    // Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    // Set up the server address structure
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // Convert the server address from text to binary form
    if (inet_pton(AF_INET, serverAddress, &server.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        close(sock);
        return;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        close(sock);
        return;
    }

    // Send and receive data in a loop
    while (true) {
        // Generate a random number
        long long randomNumber = dist(rng);
        std::cout << "Sending (client " << seed << "): " << randomNumber << std::endl;

        // Send the random number to the server
        if (send(sock, &randomNumber, sizeof(randomNumber), 0) < 0) {
            std::cerr << "Send failed" << std::endl;
            break;
        }

        // Receive a response from the server
        char response[20] = {0};
        int bytesReceived = recv(sock, response, sizeof(response) - 1, 0); // recv returns the number of bytes received, or -1 if an error occurred

        // Check if the server closed the connection (bytesReceived == 0) or if an error occurred (bytesReceived < 0)
        if (bytesReceived < 0) {
            std::cerr << "Receive failed" << std::endl;
            break;
        } else if (bytesReceived == 0) {
            std::cout << "Server closed the connection" << std::endl;
            break;
        }

        // Successfully received a response
        std::cout << "Server Response (client " << seed << "): " << response << std::endl;

        // Sleep for a specified time before sending the next number
        struct timespec ts;
        ts.tv_sec = timeSkip;
        ts.tv_nsec = 0;
        nanosleep(&ts, NULL);
    }

    // Close the socket
    close(sock);
}

// Function to validate command-line arguments
bool validateArguments(int argc, char* argv[]) {
    if (argc < 4 || argc > 5) {
        std::cerr << "Usage: " << argv[0] << " <server address> <port> <seed> [time skip]" << std::endl;
        return false;
    }

    unsigned int seed = static_cast<unsigned int>(std::atoll(argv[3]));
    if (seed == 0) {
        std::cerr << "Seed must be a non-zero integer" << std::endl;
        return false;
    }

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
    if (!validateArguments(argc, argv)) {
        return 1;
    }

    // Parse the command-line arguments to variables
    const char* serverAddress = argv[1];
    int port = std::stoi(argv[2]);
    unsigned int seed = static_cast<unsigned int>(std::atoll(argv[3]));
    int timeSkip = argc == 5 ? std::atoi(argv[4]) : 0;

    startClient(serverAddress, port, seed, timeSkip);

    return 0;
}
