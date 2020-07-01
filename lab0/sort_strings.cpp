/*
 * ECEE 2160 Lab Assignment 0.3: Sorting an array of strings
 *
 * Author:  Brian Schubert
 * Date:    2020-07-01
 *
 * References
 * ==========
 *
 * - http://www.cplusplus.com/reference/string/string/compare/
 *
 */
#include <array>            // for std::array
#include <iostream>         // for std::cout, std::cin
#include <string>           // for std::string

#include "lab0_utils.h"

/// The number of strings to read from stdin.
constexpr std::size_t STRING_COUNT{10};

int main()
{
    // Array for storing the user provided strings.
    std::array<std::string, STRING_COUNT> input_strings;

    std::cout << "Enter " << STRING_COUNT << " whitespace separated strings: ";

    for (std::size_t i{0}; i < STRING_COUNT; ++i) {
        if (!(std::cin >> input_strings[i])) {
            // Print an error message and exit with nonzero status if we fail
            // to read a string.
            std::cerr << "Invalid input";
            return EXIT_SUCCESS;
        }
    }

    // Print the unsorted strings to stdout.
    std::cout << "\nUnsorted Strings:\n=======================\n";
    print_iter(std::cout, input_strings.begin(), input_strings.end(), ",\n");

    // Sort the strings alphabetically.
    //
    // std::strings are compared lexicographically by default, so simply passing
    // the input_strings array to the sorting function from lab assignment 0.2
    // will suffice.
    selection_sort_array(input_strings.data(), input_strings.size());

    // Print the sorted strings to stdout.
    std::cout << "\n\nSorted Strings:\n=======================\n";
    print_iter(std::cout, input_strings.begin(), input_strings.end(), ",\n");

    return EXIT_SUCCESS;
}
