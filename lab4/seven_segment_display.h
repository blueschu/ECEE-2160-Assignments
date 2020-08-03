//
// Created by brian on 8/3/20.
//

#ifndef ECEE_2160_LAB_REPORTS_SEVEN_SEGMENT_DISPLAY_H
#define ECEE_2160_LAB_REPORTS_SEVEN_SEGMENT_DISPLAY_H

#include <array>

#include "de1soc_device.h"

/**
 * The seven segment display on the DE1-SoC board.
 *
 * This class inherits from DE1SoCHardwareDevice to gain access to
 * memory-mapping I/O utilities per lab instructions.
 *
 * Note: if this class used DE1SoCHardwareDevice via composition instead of
 * inheritance, we could test this class without an active hardware connection
 * by creating a mock hardware handle. Don't design hierarchies like this in
 * real-world code.
 */
class SevenSegmentDisplay : private DE1SoCHardwareDevice {
    /**
     * The union DisplayRegister requires the DE1-SoC register size to be
     * precisely 32 bits. This check is added for the purposes of documentation
     */
    static_assert(sizeof(DE1SoCHardwareDevice::Register) == 4);

    /// Integral type used to represent the encoded value of a single display.
    using DisplayValue = std::uint8_t;

    /**
     * View into a seven segment display register as either a full DE1-SoC
     * register or as individual display displays.
     *
     * Words on DE1-SoC are little-endian, so displays[0] represents the
     * first display segment in the register, and displays[3] represent the
     * last.
     *
     * Note: this construct is not portable. We use explicit knowledge about
     * the layout of the DE1-SoC's register to make this union valid. This
     * code may not work as intended if ported to a different device.
     */
    union DisplayRegister {
        DisplayValue displays[4];
        DE1SoCHardwareDevice::Register reg;
    };
    // Sanity check for display register size.
    static_assert(sizeof(DisplayRegister) == 4);

    /**
     * Mapping between [0,15] and the corresponding hexadecimal characters
     * encoded for the seven-segment displays.
     */
    constexpr static inline std::array<DisplayValue, 16> k_character_values{
        0b0011'1111, // 0
        0b0000'0110, // 1
        0b0101'1011, // 2
        0b0100'1111, // 3
        0b0110'0110, // 4
        0b0110'1101, // 5
        0b0111'1101, // 6
        0b0000'0111, // 7
        0b0111'1111, // 8
        0b0110'1111, // 9
        0b0111'0111, // A
        0b0111'1100, // B
        0b0011'1001, // C
        0b0101'1110, // D
        0b0111'1001, // E
        0b0111'0001, // F
    };

    /**
     * Seven-segment display encoded negative sign.
     */
    constexpr static inline DisplayValue k_negative_sign{0b0100'0000};

    /**
     * Cache of the current values of the seven segment display registers.
     *
     * This array corresponds to the member variables `reg0_hexValue` and
     * `reg1_hexValue` from the lab instructions.
     */
    std::array<DisplayRegister, 2> m_register_values{};

    /**
     * Whether this display is the current logically owner of the seven-segment
     * display.
     */
    bool m_owner{true};

  public:
    SevenSegmentDisplay() = default;

    ~SevenSegmentDisplay()
    {
        // Clear the seven-segment displays.
        if (m_owner) {
//            clear_all();
        }
    }

    SevenSegmentDisplay(const SevenSegmentDisplay&) = delete;

    SevenSegmentDisplay(SevenSegmentDisplay&& other) noexcept
        : m_register_values{std::exchange(other.m_register_values, {})},
          m_owner{std::exchange(other.m_owner, false)} {}

    SevenSegmentDisplay& operator=(const SevenSegmentDisplay&) = delete;

    SevenSegmentDisplay& operator=(SevenSegmentDisplay&& other) noexcept
    {
        m_register_values = std::exchange(other.m_register_values, {});
        m_owner = std::exchange(other.m_owner, false);

        return *this;
    }

    /**
     * Turns off all displays.
     */
    void clear_all();

    /**
     * Turns off the specified display.
     *
     * @param index Display index
     * @throws out_of_range if no display exists at the specified index.
     */
    void clear_display(std::size_t index);

    /**
     * Write the given character to the specified seven-segment display.
     *
     * @param index Display index.
     * @param value New display value.
     * @throws out_of_range if no display exists at the specified index.
     * @throws domain_error if the specified character cannot be displayed on
     *          a seven-segment display.
     */
    void write_display_character(std::size_t index, char character);

    /**
     * Renders the provided signed number
     *
     * @param number The value to be shown by the displays.
     */
    void show_number(int number);

  private:
    /**
     * Writes the current display states to the displays registers.
     */
    void update_display();

    /**
     * Returns a reference to the display at the given index.
     * @param index Display index.
     * @return Reference to display value.
     */
    DisplayValue& access_display_unchecked(std::size_t index)
    {
        const auto reg_index = index / sizeof(Register);
        const auto display_index = index % sizeof(Register);

        return m_register_values[reg_index].displays[display_index];
    }

};

#endif //ECEE_2160_LAB_REPORTS_SEVEN_SEGMENT_DISPLAY_H
