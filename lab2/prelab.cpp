/*
 * ECEE 2160 Lab Assignment 2 prelab part II.
 *
 * Author:  Brian Schubert
 * Date:    2020-07-15
 *
 */

#include <array>        // for std::array (used in menu)
#include <iostream>     // for std::cout, std::cin
#include <limits>       // for std::numeric_limits
#include <string_view>  // for std::string_view

// For access to string view literals
using std::string_view_literals::operator ""sv;

// Using anonymous namespace to given symbols internal linkage.
namespace {

/**
 * Forward declaration of the list structure.
 *
 * We omit the definitions from the provided personList.cpp file
 * since they are not used during the prelab.
 */
struct List;

/// Program menu from lab instructions.
constexpr auto PRELAB_MENU = std::array{
    "Add a person"sv,
    "Find a person"sv,
    "Remove a person"sv,
    "Print the list"sv,
    "Exit"sv,
};

/**
 * Runs a continuous interactive program that allows the user to manipulate
 * the provided list.
 *
 * @param list The list to be interacted with.
 */
void run_list_interactive(List*);

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

} // end namespace

int main()
{
    // list is never used, so we can safely set it as null for the prelab.
    List* list{nullptr};

    run_list_interactive(list);
}


// Internal definitions.
namespace {
void run_list_interactive(List*)
{
    while (true) {
        // Print the menu from the lab instructions.
        std::cout << "====== Menu ======\n";
        for (std::size_t i{0}; i < PRELAB_MENU.size(); ++i) {
            std::cout << (i + 1) << ". " << PRELAB_MENU[i] << '\n';
        }
        std::cout << '\n';

        // Prompt user for menu selection.
        auto user_selection = prompt_user<int>("Select an option: ");

        // Menu selection logic implemented with switch per lab instructions.
        switch (user_selection) {
            case 1:
            case 2:
            case 3:
            case 4: {
                std::cout << "You selected \"" << PRELAB_MENU[user_selection - 1] << "\"\n";
                break;
            }
            case 5: { // Exit
                std::cout << "Exiting...\n";
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
        // We assume EOF will not be reached.
        if (!(std::cin >> user_selection)) {
            // Print and immediately flush error message.
            std::cout << "Invalid input" << std::endl;
            // Clear the error
            std::cin.clear();
            // Empty the input stream until a newline character is found.
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        return user_selection;
    }
}
}
