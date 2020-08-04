/**
 * ECEE 2160 Lab Assignment 4
 *
 * Author:  Brian Schubert
 * Date:    2020-08-01
 *
 * References
 * ==========
 *
 *
 * [P0627r0] http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0627r0.pdf
 *
 */

#include <chrono>                   // for std::chrono::duration
#include <thread>                   // for std::this_thread

#include "de1soc_properties.h"      // for de1soc_config
#include "de1soc_register_io.h"     // for DE1SoCRegisterIO
#include "wrapping_counter.h"       // for WrappingCounter


#ifdef LAB4_MOCK
#include "mock_register_io.h"
#endif


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
constexpr int SWITCH_EXIT_SENTINEL = 0;

/**
 * Convenience structure to hold all DE1-SoC hardware device interfaces.
 */
struct DE1SoC {
    de1soc_config::Display display;
    de1soc_config::LEDs leds;
    de1soc_config::Switches switches;
    de1soc_config::Keys keys;
};

void run_bottom_demo(DE1SoC& board);

} // end namespace

int main()
{
    using namespace de1soc_config;

#ifdef LAB4_MOCK
    // Create a MockRegisterIO for testing without hardware.
    auto register_io = std::make_shared<MockRegisterIO>();
#else
    // Create a DE1SoCRegisterIO which will be shared by all hardware device
    // interface classes.
    auto register_io = std::make_shared<DE1SoCRegisterIO>(
        DE1SoCRegisterIO::new_connection()
    );
#endif

    // Initialize all DE1-Soc hardware interfaces.
    DE1SoC board{
        {register_io, {hex3_hex0_base, hex5_hex4_base}},
        {register_io, ledr_base},
        {register_io, switch_base},
        {register_io, key_base},
    };

    // Run the demo.
    run_bottom_demo(board);
}

// Internal definitions
namespace {

void run_bottom_demo(DE1SoC& board)
{
    // Counter holding the state to be written to the board's LEDs.
    WrappingCounter counter{(1u << de1soc_config::led_count) - 1};

    // The state of the DE1SoC's button's during the previous cycle.
    de1soc_config::Keys::State previous_button{};

    while (true) {
        // Current state of the board's buttons.
        const auto button_press = board.keys.read_all();

        // If true, the user recently pressed multiple buttons. We want to
        // wait until all buttons have been released before taking any new actions.
        const bool wait_for_unpress = previous_button.multiple() && button_press.count != 0;

        // If true, the button state has not changed, so no action should be taken.
        const bool no_button_change = button_press == previous_button;

        if (wait_for_unpress || no_button_change) {
            std::this_thread::sleep_for(REFRESH_PERIOD);
            continue;
        }

        // Check if multiple buttons are currently depressed.
        if (button_press.multiple()) {
            const auto switch_state = board.switches.read_all().bits;

            // Set the LEDs to match the state of the switches.
            counter.apply([=](auto) { return switch_state; });

            // Check if the current state of the switches matches the exit state.
            if (switch_state == SWITCH_EXIT_SENTINEL) {
                // Stop the demo program.
                break;
            }
        } else {
            // Only one button is currently pressed. Take an action based on
            // its value.
            switch (button_press.bits) {
                case 0b0000: { // No action
                    break;
                }
                case 0b0001: { // Increment the LEDs
                    ++counter;
                    break;
                }
                case 0b0010: { // Decrement the LEDs
                    --counter;
                    break;
                }
                case 0b0100: { // Shift the LEDs to the right.
                    counter.apply([](auto count) { return count >> 1u; });
                    break;
                }
                case 0b1000: { // Shift the LEDs to the left.
                    counter.apply([](auto count) { return count << 1u; });
                    break;
                }
                default: {
                    // Confirm that there are only four push buttons.
                    static_assert(de1soc_config::Keys::k_switch_count == 4);
                    // Since the above is true, we can see that all possible
                    // button states are enumerated above.

                    // Signal to compiler that this branch is impossible [P0627r0].
                    __builtin_unreachable();

                }
            } // switch
        }

        // Update the board's LEDs and seven-segment display.
        // Use explicit casts to suppress warnings about loss of precision.
        board.leds.write_all(static_cast<unsigned int>(counter));
        board.display.show_number(static_cast<int>(counter));

        previous_button = button_press;

        std::this_thread::sleep_for(REFRESH_PERIOD);

    } // while (true)

}

}
