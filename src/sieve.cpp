#include "sieve.hpp"
#include <cmath>
#include <iostream> // TODO: Remove this line.

// Define the maximum limit as 12^10
#define MAX_LIMIT 61917364224

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

std::vector<long long> sieveOfEratosthenes(long long limit)
{
    // Check if the input is valid.
    if (!validateInput(limit)) {
        return {};
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

    // For debugging purposes, print the prime numbers. // TODO: Remove this line.
    for (long long p : primes)
    {
        std::cout << p << std::endl;
    }

    // Return the vector of prime numbers.
    return primes;
}
