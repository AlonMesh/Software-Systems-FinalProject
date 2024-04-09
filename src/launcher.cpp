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
    // Construct the command to launch a client using bash
    std::string command = "./build/client ";
    command += serverAddress;
    command += " " + std::to_string(port);
    command += " " + std::to_string(seed);
    command += " " + std::to_string(delay);
    command += " &";  // Run the client in the background
    
    // Execute the command using system()
    int result = system(command.c_str());
    
    if (result != 0) {
        std::cerr << "Failed to launch client using bash with command: `" << command << "`" << std::endl;
    }
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

    std::cout << "Launcher has started with " << numClients << " clients." << std::endl;

    return 0;
}
