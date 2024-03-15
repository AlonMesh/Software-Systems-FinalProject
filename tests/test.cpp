#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#include "../src/sieve.hpp"

TEST_CASE("sieveOfEratosthenes")
{
    SUBCASE("Test with limit 50")
    {
        // Test the sieve of Eratosthenes algorithm.
        CHECK(sieveOfEratosthenes(10) == std::vector<long long>{2, 3, 5, 7});
        CHECK(sieveOfEratosthenes(20) == std::vector<long long>{2, 3, 5, 7, 11, 13, 17, 19});
        CHECK(sieveOfEratosthenes(30) == std::vector<long long>{2, 3, 5, 7, 11, 13, 17, 19, 23, 29});
        CHECK(sieveOfEratosthenes(40) == std::vector<long long>{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37});
        CHECK(sieveOfEratosthenes(50) == std::vector<long long>{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47});
    }

    SUBCASE("Handle edge cases")
    {
        // Test the sieve of Eratosthenes algorithm with edge cases.
        CHECK(sieveOfEratosthenes(1) == std::vector<long long>{});
        CHECK(sieveOfEratosthenes(2) == std::vector<long long>{2});
        CHECK(sieveOfEratosthenes(3) == std::vector<long long>{2, 3});
    }

    SUBCASE("Handles invalid input")
    {
        // Test the sieve of Eratosthenes algorithm with invalid input.
        CHECK(sieveOfEratosthenes(0) == std::vector<long long>{});
        CHECK(sieveOfEratosthenes(-1) == std::vector<long long>{});
    }

    // SUBCASE("Test with large limit")
    // {
    //     // Test the sieve of Eratosthenes algorithm with a large limit.
    //     CHECK(sieveOfEratosthenes(1000000).size() == 78498);
    // }

    // SUBCASE("Test with very large limit")
    // {
    //     // Test the sieve of Eratosthenes algorithm with a very large limit.
    //     CHECK(sieveOfEratosthenes(1000000000).size() == 50847534);
    // }
}