/*
 * ECEE 2160 Lab Assignment 0.2: Sorting an array of integers
 *
 * Author:  Brian Schubert
 * Date:    2020-07-01
 *
 * References
 * ==========
 *
 *  - http://www.cplusplus.com/reference/random/
 *  - http://www.cplusplus.com/reference/algorithm/
 *  - https://stackoverflow.com/questions/5586377/
 */
#include <array>            // for std::array
#include <ctime>            // for std::time
#include <functional>       // for std::function
#include <iostream>         // for std::cout, std::cin
#include <string_view>      // for std::string_view
#include <utility>          // for std::swap

// Suppress warnings for using std::rand instead of STL random library.
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
 * Generate a random integer from the range [min, max].
 *
 * @param min Minimum bound.
 * @param max Maximum bound
 * @return Random integer from the provided interval.
 */
int weak_random_int(int min, int max)
{
    return std::rand() % (max - min + 1) + min;
}

/**
 * Generic function for printing any C++ iterating to the given output stream
 * with elements separated by the given delimiter.
 *
 * The delimiter is taken as a std::string_view so that C-style strings,
 * std::string, etc. can be passed without manual convesions.
 *
 * @tparam I Iterator type.
 * @param out Output stream to be written to.
 * @param iter Forward iterator indicating start location.
 * @param end Forward iterator indicating end location.
 * @param delim String view of output delimiter.
 */
template<class I>
void print_iter(std::ostream& out, I iter, I end, const std::string_view delim)
{
    // Make sure the received iterator isn't already at its end.
    if (iter == end) {
        return;
    }
    // Track the previously yielded element from the iterator while consuming.
    I prev = iter;

    // Advance the iterator and check to see if it has reached the end.
    while (++iter != end) {
        // If the end has not been reached, print the previous element with
        // a trailing delimiter.
        out << *prev << delim;
        // Update the previous element with the current element.
        prev = iter;
    }
    // Print the final element without a trailing delimiter.
    out << *prev;
}

/**
 * Implementation of selection sort.
 *
 * This implementation occurs in O(1) space and O(n^2) time.
 *
 * @tparam T Array content type.
 * @param values Mutable pointer to array contents.
 * @param size Array length.
 */
template<class T>
void selection_sort(T* values, std::size_t size)
{
    for (std::size_t i{0}; i < size; ++i) {
        // Store both the value and index of the  minimum unsorted element.
        auto min_element{values[i]};
        auto min_index{i};

        // Locate the index of the minimum element in the unsorted porition.
        for (auto j{i + 1}; j < size; ++j) {
            if (values[j] < min_element) {
                min_element = values[j];
                min_index = j;
            }
        }

        // Swap the first unsorted element with the located minimum.
        std::swap(values[i], values[min_index]);
    }
}

/**
 * Implementation of selection sort.
 *
 * This implementation occurs in O(1) space and O(n^2) time.
 *
 * @tparam T Array content type.
 * @param values Mutable poitner to array contents.
 * @param size Array length.
 * @param key_selector A function mapping a T to some comparable key K.
 */
template<class T, class K>
void selection_sort_by(T* values, std::size_t size, std::function<K(T)> key_selector)
{
    for (std::size_t i{0}; i < size; ++i) {
        // Store both the value and index of the  minimum unsorted element.
        auto min_element{values[i]};
        auto min_index{i};

        // Locate the index of the minimum element in the unsorted portion.
        for (auto j{i + 1}; j < size; ++j) {
            // Store the key selector's result on values[j] so we don't
            // compute it twice
            auto current_key = key_selector(values[j]);

            if (current_key < key_selector(min_element)) {
                min_element = current_key;
                min_index = j;
            }
        }

        // Swap the first unsorted element with the located minimum.
        std::swap(values[i], values[min_index]);
    }
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

    // Sort the array of random integers
    selection_sort(random_array.data(), random_array.size());     // Using custom selection sort

    // Print the sorted array to stdout.
    std::cout << "\nRandom integers from [" << RANDOM_MIN << ',' << RANDOM_MAX << "] (sorted): ";
    print_iter(std::cout, random_array.begin(), random_array.end(), ", ");

    return EXIT_SUCCESS;
}
