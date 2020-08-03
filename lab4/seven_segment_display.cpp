//
// Created by brian on 8/3/20.
//

#include "seven_segment_display.h"

#include <stdexcept>            // for std::out_of_range, std::domain_error
#include <cctype>               // for std::isdigit, std::isxdigit

#include "de1soc_properties.h"

void SevenSegmentDisplay::update_display()
{
    write_register(de1soc::hex3_hex0_base, m_register_values[0].reg);
    write_register(de1soc::hex5_hex4_base, m_register_values[1].reg);
}

void SevenSegmentDisplay::clear_all()
{
    // Set all displays to zero.
    m_register_values = {};
    update_display();
}

void SevenSegmentDisplay::clear_display(std::size_t index)
{
    if (index >= de1soc::seven_segment_display_count) {
        throw std::out_of_range("index must not exceed seven-segment display range");
    }

    access_display_unchecked(index) = 0;
    update_display();
}

void SevenSegmentDisplay::write_display_character(std::size_t index, char character)
{
    if (index >= de1soc::seven_segment_display_count) {
        throw std::out_of_range("index must not exceed seven-segment display range");
    }

    if (!std::isxdigit(character)) {
        throw std::domain_error("character cannot be shown on seven segment display");
    }

    // Translate character to its index in the character value map.
    // Note: we assume ASCII encoding for characters. In general, it is not
    // guaranteed that the characters 'A' through 'F' are consecutive.
    std::size_t value_index = std::isdigit(character)
        ? (character - '0')
        : (character >= 'a') ? (character - 'a') : (character - 'A');

    // Set the target display to the given value.
    access_display_unchecked(index) = k_character_values[value_index];

    update_display();
}

template<int N> struct S;

void SevenSegmentDisplay::show_number(int number)
{
    /// The number of bits of an integer that can be represented per display.
    constexpr static int bits_per_display{4};

    /// The base of the representation used by the display (hex).
    constexpr static int display_base{1u << bits_per_display};

    constexpr static int max_display_value =
        (1u << (bits_per_display * de1soc::seven_segment_display_count)) - 1;
    constexpr static int min_display_value = -static_cast<int>(
        (1u << (bits_per_display * (de1soc::seven_segment_display_count - 1))) - 1
    );

    // Sanity check for min/max display values.
    static_assert(max_display_value ==  0xFF'FF'FF);
    static_assert(min_display_value == -0x0F'FF'FF);

    // Check that the passed value can be shown on the display.
    if (number > max_display_value) {
        throw std::domain_error("number exceeds maximum display value");
    }
    if (number < min_display_value) {
        throw std::domain_error("number less than minimum display value");
    }

    // Set all displays to blank values.
    m_register_values = {};

    int pending = std::abs(number);
    std::size_t current_display{0};

    // Write an unsigned representation of the value to the displays.
    while (pending != 0) {
        auto mod = pending % display_base;
        pending /= display_base;

        // Index of the given hexadecimal character in the display value mapping.
        auto value_index = static_cast<std::size_t>(mod);

        access_display_unchecked(current_display) = k_character_values[value_index];
        ++current_display;
    }

    // If the original number was negative, write a negative sign to the
    // right-most unused display.
    if (number < 0) {
        access_display_unchecked(current_display) = k_negative_sign;
    }

    update_display();
}
