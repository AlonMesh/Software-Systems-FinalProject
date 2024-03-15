#ifndef MAIN_CPP
#define MAIN_CPP

#include "io.hpp"
#include "sieve.hpp"

/**
 * @brief Main function of the program.
 *
 * This function handles command-line arguments, parses input, and orchestrates the overall flow of the program.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return The exit status of the program.
 */
int main(int argc, char *argv[]) {
    // Check if the number of command-line arguments is correct.
    if (argc != 2) {
        printUsage(argv[0]);
        return 1;
    }

    // Convert the command-line argument to a long long.
    long long limit = convertToLongLong(argv[1]);

    // Check if the input is valid (i.e. greater than 1).
    if (limit <= 1) {
        printErrorMessage("Please enter a number greater than 1.");
        return 1;
    }

    // Run the Sieve of Eratosthenes algorithm.
    sieveOfEratosthenes(limit);

    // Exit the program successfully.
    return 0;
}

#endif // MAIN_CPP