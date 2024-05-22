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
    printSieveOfEratosthenes(limit);

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

void printSieveOfEratosthenes(long long limit)
{
    // Check if the input is valid.
    if (!validateInput(limit)) {
        return;
    }

    // Create a boolean array "prime[0..limit]" and initialize all entries as true.
    std::vector<bool> prime(limit + 1, true);

    // Traverse all numbers smaller than or equal to the square root of the limit.
    for (long long p = 2; p * p <= limit; p++)
    {
        // If prime[p] is not changed, then it is a prime.
        if (prime[p])
        {
            // Update all multiples of p.
            for (long long i = p * p; i <= limit; i += p)
            {
                prime[i] = false;
            }
        }
    }

    // Create a vector to store the prime numbers.
    std::vector<long long> primes;

    // Add all prime numbers to the vector.
    for (long long p = 2; p <= limit; p++)
    {
        if (prime[p])
        {
            primes.push_back(p);
        }
    }

    // Print the prime numbers.
    for (long long p : primes)
    {
        std::cout << p << std::endl;
    }
}