#ifndef PRODUCERCONSUMER_HPP
#define PRODUCERCONSUMER_HPP
#define MILLION 1000000 // 1 million

#include "Pipeline.hpp"
#include "sieve.hpp"  // Include the sieve header file for the sieveOfEratosthenes function


class ProducerConsumer : public Pipeline {
    public:
    // Start sieving for prime numbers
    void startSieving();

    private:
    // Flag to determine if the sieving process is running
    bool isSieving = true;
    
    // Boolean Vector to store whether a number is prime or not
    std::vector<bool> isPrimeVector;
};

#endif // PRODUCERCONSUMER_HPP