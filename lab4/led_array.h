//
// Created by brian on 8/3/20.
//

#ifndef ECEE_2160_LAB_REPORTS_LED_ARRAY_H
#define ECEE_2160_LAB_REPORTS_LED_ARRAY_H

#include <cstddef>      // for std::size_t
#include <bitset>       // for std::bitset

#include "de1soc_device.h"

/**
 * An array of LEDs on the DE1-SoC board.
 */
template<std::size_t N>
class LedArray : private DE1SoCHardwareDevice {

    /// Cache of the current state of the DE1-SoC LEDs.
    std::bitset<N> m_led_state{};

  public:
    /**
     * Constructs an LedArray with all LEDs in their off set.
     */
    LedArray() = default;

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
    void write_all(unsigned long led_bits);

  private:
    /**
     * Writes the current LED states to LED control register.
     */
    void update_leds();

};

#endif //ECEE_2160_LAB_REPORTS_LED_ARRAY_H
