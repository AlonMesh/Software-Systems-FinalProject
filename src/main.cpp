#ifndef MAIN_CPP
#define MAIN_CPP

#include "io.hpp"
#include "sieve.hpp"

int main(int argc, char *argv[]) {
    // Check if the number of command-line arguments is correct.
    if (argc != 2) {
        printUsage(argv[0]);
        return 1;
    }

    // Convert the command-line argument to a long long.
    long long limit = convertToLongLong(argv[1]);

    // Run the Sieve of Eratosthenes algorithm.
    sieveOfEratosthenes(limit);

    // Exit the program successfully.
    return 0;
}

#endif // MAIN_CPP