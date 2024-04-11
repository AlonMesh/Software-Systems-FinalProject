#ifndef PIPELINE_HPP
#define PIPELINE_HPPvvvvv

#include "TSQueue.hpp"
#include "ActiveObject.hpp"

class Server {
public:
    void stop();
    int startServer(int port);

private:
    int setupSocket(int port);
    void listenForConnections();
    void receiveMessages();
    void processMessages();
    bool isPrime(long long number);

    int m_serverSocket;
    TSQueue<int> m_newConnectionsQueue;
    TSQueue<std::pair<int, std::string>> m_processingQueue;
    std::vector<ActiveObject<int>> m_activeObjects;
    bool m_running;
};

#endif // PIPELINE_HPP
