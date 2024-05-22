#ifndef PRODUCERCONSUMER_HPP
#define PRODUCERCONSUMER_HPP
#define MILLION 1000000 // 1 million
#define MILLION40 40000000 // 40 million

#include "TSQueue.hpp"
#include "ActiveObject.hpp"
#include <vector>
#include <thread>
#include <condition_variable>
#include <queue>
#include <string>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <ctime>
#include <string>

/**
 * @brief ProducerConsumer class for managing client connections and processing messages.
 * 
 * This class manages client connections and processes messages using multiple threads. It performs
 * the Sieve of Eratosthenes algorithm to determine prime numbers and handles client requests to check
 * if numbers are prime.
 */
class ProducerConsumer {
public:
    // Start the server and sieving process
    void startServer(int port);

private:
    // Start sieving for prime numbers
    void startSieving();

    // Listen for connections
    void listenForConnections(int serverSocket);

    // Receive messages from clients
    void receiveMessages();

    // Process messages using the precomputed sieve vector
    void processMessages(std::pair<int, long long> data);

    // Flag to determine if the sieving process is running
    bool isSieving = true;

    // Boolean Vector to store whether a number is prime or not
    std::vector<bool> isPrimeVector;

    // Condition variable and mutex for synchronization
    std::condition_variable condVar;
    std::mutex mtx;

    // Queues for managing new connections and processing messages
    std::queue<int> m_newConnectionsQueue;
    TSQueue<std::pair<int, long long>> m_processingQueue;

    // Flag to determine if the server is running
    bool m_running = false;
};

#endif // PRODUCERCONSUMER_HPP
