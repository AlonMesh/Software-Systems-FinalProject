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
 * This function validates the input to ensure that it is a valid number.
 * 
 * @param limit The limit to validate.
*/
bool validateInput(long long limit);

#endif // SIEVE_HPP