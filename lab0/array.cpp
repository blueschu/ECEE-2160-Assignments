/*
 * ECEE 2160 Lab Assignment 0.2: Sorting an array of integers
 *
 * Author:  Brian Schubert
 * Date:    2020-07-01
 *
 */

#include "lab0_utils.h"

#include <array>            // for std::array
#include <ctime>            // for std::time
#include <functional>       // for std::function
#include <iostream>         // for std::cout, std::cin

// Suppress linter warnings for using std::rand instead of STL random library.
#pragma ide diagnostic ignored "cert-msc50-cpp"

// Using anonymous namespace to given symbols internal linkage.
namespace {

/// Minimum integer boundary from assignment instructions.
constexpr int RANDOM_MIN{0};

/// Maximum integer boundary from assignment instructions.
constexpr int RANDOM_MAX{99};

/// Array size from assignment instructions.
constexpr std::size_t ARRAY_SIZE{10};

/**
 * Generates a random integer from the range [min, max].
 *
 * @param min Minimum bound.
 * @param max Maximum bound
 * @return Random integer from the provided interval.
 */
int weak_random_int(int min, int max)
{
    // Use std::rand instead of STL random library for simplicity.
    return std::rand() % (max - min + 1) + min;
}

/**
 * Assigns a value to each element of the given iterator using the provided
 * generator function (or callable).
 *
 * This function imitates the behavior std::generator from the STL algorithm
 * library. STL use for this assignment was limited.
 *
 * @tparam I Iterator type.
 * @tparam T Generator type.
 * @param iter Forward iterator indicating start location.
 * @param end Forward iterator indicating end location.
 * @param generator Generator callable.
 */
template<class I, class G>
void fill_iter(I iter, I end, G generator)
{
    while (iter != end) {
        // Write an element yielded from the generator to the current iterator.
        *iter = generator();
        ++iter;
    }
}

} // end namespace

int main()
{
    // Initialize the C standard library pseudo random number generator
    // using the current system time.
    {
        unsigned int seed = std::time(nullptr);
        srand(seed);
    }

    // Function object for generating random numbers.
    const std::function<int()> random_number_generator = []() {
        return weak_random_int(RANDOM_MIN, RANDOM_MAX);
    };

    // Generate an array of uniformly distributed random numbers.
    std::array<int, ARRAY_SIZE> random_array{};
    fill_iter(random_array.begin(), random_array.end(), random_number_generator);

    // Print the array of random integers to stdout.
    std::cout << "Random integers from [" << RANDOM_MIN << ',' << RANDOM_MAX << "]:          ";
    print_iter(std::cout, random_array.begin(), random_array.end(), ", ");

    // Sort the array of random integers.
    selection_sort_array(random_array.data(), random_array.size());

    // Print the sorted array to stdout.
    std::cout << "\nRandom integers from [" << RANDOM_MIN << ',' << RANDOM_MAX << "] (sorted): ";
    print_iter(std::cout, random_array.begin(), random_array.end(), ", ");
    std::cout << '\n';

    return 0;
}
