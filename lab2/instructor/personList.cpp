/*
 * ECEE 2160 Lab Assignment 2.
 *
 * The file was provided by the instructor for completing lab assignment 2.
 *
 * Modifications
 * =============
 *
 * All changes made to the original file are marked by comments beginning with
 * "EDIT: ". The only change made was moving all definitions to an anonymous
 * namespace to given the symbols internal linkage.
 */

#include <iostream>
#include <string>

using namespace std;

// EDIT: Added anonymous namespace to give symbols internal linkage.
namespace {

// Linked List Management Code
struct Person {
    // Unique identifier for the person
    int id;
    // Information about person
    string name;
    int age;
    // Pointer to next person in list
    Person* next;
};

struct List {
    // First person in the list. A value equal to nullptr indicates that the
    // list is empty.
    Person* head;
    // Current person in the list. A value equal to nullptr indicates a
    // past-the-end position.
    Person* current;
    // Pointer to the element appearing before 'current'. It can be nullptr if
    // 'current' is nullptr, or if 'current' is the first element in the list.
    Person* previous;
    // Number of persons in the list
    int count;
};

// Give an initial value to all the fields in the list.
void ListInitialize(List* list)
{
    list->head = nullptr;
    list->current = nullptr;
    list->previous = nullptr;
    list->count = 0;
}

// Move the current position in the list one element forward. If last element
// is exceeded, the current position is set to a special past-the-end value.
void ListNext(List* list)
{
    if (list->current) {
        list->previous = list->current;
        list->current = list->current->next;
    }
}

// Move the current position to the first element in the list.
void ListHead(List* list)
{
    list->previous = nullptr;
    list->current = list->head;
}

// Get the element at the current position, or nullptr if the current position is
// past-the-end.
Person* ListGet(List* list)
{
    return list->current;
}

// Set the current position to the person with the given id. If no person
// exists with that id, the current position is set to past-the-end.
void ListFind(List* list, int id)
{
    ListHead(list);
    while (list->current && list->current->id != id) {
        ListNext(list);
    }
}

// Insert a person before the element at the current position in the list. If
// the current position is past-the-end, the person is inserted at the end of
// the list. The new person is made the new current element in the list.
void ListInsert(List* list, Person* person)
{
    // Set 'next' pointer of current element
    person->next = list->current;
    // Set 'next' pointer of previous element. Treat the special case where
    // the current element was the head of the list.
    if (list->current == list->head) {
        list->head = person;
    } else {
        list->previous->next = person;
    }
    // Set the current element to the new person
    list->current = person;
}

// Remove the current element in the list. The new current element will be the
// element that appeared right after the removed element.
void ListRemove(List* list)
{
    // Ignore if current element is past-the-end
    if (!list->current) {
        return;
    }
    // Remove element. Consider special case where the current element is
    // in the head of the list.
    if (list->current == list->head) {
        list->head = list->current->next;
    } else {
        list->previous->next = list->current->next;
    }
    // Free element, but save pointer to next element first.
    Person* next = list->current->next;
    delete list->current;
    // Set new current element
    list->current = next;
}

void PrintPerson(Person* person)
{
    cout << "Person with ID: " << person->id << endl;
    cout << "\tName: " << person->name << endl;
    cout << "\tAge: " << person->age << endl << endl;;
}

} // EDIT: end namespace


int main()
{
    // EDIT: added initialization of 'list'
    List list{};                // Create the main list
    ListInitialize(&list);      // Initialize the list

    ListInsert(&list, new Person {
        1, "fred", 1, nullptr
    });
    ListInsert(&list, new Person {
        2, "jim", 2, nullptr
    });


} //end main
