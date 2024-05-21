#include "sieve.hpp"
#include <cmath>
#include <iostream> // TODO: Remove this line.

// Define the maximum limit as 12^10
#define MAX_LIMIT 61917364224

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

bool validateInput(long long limit)
{
    if (limit <= 1) {
        printErrorMessage("Please enter a number greater than 1.");
        return false;
    }

    if (limit > MAX_LIMIT) {
        printErrorMessage("Please enter a number less than or equal to 61917364224.");
        return false;
    }

    return true;
}

std::vector<bool> sieveOfEratosthenes(long long limit) {
    // Check if the input is valid.
    if (!validateInput(limit)) {
        return;
    }

    // Create a boolean vector to store whether a number is prime or not in the range [0, limit + 1]
    std::vector<bool> isPrimeVector(limit + 1, true);

    // Resize the isPrime vector to handle numbers up to the limit.
    isPrimeVector.resize(limit + 1, true);

    // Mark 0 and 1 as not prime.
    isPrimeVector[0] = isPrimeVector[1] = false;

    // Traverse all numbers smaller than or equal to the square root of the limit.
    for (long long p = 2; p * p <= limit; p++) {
        // If isPrime[p] is not changed, then it is a prime.
        if (isPrimeVector[p]) {
            // Update all multiples of p.
            for (long long i = p * p; i <= limit; i += p) {
                isPrimeVector[i] = false;
            }
        }
    }

    // Return the vector of prime flags.
    return isPrimeVector;
}
