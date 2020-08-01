/**
 * ECEE 2160 Lab Assignment 3 prelab test program.
 *
 * Author:  Brian Schubert
 * Date:    2020-07-23
 *
 */

// Misc notes.
// 8.2.9/13 (void* -> T*) and 8.2.10/7 (T* -> S*) for pointer conversions
// Manual: ftp://ftp.intel.com/pub/fpgaup/pub/Intel_Material/17.0/Tutorials/Linux_On_DE_Series_Boards.pdf


#include "prelab_fpga.h"

#include <iostream>

// Define to test the LED mapped i/o utilities.
#define TEST_LEDS

// Define to test the switch mapped i/o utilities.
#define TEST_SWITCHES

int main()
{
    auto [virtual_base, fd] = Initialize();

#ifdef TEST_LEDS
    std::cout << "=== Testing LEDs ===\n";
    std::size_t led_index;
    bool led_state;
    while (std::cin >> led_index >> led_state) {
        std::cout << "Enter (led index) (state): ";
        try {
            std::cout << "Setting LED #" << led_index << " to "
                      << (led_state ? "ON" : "OFF") << '\n';
            Write1Led(virtual_base, led_index, led_state);
        } catch (const std::out_of_range&) {
            std::cout << "Bad LED; exiting...\n";
            break;
        }
    }
#endif

#ifdef TEST_SWITCHES
    std::cout << "=== Testing switches ===\n";
    std::size_t switch_index;
    while (std::cin >> switch_index) {
        std::cout << "Enter (switch index): ";
        try {
            const bool switch_state = Read1Switch(virtual_base, switch_index);
            std::cout << "Switch " << switch_index << " = " << switch_state << '\n';
        } catch (const std::out_of_range&) {
            std::cout << "Bad switch; exiting...\n";
            break;
        }

    }

#endif

    Finalize(virtual_base, fd);
}

