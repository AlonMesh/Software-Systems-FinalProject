#ifndef LAUNCHER_HPP
#define LAUNCHER_HPP

/**
 * Validates the command-line arguments for the launcher.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return True if the arguments are valid, false otherwise.
 */
bool validateArguments(int argc, char* argv[]);

/**
 * Launches a client process.
 * @param serverAddress IP address of the server.
 * @param port Port number of the server.
 * @param seed Seed value for the random number generator used by the client.
 * @param delay Delay (in seconds) between requests made by the client.
 */
void launchClient(const char* serverAddress, int port, int seed, int delay);

#endif // LAUNCHER_HPP
