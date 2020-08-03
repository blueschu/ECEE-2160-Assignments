//
// Created by brian on 8/3/20.
//

#ifndef ECEE_2160_LAB_REPORTS_LED_ARRAY_H
#define ECEE_2160_LAB_REPORTS_LED_ARRAY_H

#include <climits>      // for CHAR_BIT
#include <cstddef>      // for std::size_t

#include "de1soc_device.h"

/**
 * An array of LEDs on the DE1-SoC board.
 *
 * This class inherits from DE1SoCHardwareDevice to gain access to
 * memory-mapping I/O utilities per lab instructions.
 *
 * This class it capable to with an LED control register representing
 * any number of LEDs up to 32.
 *
 * @tparam The number of leds, not exceeding 32.
 */
template<std::size_t N>
class LedArray : private DE1SoCHardwareDevice {

    static_assert(
        N < CHAR_BIT * sizeof(Register),
        "LedArray can only operate on one LED control register."
    );

    /// Cache of the current state of the DE1-SoC LEDs.
    Register m_led_state{};

    /// Offset to LED control register on the DE1-SoC board.
    std::size_t m_base_offset;

  public:
    /**
     * Constructs an LedArray with all LEDs in their off set.
     */
    explicit LedArray(std::size_t base_offset) : m_base_offset{base_offset} {};

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
