/**
 * ECEE 2160 Lab Assignment 4
 *
 * Author:  Brian Schubert
 * Date:    2020-08-01
 *
 */

#include <chrono>                   // for std::chrono::duration
#include <thread>                   // for std::this_thread

#include "de1soc_properties.h"
#include "seven_segment_display.h"
#include "led_array.h"
#include "switch_array.h"
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
constexpr int SWITCH_EXIT_SENTINEL = 0;

/**
 * Convenience structure to hold all DE1-SoC hardware device interfaces.
 */
struct DE1SoC {
    SevenSegmentDisplay<de1soc::seven_segment_display_count> display;

    LedArray<de1soc::led_count> led_array;

    SwitchArray<de1soc::switch_count> switch_array;

    SwitchArray<de1soc::key_count> push_buttons;
};

/**
 * Initializes all DE1-SoC hardware interfaces.
 *
 * @return New DE1SoC.
 */
DE1SoC make_de1soc();

void run_bottom_demo(DE1SoC& board);

} // end namespace




int main()
{

    auto board = make_de1soc();

    run_bottom_demo(board);

}

// Internal definitions
namespace {

void run_bottom_demo(DE1SoC& board)
{
    // Counter holding the state to be written to the board's LEDs.
    WrappingCounter counter{(1u << de1soc::led_count) - 1};

    // The state of the DE1SoC's button's during the previous cycle.
    decltype(board.push_buttons)::State previous_button{};

    while (true) {
        // Current state of the board's buttons.
        const auto button_press = board.push_buttons.read_all();

        // If true, the user recently pressed multiple buttons. We want to
        // wait until all buttons have been released before taking any new actions.
        const bool wait_for_unpress = previous_button.multiple()
            && button_press.count != 0;

        // If true, the button state has not changed, so no action should be taken.
        const bool no_button_change = button_press.bits == previous_button.bits;

        if (wait_for_unpress || no_button_change) {
            std::this_thread::sleep_for(REFRESH_PERIOD);
            continue;
        }

        if (button_press.multiple()) {
            const auto switch_state = board.switch_array.read_all().bits;

            // Set the LEDs to match the state of the switches.
            counter.apply([=](auto) { return switch_state; });

            // Check if the current state of the switches matches the exit state.
            if (switch_state == SWITCH_EXIT_SENTINEL) {
                break;
            }
        } else {
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
                    static_assert(decltype(board.push_buttons)::k_switch_count == 4);
                    // Since the above is true, we can see that all possible
                    // button states are enumerated above.

                    // Signal to compiler that this branch is impossible.
                    // todo cite
                    __builtin_unreachable();

                }
            }
        }

        // Update the board's LEDs.
        // Add explicit casts to suppress warnings about loss of precision.
        board.led_array.write_all(static_cast<unsigned int>(counter));
        board.display.show_number(static_cast<int>(counter));

        previous_button = button_press;

        std::this_thread::sleep_for(REFRESH_PERIOD);

    }

}

DE1SoC make_de1soc()
{
    using namespace de1soc;

    return {
        SevenSegmentDisplay<de1soc::seven_segment_display_count>{{hex3_hex0_base, hex5_hex4_base}},
        LedArray<led_count>{ledr_base},
        SwitchArray<switch_count>{switch_base},
        SwitchArray<key_count>{key_base},
    };
}

}
