/*
 * ECEE 2160 Lab Assignment 2 prelab.
 *
 * The file was provided by the instructor for completing the prelab
 * assignment part I.
 *
 * All comments below indicate modifications to the original file.
 *
 */

#include<iostream>
#include<string>

using namespace std;

// Added anonymous namespace to give symbols internal linkage.
namespace {

struct Person {
    string name;
    int age;
};

void PrintPerson(Person* person)
{
    cout << person->name << " is " << person->age << " years old\n";
}

} // end namespace

int main()
{
    Person person;
    person.name = "John";
    person.age = 10;
    PrintPerson(&person);
}
