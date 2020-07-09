/*
 * ECEE 2160 Lab Assignment 1 main file.
 *
 * Author:  Brian Schubert
 * Date:    2020-07-08
 *
 */


// http://www.cplusplus.com/reference/typeinfo/type_info/name/
// cpp reference strings views


#include "double_vec.h"

#include <array>        // for std::array (used in menu)
#include <iostream>     // for std::cout, std::cin
#include <limits>       // for std::numeric_limits
#include <string_view>  // for std::string_view

// For access to string view literals
using std::string_view_literals::operator ""sv;

// Using anonymous namespace to given symbols internal linkage.
namespace {

void test() {}

/// Program menu from lab instructions.
constexpr auto PRELAB_MENU = std::array{
    "Print the array"sv,
    "Append element at the end"sv,
    "Remove list element"sv,
    "Insert one element"sv,
    "Exit"sv,
};

/**
 * Runs a continuous interactive program that allows the user to manipulate
 * the provided vector.
 *
 * @param vec The vector to be interacted with.
 */
void run_vector_interactive(DoubleVec& vec);

/**
 * Prints the specified prompt to the standard output and reads a T value
 * from the standard input.
 *
 * If the user provides an invalid T value, the standard input will be cleared
 * and the user will be reprompted.
 *
 * @tparam T The type of the object to read.
 * @param prompt The message to be displayed to the user.
 * @return User provided integer.
 */
template<typename T>
T prompt_user(std::string_view prompt);

/**
 * Output stream operator for DoubleVec
 *
 * This operator replaces the function `printArray` from the lab instructions.
 */
std::ostream& operator<<(std::ostream& out, const DoubleVec& vec);

} // end namespace

int main()
{
    DoubleVec vec{}; // Default construct

    run_vector_interactive(vec);
}


// Internal definitions.
namespace {
void run_vector_interactive(DoubleVec& vec)
{
    while (true) {
        // Print the menu from the lab instructions.
        std::cout << "Main menu:\n\n";
        for (std::size_t i{0}; i < PRELAB_MENU.size(); ++i) {
            std::cout << (i + 1) << ". " << PRELAB_MENU[i] << '\n';
        }
        std::cout << '\n';

        // Prompt user for menu selection.
        auto user_selection = prompt_user<int>("Selection an option: ");

        // Menu selection logic implemented with switch per lab instructions.
        switch (user_selection) {
            case 1: { // Print array
                std::cout << vec;
                break;
            }
            case 2: { // Append element
                auto new_element = prompt_user<double>("Enter the new element: ");
                vec.append(new_element);
                break;
            }
            case 3: { // Remove element
                auto removed = vec.pop();
                if (removed) {
                    std::cout << "Removed " << removed.value() << '\n';
                } else {
                    std::cout << "Cannot remove element - vector is empty\n";
                }
                break;
            }
            case 4: { // Insert element
                auto index = prompt_user<std::size_t>("Enter the index of the new element: ");
                auto new_element = prompt_user<double>("Enter the new element: ");

                if (index > vec.count()) {
                    std::cout
                        << "Invalid index - index cannot exceed the length of the vector ("
                        << vec.count() << ")\n";
                } else {
                    vec.insert(index, new_element);
                }
                break;
            }
            case 5: { // Exit
                std::cout << "Exiting...";
                return;
            }
            default: {
                // Print and immediately flush error message.
                std::cout
                    << "Invalid selection - selection must be an integer from [1,"
                    << PRELAB_MENU.size() << ']' << std::endl;
            }
        }

        std::cout << '\n';

    }
}

template<typename T>
T prompt_user(const std::string_view prompt)
{
    T user_selection;
    while (true) {
        std::cout << prompt;
        // operator>>(std::istream&, T) returns false on input failure.
        if (!(std::cin >> user_selection)) {
            // Print and immediately flush error message.
            std::cout << "Invalid selection - input must be a valid GCC type '"
                      << typeid(T).name() << "'" << std::endl;
            // Clear the error
            std::cin.clear();
            // Empty the input stream until a newline character is found.
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        return user_selection;
    }
}

std::ostream& operator<<(std::ostream& out, const DoubleVec& vec)
{
    out << "[ ";
    for (const auto& elem : vec) {
        out << elem << ' ';
    }
    out << ']';
    return out;
}
}
