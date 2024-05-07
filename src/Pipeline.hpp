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

class Pipeline {
public:
//    void stop();
    void startServer(int port);

private:
//    int setupSocket(int port);
    void listenForConnections(int serverSocket);
    void receiveMessages();
    void processMessages(std::pair<int, long long>);
    bool isPrime(long long number);

    std::queue<int> m_newConnectionsQueue;
    TSQueue<std::pair<int, long long>> m_processingQueue;
    bool m_running;
};

#endif // PIPELINE_HPP
