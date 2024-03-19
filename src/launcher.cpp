#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <vector>
#include <sys/wait.h>

bool validateArguments(int argc, char* argv[]) {
    // Check if the number of arguments is correct
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <server address> <port> <num clients> <delay>" << std::endl;
        return false;
    }

    // Check if the port number is valid
    int port = std::stoi(argv[2]);

    if (port < 1024 || port > 65535) {
        std::cerr << "Port number must be between 1024 and 65535" << std::endl;
        return false;
    }

    // Check if the number of clients is valid
    int numClients = std::stoi(argv[3]);

    if (numClients < 1) {
        std::cerr << "Number of clients must be at least 1" << std::endl;
        return false;
    }

    // Check if the delay is valid
    int delay = std::stoi(argv[4]);

    if (delay < 0) {
        std::cerr << "Delay must be non-negative" << std::endl;
        return false;
    }

    return true;
}

void launchClient(const char* serverAddress, int port, int seed, int delay) {
    // Fork a new process
    pid_t pid = fork();
    
    if (pid == 0) {
        // Child process: Execute client
        // Convert port, seed, and delay to strings
        std::string portStr = std::to_string(port);
        std::string seedStr = std::to_string(seed);
        std::string delayStr = std::to_string(delay);
        
        // Execute client
        execl("./build/client", "client", serverAddress, portStr.c_str(), seedStr.c_str(), delayStr.c_str(), (char *)NULL);

        // If execl returns here, it means the client failed to launch
        std::cerr << "Failed to launch client" << std::endl;
        exit(1);
    } else if (pid < 0) {
        // Failed to fork
        std::cerr << "Failed to fork" << std::endl;
    }
    // Parent process does nothing here (else) and returns
}

int main(int argc, char *argv[]) {
    // Validate command-line arguments
    if (!validateArguments(argc, argv)) {
        return 1;
    }

    // Extract command-line arguments into variables
    const char* serverAddress = argv[1];
    int port = std::stoi(argv[2]);
    int numClients = std::stoi(argv[3]);
    int delay = std::stoi(argv[4]);

    for (int i = 0; i < numClients; ++i) {
        int seed = i + 1; // Unique seed for each client
        launchClient(serverAddress, port, seed, delay);
    }

    // Optionally wait for all clients to finish
    for (int i = 0; i < numClients; ++i) {
        wait(NULL);
    }

    std::cout << "Launcher has started with " << numClients << " clients." << std::endl;

    return 0;
}
