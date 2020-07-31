/**
 * ECEE 2160 Lab Assignment 3 DE1-SoC fpga device interface class.
 *
 * Completed for lab assignment 3.5.
 *
 * All symbol names were specified by the lab instructions.
 *
 * Author:  Brian Schubert
 * Date:    2020-07-26
 *
 * References
 * ===========
 *
 *  [1] https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
 */

#ifndef ECEE_2160_LAB_REPORTS_DEVICE_CONTROL_H
#define ECEE_2160_LAB_REPORTS_DEVICE_CONTROL_H

#include "de1socfpga.h"

/**
 * The "LEDControl" class from the assignment instructions.
 */
class DeviceControl {

    DE1SoCfpga m_fpga;

  public:

    /// The number of LEDs on the DE1-SoC board.
    constexpr static inline std::size_t LED_COUNT{10};

    /// The number of switches on the DE1-SOC board.
    constexpr static inline std::size_t SWITCH_COUNT{10};

    /// The number of push buttons on the DE1-SOC board.
    constexpr static inline std::size_t PUSH_BUTTON_COUNT{4};

    /**
     * The possible state representations of the DE1SoC push buttons.
     *
     * The representation values match the specification in the instructions
     * for assignment 4.
     */
    enum class PushButton : std::int8_t {
        None = -1,
        Button0,    // 0
        Button1,    // 1
        Button2,    // 2
        Button3,    // 3
        Multiple,   // 4
    };

    // Constructor
    explicit DeviceControl(DE1SoCfpga fgpa) : m_fpga{std::move(fgpa)} {}

    // No destructor defined per "Rule of 0" [C.20 from 1].
    // ~DeviceControl()

    /**
     * Sets the LED at the given index to the specified state.
     *
     *
     * @param led_index Index of the LED.
     * @param state Desired state of the LED (true for on, false for off).
     */
    void Write1Led(std::size_t led_index, bool state);

    /**
     * Reads the state of the switch at the given index to the specified state.
     *
     * This function was implemented as part of the prelab assignment.
     *
     * @param led_index Index of the LED.
     * @param state Desired state of the LED (true for on, false for off).
     */
    bool Read1Switch(std::size_t switch_index);

    /**
     * Sets the state of the LEDs with the given value.
     *
     * @param leds_state State for all of the LEDs.
     */
    void WriteAllLeds( DE1SoCfpga::Register leds_state);

    /**
     * Reads the state the switches and returns their value as a single integer
     *
     * @return Switch state.
     */
    DE1SoCfpga::Register ReadAllSwitches();

    /**
     * Reads the state of the push buttons.
     *
     * @return Push button state.
     */
    PushButton PushButtonGet();

};

#endif //ECEE_2160_LAB_REPORTS_DEVICE_CONTROL_H
