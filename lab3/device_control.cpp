#include "device_control.h"

/// Physical base address of FPGA Devices.
constexpr inline std::size_t LW_BRIDGE_BASE{0xFF'20'00'00};

/// Length of memory-mapped IO window
constexpr inline std::size_t LW_BRIDGE_SPAN{0x00'00'50'00};

/// Base offset to LED registers.
constexpr inline std::size_t LEDR_BASE{0x00'00'00'00};

/// Base offset to switch registers.
constexpr inline std::size_t SW_BASE{0x00'00'00'40};

/// Base offset to push button registers.
constexpr inline std::size_t KEY_BASE{0x00'00'00'50};

void DeviceControl::Write1Led(std::size_t led_index, bool state)
{
    if (led_index >= DeviceControl::LED_COUNT) {
        throw std::out_of_range("invalid LED index.");
    }

    // Bit mask identifying the controlling bit for the specified LED.
    const DE1SoCfpga::Register led_mask{1u << led_index};
    // The current state of the LEDs.
    DE1SoCfpga::Register led_state = m_fpga.RegisterRead(LEDR_BASE);

    if (state) { // Set an LED to ON.
        led_state |= led_mask;
    } else { // Set an LED to OFF.
        led_state &= ~led_mask;
    }

    m_fpga.RegisterWrite(LEDR_BASE, led_state);
}

bool DeviceControl::Read1Switch(std::size_t switch_index)
{
    if (switch_index >= DeviceControl::SWITCH_COUNT) {
        throw std::out_of_range("invalid switch index.");
    }

    // The current state of the switches.
    DE1SoCfpga::Register switch_state = m_fpga.RegisterRead(SW_BASE);

    // Shift the switch state so that the target switch's bit is the LSB.
    switch_state >>= switch_index;

    // Return the state of the LSB.
    return switch_state & 1u;
}

void DeviceControl::WriteAllLeds(DE1SoCfpga::Register leds_state)
{
    m_fpga.RegisterWrite(LEDR_BASE, leds_state);
}

// Implemented for assignment 3.
DE1SoCfpga::Register DeviceControl::ReadAllSwitches()
{
    return m_fpga.RegisterRead(SW_BASE);
}

// Implemented for assignment 4.
DeviceControl::PushButton DeviceControl::PushButtonGet()
{
    // The current state of the buttons.
    const auto register_state = m_fpga.RegisterRead(KEY_BASE);

    auto button_state = PushButton::None;

    for (std::size_t i{0}; i < PUSH_BUTTON_COUNT; ++i) {
        // Check if the ith button is pressed.
        if (register_state & (1u << i)) {
            // Check if we have already found a pressed button.
            if (button_state != PushButton::None) {
                return PushButton::Multiple;
            }
            button_state = static_cast<PushButton>(i);
        }
    }
    return button_state;
}

