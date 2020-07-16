/*
 * ECEE 2160 Lab Assignment 2 main file
 *
 * Author:  Brian Schubert
 * Date:    2020-07-16
 *
 * References
 * ==========
 *
 *  - https://en.cppreference.com/w/cpp/algorithm/find
 *  - https://en.cppreference.com/w/cpp/iterator/advance
 *
 */

#include "linked_list.h"

#include <algorithm>    // for std::find
#include <array>        // for std::array (used in menu)
#include <iostream>     // for std::cout, std::cin
#include <limits>       // for std::numeric_limits
#include <string_view>  // for std::string_view

// For access to string view literals
using std::string_view_literals::operator ""sv;

// Using anonymous namespace to given symbols internal linkage.
namespace {

/// Program menu from lab instructions.
constexpr auto PRELAB_MENU = std::array{
    "Add a person"sv,
    "Find a person"sv,
    "Remove a person"sv,
    "Print the list"sv,
    "Exit"sv,
};

/**
 * Person structure from the instructor provided personList.cpp.
 */
struct Person {
    /// This person's unique identifier.
    int id;
    /// This person's age.
    int age;
    /// This person's name.
    std::string name;
};

/**
 * Runs a continuous interactive program that allows the user to manipulate
 * the provided list.
 *
 * @param list The list to be interacted with.
 */
void run_list_interactive(LinkedList<Person>& list);

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
 * Output stream operator for Person.
 */
std::ostream& operator<<(std::ostream& out, const Person& p);

/**
 * Output stream operator for LinkedList<Person>.
 *
 * We accept list as a non-const reference since we choose not to implement
 * constant iterator for this lab.
 */
std::ostream& operator<<(std::ostream& out, LinkedList<Person>& list);

} // end namespace

int main()
{
    LinkedList<Person> list;

    run_list_interactive(list);
}


// Internal definitions.
namespace {
void run_list_interactive(LinkedList<Person>& list)
{
    // Lambda to generate ID's for each person added to the list.
    auto person_id_generator = []() {
        static int last_id{0};
        return ++last_id;
    };

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
            case 1: { // Add a person.
                auto name = prompt_user<std::string>("Enter the person's name: ");
                auto age = prompt_user<int>("Enter the person's age: ");

                // Move overload not implemented for LinkedList::push_front,
                // to this call results in a deep copy of the temporary.
                list.push_front(Person{
                    person_id_generator(),
                    age,
                    name
                });
                break;
            }
            case 2: { // Find a person.
                auto needle_id = prompt_user<int>("Enter the ID to search for: ");

                auto loc = std::find_if(list.begin(), list.end(), [needle_id](const Person& p) {
                    return p.id == needle_id;
                });

                if (loc != list.end()) {
                    std::cout << "Found " << *loc << '\n';
                } else {
                    std::cout << "No person with ID=" << needle_id << " found.\n";
                }

                break;
            }
            case 3: { // Remove a person.
                auto needle_id = prompt_user<int>("Enter the ID of the person to delete: ");

                auto loc = std::find_if(list.begin(), list.end(), [needle_id](const Person& p) {
                    return p.id == needle_id;
                });

                if (loc != list.end()) {
                    // Unfortunately, we can only remove elements that appear after
                    // an iterator, so we need to manual acquire an iterator to
                    // the element one slot prior.
                    auto removal_iter = list.before_begin();
                    // Advance the iterator to just before the element to remove.
                    std::advance(removal_iter, std::distance(list.begin(), loc));
                    list.remove_after(removal_iter);
                } else {
                    std::cout << "No person with ID=" << needle_id << " found.\n";
                }

                break;
            }
            case 4: { // Print the list.
                std::cout << list << '\n';
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

std::ostream& operator<<(std::ostream& out, LinkedList<Person>& list)
{
    for (auto& elem : list) {
        out << " - " << elem << '\n';
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const Person& p)
{
    out << "Person { id=" << p.id
        << ", name=\"" << p.name
        << "\", age=" << p.age << '}';

    return out;
}
} // end namespace
