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

#endif // CLIENT_HPP