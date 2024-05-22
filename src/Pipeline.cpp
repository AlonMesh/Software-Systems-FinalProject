#include "Pipeline.hpp"

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

void Pipeline::startServer(int port) {
    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address structure
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    // Bind the socket to the port
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) { // bind returns 0 on success
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, MAX_CLIENTS) < 0) { // listen returns 0 on success
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << port << std::endl;

    m_running = true;

    // Start thread to listen for new connections
    std::thread listenThread(&Pipeline::listenForConnections, this, serverSocket);
    listenThread.detach();

    // Start thread to receive messages from clients
    std::thread receiveThread(&Pipeline::receiveMessages, this);
    receiveThread.detach();

    // Start an active object to process messages
    ActiveObject<std::pair<int, long long>> processor(&m_processingQueue, std::bind(&Pipeline::processMessages, this, std::placeholders::_1));
}

void Pipeline::listenForConnections(int serverSocket) {
    while (m_running) {
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);

        // Accept new client connection
        int newClientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        if (newClientSocket < 0) {
            perror("Accepting connection failed");
            continue;
        }

        m_newConnectionsQueue.push(newClientSocket);
        std::cout << "Got a new client with fd num: " << newClientSocket << std::endl;
    }
}

void Pipeline::receiveMessages() {
    timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    fd_set readfds;
    FD_ZERO(&readfds);
    int maxfd = 0;
    std::vector<int> clientSockets;

    while (true) {
        // Add new connections to the client sockets vector and fd set
        while (!m_newConnectionsQueue.empty()) {
            int clientSocket = m_newConnectionsQueue.front();
            m_newConnectionsQueue.pop();
            clientSockets.push_back(clientSocket);
            FD_SET(clientSocket, &readfds);
            maxfd = std::max(maxfd, clientSocket);
        }

        fd_set tempfds = readfds; // Temporary fd set for select call

        // Select the ready file descriptors
        int result = select(maxfd + 1, &tempfds, NULL, NULL, &timeout); // select returns the number of ready file descriptors
        if (result < 0) {
            perror("Select error");
            return;
        } else if (result == 0) {
            continue; // Timeout, no messages received
        }

        // Check each client socket for incoming messages
        for (size_t i = 0; i < clientSockets.size(); ++i) {
            int clientSocket = clientSockets[i];
            if (FD_ISSET(clientSocket, &tempfds)) {
                long long number;
                ssize_t bytesRead = recv(clientSocket, &number, sizeof(number), 0);
                if (bytesRead <= 0) {
                    // Close the socket and remove it from the fd set and client sockets vector
                    std::cout << "Client " << clientSocket << " disconnected" << std::endl;
                    close(clientSocket);
                    FD_CLR(clientSocket, &readfds);
                    clientSockets.erase(clientSockets.begin() + i);
                    --i;
                } else {
                    // Push received number to processing queue
                    m_processingQueue.push({clientSocket, number});
                }
            }
        }
    }
}

void Pipeline::processMessages(std::pair<int, long long> data) {
    int clientFd = data.first;
    long long number = data.second;

    // Check if the number is prime
    bool prime = isPrime(number);
    std::string numStr = std::to_string(number);
    const char* response = prime ? "YES" : "NO";

    // Create response string and send it to the client
    std::string combinedResponse = numStr + " " + response;
    send(clientFd, combinedResponse.c_str(), combinedResponse.length(), 0);
}

// Function with a naive implementation (same as in the previous snippet) to check if a number is prime
bool Pipeline::isPrime(long long number) {
    if (number <= 1) return false;
    for (long long i = 2; i * i <= number; i++) {
        if (number % i == 0) return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << "<port>" << std::endl;
        return 1;
    }

    // Extract port number from command line arguments
    int port = std::stoi(argv[1]);

    // Create a Pipeline object and start the server
    Pipeline pipeline;
    pipeline.startServer(port);

    return 0;
}
