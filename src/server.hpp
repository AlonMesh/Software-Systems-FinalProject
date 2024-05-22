#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/select.h>
#include <sys/types.h>  

/**
 * @brief Start the server.
 *
 * This function starts the server and listens for incoming connections.
 *
 * @param port The port of the server.
 */
void startServer(int port);

/**
 * @brief Handle the client.
 *
 * This function handles the client by receiving a number from the client and sending a response.
 *
 * @param masterSet The set of file descriptors.
 * @param sd The socket descriptor.
 */
void handleClient(fd_set& masterSet, int sd);

/**
 * @brief Check if a number is prime.
 *
 * This function checks if a number is prime.
 *
 * @param number The number to check.
 * @return True if the number is prime, false otherwise.
 */
bool isPrime(long long number);

#endif // SERVER_HPP
