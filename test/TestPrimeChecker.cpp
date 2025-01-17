#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this once per test-runner!

#include "../lib/catch.hpp"
#include "../templates/PrimeChecker.hpp"

// Test case to check if PrimeChecker correctly identifies a prime number.
TEST_CASE("Test PrimeChecker 1.") {
    PrimeChecker pc;
    REQUIRE(pc.isPrime(5));
}