/*
 * ECEE 2160 Lab Assignment 1 Prelab
 *
 * Author:  Brian Schubert
 * Date:    2020-07-07
 *
 * References:
 * ===========
 *
 *  - https://en.cppreference.com/w/cpp/io/basic_istream/ignore
 */

#include <array>        // for std::array (used in menu)
#include <iostream>     // for std::cout, std::cin
#include <limits>       // for std::numeric_limits
#include <string_view>  // for std::string_view


// Using anonymous namespace to given symbols internal linkage.
namespace {

/// Program menu from lab instructions.
constexpr std::array<std::string_view, 5> PRELAB_MENU{
    "Print the array",
    "Append element at the end",
    "Remove list element",
    "Insert one element",
    "Exit",
};

/**
 * Global variables for the properties of the lab vector.
 *
 * From lecture: these global variables may be substituted with a structure
 * or class after the prelab.
 */
struct {
    double* values;
    std::size_t count;
    std::size_t size;
} g_lab_vector{};

/**
 * Initializes the global vector.
 *
 * This function should only be called once. Repeated calls will have no effect.
 */
void initialize();

/**
 * Deallocates the memory held by the global vector and zeros its
 * member variables.
 *
 * This function should only be called once. Repeated calls will have no effect.
 */
void finalize();

/**
 * Runs a continuous interactive menu until the user chooses to exit.
 */
void run_program_loop();

/**
 * Prints the specified prompt to the standard output and reads an integer
 * from the standard input.
 *
 * If the user provides an invalid integer, the standard input will be cleared
 * and the user will be reprompted.
 *
 * @param prompt The message to be displayed to the user.
 * @return User provided integer.
 */
int prompt_user_integer(std::string_view prompt);

} // end namespace

int main()
{
    // Initialize the global vector's state.
    initialize();

    // Start the interactive menu.
    run_program_loop();

    // Deallocate the global vector.
    finalize();
    return 0;
}

// Internal definitions.
namespace {
void initialize()
{
    constexpr std::size_t DEFAULT_SIZE{2};

    // Make sure g_lab_vector hasn't already been initialized
    if (g_lab_vector.values != nullptr) {
        g_lab_vector.values = new double[DEFAULT_SIZE];
        g_lab_vector.count = 0;
        g_lab_vector.size = DEFAULT_SIZE;
    }
}

void finalize()
{
    delete[] g_lab_vector.values;
    g_lab_vector.values = nullptr;
    g_lab_vector.count = 0;
    g_lab_vector.size = 0;

}

void run_program_loop()
{
    while (true) {
        // Print the menu from the lab instructions.
        std::cout << "Main menu\n\n";
        for (std::size_t i{0}; i < PRELAB_MENU.size(); ++i) {
            std::cout << (i + 1) << ". " << PRELAB_MENU[i] << '\n';
        }
        std::cout << '\n';

        // Prompt user for menu selection.
        int user_selection = prompt_user_integer("Selection an option: ");

        // Menu selection logic implemented with switch per lab instructions.
        switch (user_selection) {
            case 1:
            case 2:
            case 3:
            case 4: {
                std::cout << "You selected \"" << PRELAB_MENU[user_selection - 1] << "\"\n";
                break;
            }
            case 5: {
                std::cout << "Exiting...";
                return;
            }
            default: {
                // Print and immediately flush error message.
                std::cerr
                    << "Invalid selection - selection must be an integer from [0,"
                    << PRELAB_MENU.size()
                    << std::endl;
            }
        }

        std::cout << '\n';

    }
}

int prompt_user_integer(const std::string_view prompt)
{
    int user_selection;
    while (true) {
        std::cout << prompt;
        // operator>>(std::istream&, T) returns false on input failure.
        if (!(std::cin >> user_selection)) {
            // Print and immediately flush error message.
            std::cerr << "Invalid selection - input must be an integer" << std::endl;
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
