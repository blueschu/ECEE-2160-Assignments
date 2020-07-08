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

#include <iostream>     // for std::cout, std::cin
#include <string_view>  // for std::string_view
#include <limits>       // for std::numeric_limits


// Using anonymous namespace to given symbols internal linkage.
namespace {

/// The number of options in the menu.
constexpr std::size_t PRELAB_MENU_CHOICES{5};

/// Program menu from lab instructions.
constexpr std::string_view PRELAB_MENU =
    "Main menu\n\n"
    "1. Print the array\n"
    "2. Append element at the end\n"
    "3. Remove list element\n"
    "4. Insert one element\n"
    "5. Exit\n";

/**
 * Global variables for the properties of the lab vector.
 *
 * From lecture: these global variables may be substituted with a structure
 * after the prelab.
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

}

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
        // Print menu with extra empty line.
        std::cout << PRELAB_MENU << '\n';

        // Prompt user for menu selection.
        int user_selection = prompt_user_integer("Selection an option: ");

        // Menu selection logic implemented with switch per lab instructions.
        switch (user_selection) {
            case 1: {
                std::cout << "(print array)\n";
                break;
            }
            case 2: {
                std::cout << "(append element)\n";
                break;
            }
            case 3: {
                std::cout << "(remove element)\n";
                break;
            }
            case 4: {
                std::cout << "(insert element)\n";
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
                    << PRELAB_MENU_CHOICES
                    << std::endl;
            }
        }

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
