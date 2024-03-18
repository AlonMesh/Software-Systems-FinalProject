#include "client.hpp"
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <random>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc < 4 || 5 < argc) {
        std::cerr << "Usage: " << argv[0] << " <server address> <port> <seed> [time skip]" << std::endl;
        return 1;
    }

    const char* serverAddress = argv[1];
    int port = std::stoi(argv[2]);
    unsigned int seed = static_cast<unsigned int>(std::atoll(argv[3]));
    int timeSkip = argc == 5 ? std::atoi(argv[4]) : 0;

    startClient(serverAddress, port, seed, timeSkip);

    return 0;
}

void startClient(const char* serverAddress, int port, unsigned int seed, int timeSkip) {
    // Initialize random number generator
    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<long long> dist(0, 61917364224);

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

    while (true) {
        // Generate random number
        long long randomNumber = dist(rng);
        std::cout << "Sending: " << randomNumber << std::endl;

        // Send number to server
        if (send(sock, &randomNumber, sizeof(randomNumber), 0) < 0) {
            std::cerr << "Send failed" << std::endl;
            break;
        }

        // Wait for response
        char response[3];
        if (recv(sock, response, sizeof(response), 0) < 0) {
            std::cerr << "Receive failed" << std::endl;
            break;
        }

        // Avoid cases like printing "NOS" instead of "NO"
        if (response[0] == 'N') {
            response[2] = '\0';
        }

        // Print response
        std::cout << "Server Response: " << response << std::endl;

        if (timeSkip > 0) {
            std::this_thread::sleep_for(std::chrono::seconds(timeSkip));
        }
    }

    close(sock);
}
