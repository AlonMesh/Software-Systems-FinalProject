#ifndef SIEVE_HPP
#define SIEVE_HPP

#include "io.hpp"
#include <vector>

/**
 * @brief Run the Sieve of Eratosthenes algorithm.
 *
 * This function runs the Sieve of Eratosthenes algorithm to find all prime numbers less than or equal to the given limit.
 *
 * @param limit The limit to find prime numbers up to.
 */
std::vector<long long> sieveOfEratosthenes(long long limit);

/**
 * @brief Validate the input.
 *
 * This function validates the input to ensure it is greater than 1.
 *
 * @param limit The limit to validate.
 * @return True if the input is valid, false otherwise.
 */
bool validateInput(long long limit);

#endif // SIEVE_HPP