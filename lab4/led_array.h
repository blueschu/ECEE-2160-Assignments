/**
 * ECEE 2160 Lab Assignment 4 - Generic memory-mapped LED I/O.
 *
 * Author:  Brian Schubert
 * Date:    2020-08-03
 */

#ifndef ECEE_2160_LAB_REPORTS_LED_ARRAY_H
#define ECEE_2160_LAB_REPORTS_LED_ARRAY_H

#include <climits>      // for CHAR_BIT
#include <cstddef>      // for std::size_t
#include <memory>

#include "register_io.h"

/**
 * An array of LEDs on a generic board.
 *
 * This class is designed to be de-coupled with the details of the DE1-SoC
 * board itself. It is capable of interfacing with an LED control register
 * representing any number of LEDs up to to word size of the target board.
 *
 * @tparam The number of leds, not exceeding target board word size.
 */
template<std::size_t N, typename Reg>
class LedArray {
    /**
     * Alias for the underlying type of the LED control register.
     */
    using Register = typename RegisterIO<Reg>::Register;

    static_assert(
        N <= CHAR_BIT * sizeof(Register),
        "LedArray control register must fit in one hardware register."
    );

    /// Cache of the current state of the board's LEDs.
    Register m_led_state{};

    /// Shared accessor to the board's physical memory.
    std::shared_ptr<RegisterIO<Reg>> m_register_io;

    /// Offset to LED control register on the board.
    std::size_t m_base_offset;

  public:
    /**
     * Constructs an LedArray with all LEDs in their off set using the
     * specified register I/O accessor and offset to the LED control register.
     *
     * @param register_io Accessor to memory-mapped physical addresses.
     * @param base_offset Offset from the physical mapping base to the LED
     *                    control register.
     */
    LedArray(std::shared_ptr<RegisterIO<Reg>> register_io, std::size_t base_offset)
        : m_register_io{std::move(register_io)}, m_base_offset{base_offset} {};

    /**
     * Sets the LED at the given index to the specified state.
     *
     * @param led_index Index of the LED.
     * @param state Desired state of the LED (true for on, false for off).
     * @throws out_of_range if no LED exists at the given index.
     */
    void write(std::size_t index, bool state);

    /**
     * Sets the state of the LEDs with the given value.
     *
     * @param leds_bits Sequence of bits representing the LED states..
     */
    void write_all(Register led_bits);

  private:
    /**
     * Writes the current LED states to LED control register.
     */
    void update_leds();

};

#include "led_array.tpp"

#endif //ECEE_2160_LAB_REPORTS_LED_ARRAY_H
