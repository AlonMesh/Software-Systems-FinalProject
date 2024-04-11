#ifndef CLIENT_HPP
#define CLIENT_HPP

/**
 * @brief Start the client.
 *
 * This function starts the client and connects to the server.
 *
 * @param serverAddress The address of the server.
 * @param port The port of the server.
 * @param seed The seed for the random number generator.
 * @param timeSkip The time to skip between requests.
 */
void startClient(const char* serverAddress, int port, unsigned int seed, int timeSkip);

/**
 * @brief Validate command-line arguments.
 *
 * This function validates the command-line arguments.
 *
 * @param argc The number of arguments.
 * @param argv The arguments.
 * @return True if the arguments are valid, false otherwise.
 */
bool validateArguments(int argc, char* argv[]);

#endif // CLIENT_HPP