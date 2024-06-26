#include "server.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/select.h>
#define MAX_CLIENTS 10

// Function with a naive implementation to check if a number is prime
bool isPrime(long long number) {
    if (number <= 1) return false;
    for (long long i = 2; i * i <= number; i++) {
        if (number % i == 0) return false;
    }
    return true;
}

// Function to handle client requests
void handleClient(fd_set& masterSet, int sd) {
    long long number;

    // Receive data from client and set it to number
    ssize_t recvSize = recv(sd, &number, sizeof(number), 0);

    if (recvSize < 0) {
        std::cerr << "Error receiving data from client " << sd << std::endl;
        close(sd);
        FD_CLR(sd, &masterSet);
    } 

    else if (recvSize == 0) {
        std::cout << "Client " << sd << " disconnected." << std::endl;
        close(sd);
        FD_CLR(sd, &masterSet);
    } 
    
    else {
        bool prime = isPrime(number);
        const char* response = prime ? "YES" : "NO";
        std::string combinedResponse = std::to_string(number) + " " + response;
        send(sd, combinedResponse.c_str(), combinedResponse.length(), 0);
        }
}

// Function to start the server
void startServer(int port) {
    // Create socket for server
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) {
        std::cerr << "Cannot open socket" << std::endl;
        return;
    }

    // Setup server structure
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    // Bind the socket to the address and port
    if (bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) { // bind returns 0 on success, -1 on failure
        std::cerr << "Bind failed" << std::endl;
        close(serverSock);
        return;
    }

    // Listen for incoming connections
    if (listen(serverSock, MAX_CLIENTS) < 0) { // Listen for up to MAX_CLIENTS connections
        std::cerr << "Listen failed" << std::endl;
        close(serverSock);
        return;
    }

    // Connection successful
    std::cout << "Server listening on port " << port << std::endl;

    // Accept incoming connections and handle them using select
    fd_set masterSet, readSet;
    FD_ZERO(&masterSet);
    FD_SET(serverSock, &masterSet);
    int maxSd = serverSock;

    while (true) {
        readSet = masterSet;

        // Select the ready file descriptors
        int activity = select(maxSd + 1, &readSet, NULL, NULL, NULL); // select returns the number of ready fd, or -1 if an error occurred

        if (activity < 0) {
            std::cerr << "Select error" << std::endl;
            break;
        }

        for (int sd = 0; sd <= maxSd; sd++) {
            if (FD_ISSET(sd, &readSet)) {
                if (sd == serverSock) {
                    // Accept new connection
                    struct sockaddr_in clientAddr;
                    socklen_t clientLen = sizeof(clientAddr);
                    int clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, &clientLen); // accept returns the client socket, or -1 if an error occurred
                    if (clientSock < 0) {
                        std::cerr << "Accept failed" << std::endl;
                    } else {
                        std::cout << "New connection, socket fd is " << clientSock << std::endl;
                        FD_SET(clientSock, &masterSet);
                        if (clientSock > maxSd) {
                            maxSd = clientSock;
                        }
                    }
                } else {
                    handleClient(masterSet, sd);
                }
            }
        }
    }

    // Cleanup
    close(serverSock);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    // Extract port number and start server
    int port = std::stoi(argv[1]);
    startServer(port);

    return 0;
}
