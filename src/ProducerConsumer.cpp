#include "ProducerConsumer.hpp"
#include <iostream>
#include <string>

// Method to start sieving before processing requests
void ProducerConsumer::startSieving() {
    // Start sieving
    std::cout << "Starting sieving process..." << std::endl;

    // Call the sieveOfEratosthenes function to find prime numbers and store the result in isPrimeVector
    isPrimeVector = sieveOfEratosthenes(MILLION);

    // Turn isSieving flag to false to stop the sieving process
    isSieving = false;
    std::cout << "Sieving process complete." << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << "<port>" << std::endl;
        return 1;
    }

    int port = std::stoi(argv[1]);

    // Create an instance of the ProducerConsumer clas
    ProducerConsumer producerConsumer;

    // Start the sieving process in a separate thread


    // Start the server
    producerConsumer.startServer(port);

    return 0;
}
