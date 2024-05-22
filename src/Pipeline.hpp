#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "TSQueue.hpp"
#include "ActiveObject.hpp"

#include <queue>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <string>

/**
 * @brief Pipeline class for managing client connections and processing messages.
 * 
 * This class manages client connections and processes messages using multiple threads. It listens for new
 * connections, receives messages, and processes them to check if numbers are prime.
 */
class Pipeline {
public:
    /**
     * @brief Starts the server on the specified port.
     * 
     * This function starts the server by creating a socket, binding it to the specified port, and listening
     * for incoming connections. It also starts threads for listening to connections and receiving messages.
     * 
     * @param port The port number on which the server will listen.
     */
    void startServer(int port);

private:
    /**
     * @brief Listens for new client connections and adds them to the new connections queue.
     * 
     * @param serverSocket The server socket file descriptor.
     */
    void listenForConnections(int serverSocket);

    /**
     * @brief Receives messages from clients, extracts numbers, and adds them to the processing queue.
     */
    void receiveMessages();

    /**
     * @brief Processes messages from the queue.
     * 
     * This function processes messages by checking if the received number is prime and sends the result back
     * to the client.
     * 
     * @param data A pair containing the client file descriptor and the number to be processed.
     */
    void processMessages(std::pair<int, long long> data);

    /**
     * @brief Checks if a number is prime.
     * 
     * @param number The number to be checked.
     * @return True if the number is prime, false otherwise.
     */
    bool isPrime(long long number);

    std::queue<int> m_newConnectionsQueue;                // Queue for storing new client connections
    TSQueue<std::pair<int, long long>> m_processingQueue; // Thread-safe queue for processing messages
    bool m_running;                                       // Indicates if the server is running
};

#endif // PIPELINE_HPP
