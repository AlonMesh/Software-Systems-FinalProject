#include "Pipeline.hpp"
#include "TSQueue.hpp"
#include "ActiveObject.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

// void Pipeline::stop() {
//     m_running = false;
//     close(m_serverSocket);
//     for (auto& activeObject : m_activeObjects) {
//         activeObject.stop();
//     }
// }

void Pipeline::startServer(int port) {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, MAX_CLIENTS) < 0) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    m_running = true;
    std::thread listenThread(&Pipeline::listenForConnections, this);
    listenThread.detach();

    ActiveObject<int> reciver(&m_newConnectionsQueue, std::bind(&Pipeline::receiveMessages, this));
    ActiveObject<std::pair<int, std::string>> processor(&m_processingQueue, std::bind(&Pipeline::processMessages, this));
}

void Pipeline::listenForConnections() {
    while (m_running) {
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int newClientSocket = accept(m_serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        if (newClientSocket < 0) {
            perror("Accepting connection failed");
            continue;
        }
        m_newConnectionsQueue.push(newClientSocket);
    }
}

void Pipeline::receiveMessages() {
    fd_set readfds;
    while (m_running) {
        FD_ZERO(&readfds);

        // Add client sockets to set
        while (!m_newConnectionsQueue.empty()) {
            int clientSocket = m_newConnectionsQueue.pop();
            FD_SET(clientSocket, &readfds);
        }

        // Wait for activity on sockets
        if (select(FD_SETSIZE, &readfds, NULL, NULL, NULL) < 0) {
            perror("Select error");
            continue;
        }

        for (int clientSocket = 0; clientSocket < FD_SETSIZE; ++clientSocket) {
            if (FD_ISSET(clientSocket, &readfds)) {
                char buffer[BUFFER_SIZE];
                ssize_t bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
                if (bytesRead <= 0) {
                    // Handle disconnect or error
                    close(clientSocket);
                    // Remove clientSocket from m_newConnectionsQueue if needed
                    // Note: You might need to implement a method to remove a specific element from TSQueue
                } else {
                    buffer[bytesRead] = '\0'; // Null-terminate the string
                    m_processingQueue.push({clientSocket, std::string(buffer)});
                }
            }
        }
    }
}


void Pipeline::processMessages() {
    while (m_running) {
        auto message = m_processingQueue.pop();
        int clientSocket = message.first;
        std::string msg = message.second;

        // Process message
        long long number;
        try {
            number = std::stoll(msg);
        } catch (...) {
            send(clientSocket, "Invalid input\n", strlen("Invalid input\n"), 0);
            close(clientSocket);
            continue;
        }

        std::string response = (isPrime(number) ? "YES\n" : "NO\n");
        send(clientSocket, response.c_str(), response.size(), 0);
        close(clientSocket);
    }
}

bool Pipeline::isPrime(long long number) {
    if (number <= 1) return false;
    if (number <= 3) return true;
    if (number % 2 == 0 || number % 3 == 0) return false;
    for (long long i = 5; i * i <= number; i += 6) {
        if (number % i == 0 || number % (i + 2) == 0) return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << "<port>" << std::endl;
        return 1;
    }

    int port = std::stoi(argv[1]);

    Pipeline pipeline; // Create an instance of Pipeline
    pipeline.startServer(port); // Call startServer on the instance

    return 0;
}