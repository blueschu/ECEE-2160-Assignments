/**
 * ECEE 2160 Lab Assignment 4 - Generic seven-segment display I/O.
 *
 * Author:  Brian Schubert
 * Date:    2020-08-03
 *
 * References
 * ==========
 *
 * [wiki-endianness]    https://en.wikipedia.org/wiki/Endianness
 * [isocpp-guidelines]  https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
 */


#ifndef ECEE_2160_LAB_REPORTS_SEVEN_SEGMENT_DISPLAY_H
#define ECEE_2160_LAB_REPORTS_SEVEN_SEGMENT_DISPLAY_H

#include <array>                // for std::array

#include "register_io.h"        // for RegisterIO

/// Namespace for universal seven-segment display properties.
namespace display_config {
/// Integral type used to represent the encoded value of a single display.
using DisplayValue = std::uint8_t;

/**
 * Mapping between [0,15] and the corresponding hexadecimal characters
 * encoded for the seven-segment displays.
 */
constexpr inline std::array<DisplayValue, 16> character_values{
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
constexpr inline DisplayValue negative_sign{0b01000000};
} // namespace display_config

/**
 * The seven segment display on the a generic board.
 *
 * This class is designed to be de-coupled with the details of the DE1-SoC
 * board itself. It should be usable on any system with little-endian
 * control registers without any changes.
 *
 * Unlike other hardware interface implemented for this lab, this class
 * can should be able to freely operate on boards with word sizes of any
 * multiple of CHAR_BIT.
 *
 */
template<std::size_t N, typename Reg>
class SevenSegmentDisplay {
    /**
     * Alias for the underlying type of seven-segment display control registers.
     */
    using Register = typename RegisterIO<Reg>::Register;

    /**
     * The number of control registers storing the state of the seven-segment
     * displays.
     *
     * This constant equals ceil(N / 4) where N is the number of displays.
     */
    constexpr static inline std::size_t k_register_count{
        N / sizeof(Register) + (N % sizeof(Register) == 0 ? 0 : 1)
    };

    /// Type representing a static array with a length equal to the number
    /// of seven-segment display control registers.
    template<typename V>
    using RegisterMappingArray = std::array<V, k_register_count>;

    /**
     * View into a seven segment display register as either a full DE1-SoC
     * register or as individual display displays.
     *
     * Words are expected to be little-endian, so displays[0] represents the
     * first display segment in the register, and displays[n] represents the
     * last.
     *
     * Note: This union should be portable, but we do not have sufficient C++
     * expertise to guarantee this. If this code is ported to a different,
     * some special attention should be given to this type.
     */
    union DisplayRegister {
        display_config::DisplayValue displays[sizeof(Register)];
        Register reg;
    };
    /// Ensure the DisplayRegisters have the same layout as the control registers.
    static_assert(sizeof(DisplayRegister) == sizeof(Register));

    /**
     * Cache of the current values of the seven segment display registers.
     *
     * This array corresponds to the member variables `reg0_hexValue` and
     * `reg1_hexValue` from the lab instructions.
     */
    RegisterMappingArray<DisplayRegister> m_register_values{};

    /// Shared accessor to the board's physical memory.
    std::shared_ptr<RegisterIO<Reg>> m_register_io;

    /**
     * The offsets to the control registers for the seven segment dispalys.
     */
    RegisterMappingArray<std::size_t> m_register_offsets;

    /**
     * Whether this display is the current logically owner of the seven-segment
     * display.
     *
     * We need to track this property to prevent "moved-from"
     * SevenSegmentDisplay instances from clearing the displays.
     */
    bool m_owner{true};

  public:
    /**
     * Constructs a SevenSegmentDisplay with the specified register I/O accessor
     * and with the given sequence of display control registers.
     *
     *
     * @param register_io       Accessor to memory-mapped physical addresses.
     * @param register_offsets  Offsets from the physical mapping base to
     *                          the seven-segment display control registers.
     */
    SevenSegmentDisplay(
        std::shared_ptr<RegisterIO<Reg>> register_io,
        RegisterMappingArray<std::size_t> register_offsets
    )
        : m_register_io{std::move(register_io)},
          m_register_offsets{std::move(register_offsets)} {}

    // Destructor. Marked noexcept per C.37 [isocpp-guidelines].
    ~SevenSegmentDisplay() noexcept
    {
        // Clear the seven-segment displays per lab instructions.
        if (m_owner) {
            clear_all();
        }
    }

    // Prevent copying from lvalue, C.21, C.81. [isocpp-guidelines].
    SevenSegmentDisplay(const SevenSegmentDisplay&) = delete;

    // Move constructor, C.21 [isocpp-guidelines].
    SevenSegmentDisplay(SevenSegmentDisplay&& other) noexcept
        : m_register_values{std::move(other.m_register_values)},
          m_register_io{std::move(other.m_register_io)},
          m_register_offsets{std::move(other.m_register_offsets)},
          m_owner{std::exchange(other.m_owner, false)} {}

    // Prevent copying from lvalue, C.21, C.81. [isocpp-guidelines].
    SevenSegmentDisplay& operator=(const SevenSegmentDisplay&) = delete;

    // Move assignment, C.21 [isocpp-guidelines].
    SevenSegmentDisplay& operator=(SevenSegmentDisplay&& other) noexcept
    {
        m_register_values = std::move(other.m_register_values);
        m_register_io = std::move(other.m_register_io);
        m_register_offsets = std::move(other.m_register_offsets);
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
     * This function is defined for the lab, but is not used in the final
     * demo program.
     *
     * @param index Display index
     * @throws out_of_range if no display exists at the specified index.
     */
    [[maybe_unused]]
    void clear_display(std::size_t index);

    /**
     * Write the given character to the specified seven-segment display.
     *
     * This function is defined for the lab, but is not used in the final
     * demo program.
     *
     * @param index Display index.
     * @param value New display value.
     * @throws out_of_range if no display exists at the specified index.
     * @throws domain_error if the specified character cannot be displayed on
     *          a seven-segment display.
     */
    [[maybe_unused]]
    void write_display_character(std::size_t index, char character);

    /**
     * Renders the provided signed number
     *
     * @param number The value to be shown by the displays.
     */
    void print_hex(int number);

    /**
     * Returns a pair containing the minimum and maximum value that can be
     * displayed on the seven-segment display.
     *
     * @return [min, max] interval for seven-segment display.
     */
    constexpr static std::pair<int, int> display_range()
    {
        /// The number of bits of an integer that can be represented per display.
        constexpr std::size_t bits_per_display{4};

        constexpr int max_display_value =
            (1u << (bits_per_display * N)) - 1;
        constexpr int min_display_value = -static_cast<int>(
            (1u << (bits_per_display * (N - 1))) - 1
        );

        return {min_display_value, max_display_value};
    }

  private:

    /**
     * Writes the current display states to the displays registers.
     */
    void update_display();

    /**
     * Returns a reference to the display at the given index.
     *
     * Marked as nodiscard per clang-tiddy recommendation.
     *
     * @param index Display index.
     * @return Reference to display value.
     */
    [[nodiscard]]
    display_config::DisplayValue& access_display_unchecked(std::size_t index)
    {
        const auto reg_index = index / sizeof(Register);
        const auto display_index = index % sizeof(Register);

        return m_register_values[reg_index].displays[display_index];
    }

};

// Sanity check for min/max display values. Register type is arbitrary.
static_assert(SevenSegmentDisplay<6, int>::display_range().first == -0x0F'FF'FF);
static_assert(SevenSegmentDisplay<6, int>::display_range().second == 0xFF'FF'FF);

#include "seven_segment_display.tpp"

#endif //ECEE_2160_LAB_REPORTS_SEVEN_SEGMENT_DISPLAY_H
