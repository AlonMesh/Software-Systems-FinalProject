#include "Pipeline.hpp"

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

    // Connection successful
    std::cout << "Server listening on port " << port << std::endl;

    m_running = true;
    std::thread listenThread(&Pipeline::listenForConnections, this, serverSocket);
    listenThread.detach();

    std::thread receiveThread(&Pipeline::receiveMessages, this);
    receiveThread.detach();

    ActiveObject<std::pair<int, long long>> processor(&m_processingQueue, std::bind(&Pipeline::processMessages, this, std::placeholders::_1));
}

void Pipeline::listenForConnections(int serverSocket) {
    while (m_running) {
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int newClientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        if (newClientSocket < 0) {
            perror("Accepting connection failed");
            continue;
        }
        m_newConnectionsQueue.push(newClientSocket);
        std::cout << "got a new client with fd num: " << newClientSocket <<std::endl;
    }
}

void Pipeline::receiveMessages() {
    timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    fd_set readfds;
    FD_ZERO(&readfds);
    int maxfd = 0;

    while (true) {
        while (!m_newConnectionsQueue.empty()) {
            int clientSocket = m_newConnectionsQueue.front(); // Get the front element
            m_newConnectionsQueue.pop(); // Remove the front element
            FD_SET(clientSocket, &readfds); //Add it to the fd set
            maxfd = std::max(maxfd, clientSocket); // Update maxfd

//            std::cout << clientSocket <<std::endl;

        }
//        std::cout << "here!" <<std::endl;

        // Wait for activity on sockets
        int result = select(maxfd+1, &readfds, NULL, NULL, &timeout);
//        std::cout << "Select result: "<< result << std::endl;
        if (result < 0) {
            perror("Select error");
            return;
        } else if(result == 0){
//            std::cout << "no messages" <<std::endl;
        } else if(result >= 1){
            for (int fd = 0; fd <= maxfd; ++fd) {
                if (FD_ISSET(fd, &readfds)) { // Got message!
                    std::cout << "message from fd num: " << fd <<std::endl;
                    long long number;
                    ssize_t bytesRead = recv(fd, &number, sizeof(number), 0);
                    if (bytesRead <= 0) {
                        // Handle disconnect or error
                        close(fd);
                        // Remove clientSocket from m_newConnectionsQueue if needed
                        // Note: You might need to implement a method to remove a specific element from TSQueue
                    } else {
                        std::cout << "got " << number << std::endl;
                        // give the buffer time to clean
                        struct timespec ts;
                        ts.tv_sec = 2;
                        nanosleep(&ts, NULL);  // Sleep 2 seconds
                        // Push the message (number) into the ActiveObject processing queue.
                        m_processingQueue.push({fd, number});
                    }
                }
            }
        }
    }
}


void Pipeline::processMessages(std::pair<int, long long> data) {
    int clientFd = data.first;
    long long number = data.second;


    bool prime = isPrime(number);
    std::string numStr = std::to_string(number);
    const char* response = prime ? "YES" : "NO";

    // Concatenate the number string and the response string
    std::string combinedResponse = numStr + " " + response;

    // Send the concatenated string over the socket
    send(clientFd, combinedResponse.c_str(), combinedResponse.length(), 0);
}

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

    int port = std::stoi(argv[1]);

    Pipeline pipeline; // Create an instance of Pipeline
    pipeline.startServer(port); // Call startServer on the instance

    return 0;
}