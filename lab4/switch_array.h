//
// Created by brian on 8/3/20.
//

#ifndef ECEE_2160_LAB_REPORTS_SWITCH_ARRAY_H
#define ECEE_2160_LAB_REPORTS_SWITCH_ARRAY_H

#include <cstddef>

/**
 * An array of switches on the DE1-SoC board.
 *
 * This class inherits from DE1SoCHardwareDevice to gain access to
 * memory-mapping I/O utilities per lab instructions.
 *
 * This class is designed to be de-coupled with the details of the DE1-SoC
 * board itself. It is capable of interfacing with switch control registers
 * on any system with up to 32 switches
 *
 * @tparam The number of leds, not exceeding 32.
 */
template<std::size_t N>
class SwitchArray : private DE1SoCHardwareDevice {

    static_assert(
        N <= CHAR_BIT * sizeof(Register),
        "SwitchArray can only operate on one switch control register (32 switches)."
    );

    /// Offset to switch control register on the DE1-SoC board.
    std::size_t m_base_offset;

  public:
    /// The number of switches.
    constexpr static inline std::size_t k_switch_count{N};

    /**
     * Helper structure representing a snapshot of the switches state.
     */
    struct State {
        Register bits{0};
        std::size_t count{0};

        State() = default;

        State(Register b) : bits{b}
        {
            for (std::size_t i{0}; i < N; ++i) {
                if (bits & (1u << i)) {
                    ++count;
                }
            }
        }

        /// Returns `true1 if more than one switch is on.
        bool multiple() const { return count > 1; }
    };

    /**
     * Constructs an SwitchArray with a control register at the given physical
     * offset.
     */
    explicit SwitchArray(std::size_t base_offset) : m_base_offset{base_offset} {};

    /**
     * Reads the the state of the switch at the given index..
     *
     * Marked nodiscard per clang-tiddy recommendation.
     *
     * @param led_index Index of the switch.
     * @throws out_of_range if no switch exists at the given index.
     * @returns Switch state.
     */
    [[nodiscard]]
    bool read(std::size_t index) const;

    /**
     * Reads the state of all of the switches.
     *
     * Marked nodiscard per clang-tiddy recommendation.
     *
     * @returns Sequence of bits representing the switch states
     */
    [[nodiscard]]
    State read_all() const;

};

#include "switch_array.tpp"

#endif //ECEE_2160_LAB_REPORTS_SWITCH_ARRAY_H
