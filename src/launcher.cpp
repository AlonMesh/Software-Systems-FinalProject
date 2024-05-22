#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <vector>
#include <sys/wait.h>
#include <signal.h>

std::vector<pid_t> childPIDs;

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
    pid_t pid = fork();
    if (pid == 0) { // Child process
        // Construct the command to launch a client using exec
        std::string command = "./build/client ";
        command += serverAddress;
        command += " " + std::to_string(port);
        command += " " + std::to_string(seed);
        command += " " + std::to_string(delay);

        // Convert command string to char array for exec
        std::vector<char> commandVec(command.begin(), command.end());
        commandVec.push_back('\0');

        execl("/bin/sh", "sh", "-c", commandVec.data(), (char *) NULL);
        // If exec fails
        perror("execl");
        exit(1);
    } else if (pid > 0) { // Parent process
        childPIDs.push_back(pid);
    } else {
        perror("fork");
    }
}

void signalHandler(int signum) {
    std::cout << "Caught signal " << signum << ", terminating clients..." << std::endl;
    for (pid_t pid : childPIDs) {
        kill(pid, SIGTERM);
    }
    exit(signum);
}

int main(int argc, char *argv[]) {
    // Register signal handler for SIGINT
    signal(SIGINT, signalHandler);

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
        int seed = i + 1; // Unique seed for each client according to the index
        launchClient(serverAddress, port, seed, delay);
    }

    std::cout << "Launcher has started with " << numClients << " clients." << std::endl;

    // Wait for child processes to finish
    for (pid_t pid : childPIDs) {
        waitpid(pid, NULL, 0);
    }

    return 0;
}
