#include "ProducerConsumer.hpp"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <mutex>
#include <condition_variable>
#include <queue>
#define MAX_LIMIT 6191736422

void ProducerConsumer::startServer(int port) {
    // Start the sieving process in a separate thread
    std::thread sieveThread(&ProducerConsumer::startSieving, this);

    // Create socket for the server
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

    if (listen(serverSocket, 10) < 0) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << port << std::endl;

    m_running = true;
    std::thread listenThread(&ProducerConsumer::listenForConnections, this, serverSocket);
    listenThread.detach();

    std::thread receiveThread(&ProducerConsumer::receiveMessages, this);
    receiveThread.detach();

    // Detach the sieve thread
    sieveThread.detach();

    // Wait for the sieving process to complete
    {
        std::unique_lock<std::mutex> lock(mtx);
        condVar.wait(lock, [this]() { return !isSieving; });
    }

    std::cout << "Start processing messages..." << std::endl;

    ActiveObject<std::pair<int, long long>> processor(&m_processingQueue, std::bind(&ProducerConsumer::processMessages, this, std::placeholders::_1));
}

bool validateInput(long long limit) {
    if (limit <= 1) {
        std::cerr << "Please enter a number greater than 1." << std::endl;
        return false;
    }

    if (limit > MAX_LIMIT) {
        std::cerr << "Please enter a number less than or equal to " << MAX_LIMIT << "." << std::endl;
        return false;
    }

    return true;
}

std::vector<bool> sieveOfEratosthenes(long long limit) {
    if (!validateInput(limit)) {
        return {};
    }

    std::vector<bool> isPrimeVector(limit + 1, true);

    isPrimeVector[0] = isPrimeVector[1] = false;

    for (long long p = 2; p * p <= limit; p++) {
        if (isPrimeVector[p]) {
            for (long long i = p * p; i <= limit; i += p) {
                isPrimeVector[i] = false;
            }
        }
    }

    return isPrimeVector;
}

void ProducerConsumer::startSieving() {
    {
        std::lock_guard<std::mutex> lock(mtx);

        std::cout << "Starting sieving process..." << std::endl;

        // Perform the Sieve of Eratosthenes algorithm
        isPrimeVector = sieveOfEratosthenes(MILLION40);

        std::cout << "Sieving process complete." << std::endl;

        // Sleep for 1 seconds
        std::this_thread::sleep_for(std::chrono::seconds(1));

        isSieving = false;
    }

    // Notify other threads that sieving is complete
    condVar.notify_all();
}

void ProducerConsumer::listenForConnections(int serverSocket) {
    while (m_running) {
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int newClientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        if (newClientSocket < 0) {
            perror("Accepting connection failed");
            continue;
        }
        m_newConnectionsQueue.push(newClientSocket);
        std::cout << "Got a new client with fd num: " << newClientSocket << std::endl;
    }
}

void ProducerConsumer::receiveMessages() {
    timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    fd_set readfds;
    FD_ZERO(&readfds);
    int maxfd = 0;
    std::vector<int> clientSockets;

    while (true) {
        while (!m_newConnectionsQueue.empty()) {
            int clientSocket = m_newConnectionsQueue.front();
            m_newConnectionsQueue.pop();
            clientSockets.push_back(clientSocket);
            FD_SET(clientSocket, &readfds);
            maxfd = std::max(maxfd, clientSocket);
        }

        fd_set tempfds = readfds;

        int result = select(maxfd + 1, &tempfds, NULL, NULL, &timeout);
        if (result < 0) {
            perror("Select error");
            return;
        } else if (result == 0) {
            continue; // Timeout, no messages received
        }

        for (size_t i = 0; i < clientSockets.size(); ++i) {
            int clientSocket = clientSockets[i];
            if (FD_ISSET(clientSocket, &tempfds)) {
                long long number;
                ssize_t bytesRead = recv(clientSocket, &number, sizeof(number), 0);
                if (bytesRead <= 0) {
                    close(clientSocket);
                    FD_CLR(clientSocket, &readfds);
                    clientSockets.erase(clientSockets.begin() + i);
                    --i;
                } else {
                    m_processingQueue.push(std::make_pair(clientSocket, number));
                }
            }
        }
    }
}

void ProducerConsumer::processMessages(std::pair<int, long long> data) {
    int clientFd = data.first;
    long long number = data.second;

    // Check if the number is prime using the precomputed sieve vector
    bool prime = (number <= MILLION40 && isPrimeVector[number]);

    std::string numStr = std::to_string(number);
    const char* response = prime ? "YES" : "NO";

    std::string combinedResponse = numStr + " " + response;

    send(clientFd, combinedResponse.c_str(), combinedResponse.length(), 0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    int port = std::stoi(argv[1]);

    ProducerConsumer producerConsumer;
    producerConsumer.startServer(port);

    return 0;
}
