/**
 * ECEE 2160 Lab Assignment 3.5 - Controlling the push buttons ""OO Style".
 *
 * Author:  Brian Schubert
 * Date:    2020-07-26
 *
 */

#include <chrono>           // for std::chrono::duration
#include <thread>           // for std::this_thread

#include "device_control.h"
#include "wrapping_counter.h"


// Using anonymous namespace to given symbols internal linkage.
namespace {

// For access to duration literals.
using namespace std::literals::chrono_literals;

/**
 * The period of time elapsed between button reads.
 */
constexpr std::chrono::duration REFRESH_PERIOD = 1ms;

/**
 * The state of the board's switches that signals the program to exit.
 */
constexpr DE1SoCfpga::Register SWITCH_EXIT_SENTINEL = 0;

void run_button_demo(DeviceControl& device_control);

} // end namespace

int main()
{
    DeviceControl device_control{{}};

    run_button_demo(device_control);
}

// Internal definitions.
namespace {

void run_button_demo(DeviceControl& device_control)
{
    // Counter holding the state to be written to the board's LEDs.
    WrappingCounter counter{(1u << DeviceControl::LED_COUNT) - 1};

    // The state of the DE1SoC's button's during the previous cycle.
    auto previous_button{DeviceControl::PushButton::None};

    // Whether to continue the button demo.
    bool run_demo{true};

    while (run_demo) {
        // Current state of the board's buttons.
        const auto button_press = device_control.PushButtonGet();

        // If true, the user recently pressed multiple buttons. We want to
        // wait until all buttons have been released before taking any new actions.
        const bool wait_for_unpress = previous_button == DeviceControl::PushButton::Multiple
            && button_press != DeviceControl::PushButton::None;
        // If true, the button state has not changed, so no action should be taken.
        const bool no_button_change = button_press == previous_button;

        if (wait_for_unpress || no_button_change) {
            std::this_thread::sleep_for(REFRESH_PERIOD);
            continue;
        }

        switch (button_press) {
            case DeviceControl::PushButton::None: { // No action
                break;
            }
            case DeviceControl::PushButton::Button0: { // Increment the LEDs
                ++counter;
                break;
            }
            case DeviceControl::PushButton::Button1: { // Decrement the LEDs
                --counter;
                break;
            }
            case DeviceControl::PushButton::Button2: { // Shift the LEDs to the right.
                counter.apply([](auto count) { return count >> 1u; });
                break;
            }
            case DeviceControl::PushButton::Button3: { // Shift the LEDs to the left.
                counter.apply([](auto count) { return count << 1u; });
                break;
            }
            case DeviceControl::PushButton::Multiple: {
                const auto switch_state = device_control.ReadAllSwitches();

                // Set the LEDs to match the state of the switches.
                counter.apply([=](auto) { return switch_state; });

                // Check if the current state of the switches matches the exit state.
                if (switch_state == SWITCH_EXIT_SENTINEL) {
                    run_demo = false;
                }
            }
        }

        // Update the board's LEDs.
        device_control.WriteAllLeds(static_cast<DE1SoCfpga::Register>(counter));
        previous_button = button_press;

        std::this_thread::sleep_for(REFRESH_PERIOD);
    }

}

} // end namespace


