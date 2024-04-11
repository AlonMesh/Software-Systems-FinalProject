#include "Pipeline.hpp"
#include "TSQueue.hpp"
#include "ActiveObject.hpp"

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

void Server::stop() {
    m_running = false;
    close(m_serverSocket);
    for (auto& activeObject : m_activeObjects) {
        activeObject.stop();
    }
}

int Server::startServer(int port) {
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
    std::thread listenThread(&Server::listenForConnections, this);
    listenThread.detach();

    m_activeObjects.emplace_back(&m_newConnectionsQueue, std::bind(&Server::receiveMessages, this));
    m_activeObjects.emplace_back(&m_processingQueue, std::bind(&Server::processMessages, this));

    for (auto& activeObject : m_activeObjects) {
        activeObject.start();
    }
}

void Server::listenForConnections() {
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

void Server::receiveMessages() {
    fd_set readfds;
    while (m_running) {
        FD_ZERO(&readfds);
        int maxFd = 0;

        // Add client sockets to set
        for (int clientSocket : m_newConnectionsQueue.getCopyOfQueue()) {
            FD_SET(clientSocket, &readfds);
            maxFd = std::max(maxFd, clientSocket);
        }

        // Wait for activity on sockets
        if (select(maxFd + 1, &readfds, NULL, NULL, NULL) < 0) {
            perror("Select error");
            continue;
        }

        for (int clientSocket : m_newConnectionsQueue.getCopyOfQueue()) {
            if (FD_ISSET(clientSocket, &readfds)) {
                char buffer[BUFFER_SIZE];
                ssize_t bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
                if (bytesRead <= 0) {
                    // Handle disconnect or error
                    close(clientSocket);
                    m_newConnectionsQueue.remove(clientSocket);
                } else {
                    buffer[bytesRead] = '\0'; // Null-terminate the string
                    m_processingQueue.push({clientSocket, std::string(buffer)});
                }
            }
        }
    }
}

void Server::processMessages() {
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

bool Server::isPrime(long long number) {
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
    startServer(port)

    return 0;
}