#include "server.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    int port = std::stoi(argv[1]);
    startServer(port);

    return 0;
}


bool isPrime(long long number) {
    if (number <= 1) return false;
    for (long long i = 2; i * i <= number; i++) {
        if (number % i == 0) return false;
    }
    return true;
}

void handleClient(int clientSock) {
    while (true) {
        long long number;
        ssize_t recvSize = recv(clientSock, &number, sizeof(number), 0);
        if (recvSize < 0) {
            std::cerr << "Error receiving data." << std::endl;
            break;
        } else if (recvSize == 0) {
            // Client disconnected
            break;
        }

        bool prime = isPrime(number);
        const char* response = prime ? "YES" : "NO";
        send(clientSock, response, strlen(response), 0);
    }
    close(clientSock);
}

void startServer(int port) {
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) {
        std::cerr << "Cannot open socket" << std::endl;
        return;
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return;
    }

    listen(serverSock, 5);

    std::cout << "Server listening on port " << port << std::endl;

    while (true) {
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, &clientLen);
        if (clientSock < 0) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }

        std::thread(handleClient, clientSock).detach();
    }
}
