/**
 * ECEE 2160 Lab Assignment 4 - Generic switch I/O.
 *
 * Author:  Brian Schubert
 * Date:    2020-08-03
 *
 */

#ifndef ECEE_2160_LAB_REPORTS_SWITCH_ARRAY_H
#define ECEE_2160_LAB_REPORTS_SWITCH_ARRAY_H

#include <cstddef>      // for std::size_t
#include <memory>       // for std::shared_ptr

#include "register_io.h"

/**
 * An array of switches (or push buttons) on a generic board.
 *
 * This class is designed to be de-coupled with the details of the DE1-SoC
 * board itself. It is capable of interfacing with a switch control register
 * representing any number of switches up to to word size of the target board.
 *
 * @tparam The number of switches, not exceeding the number of bits
 *          in a hardware register..
 * @tparam Reg The integral type representing a hardware register.
 */
template<std::size_t N, typename Reg>
class SwitchArray {

    /**
     * Alias for the underlying type of seven-segment display control registers.
     */
    using Register = typename RegisterIO<Reg>::Register;

    static_assert(
        N <= CHAR_BIT * sizeof(Register),
        "SwitchArray control register must fit in one hardware register."
    );

    /// Shared accessor to the board's physical memory.
    std::shared_ptr<RegisterIO<Reg>> m_register_io;

    /// Offset to switch control register on the DE1-SoC board.
    std::size_t m_base_offset;

  public:
    /// The number of switches.
    constexpr static inline std::size_t k_switch_count{N};

    /**
     * Helper structure representing a snapshot of the switches state.
     */
    struct State {
        /// Sequence of bits representing the states of the switches.
        Register bits{0};
        /// The number of switches that are "on".
        std::size_t count{0};

        State() = default;

        explicit State(Register b) : bits{b}
        {
            // Count the number of `1` bits.
            for (std::size_t i{0}; i < k_switch_count; ++i) {
                if (bits & (1u << i)) {
                    ++count;
                }
            }
        }

        /// Returns `true` if more than one switch is "on."
        bool multiple() const { return count > 1; }

        friend bool operator==(const State& lhs, const State& rhs) {
            return lhs.bits == rhs.bits;
        }
    }; // struct State

    /**
     * Constructs an SwitchArray using the specified register I/O accessor and
     * with a control register at the given physical offset.
     *
     * @param register_io Accessor to memory-mapped physical addresses.
     * @param base_offset Offset from the physical mapping base to the switch
     *                    control register.
     */
    SwitchArray(std::shared_ptr<RegisterIO<Reg>> register_io, std::size_t base_offset)
        : m_register_io{std::move(register_io)}, m_base_offset{base_offset} {};

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
