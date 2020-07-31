/**
 * ECEE 2160 Lab Assignment 3.4 - Controlling the push buttons.
 *
 * Author:  Brian Schubert
 * Date:    2020-07-26
 *
 * References
 * ==========
 *
 * - https://en.cppreference.com/w/cpp/language/operator_incdec
 * - https://en.cppreference.com/w/cpp/language/user_literal
 * - https://en.cppreference.com/w/cpp/header/thread
 *
 */

#include <chrono>           // for std::chrono::duration
#include <stdexcept>        // for std::domain_error
#include <thread>           // for std::this_thread

#include "prelab/prelab_fpga.h"


// Using anonymous namespace to given symbols internal linkage.
namespace {

// For access to duration literals.
using namespace std::literals::chrono_literals;

/**
 * The period of time elapsed between button reads.
 */
constexpr std::chrono::duration REFRESH_PERIOD = 1ms;

/**
 * A counter over an interval [0,max] for some max that wraps around its
 * endpoints.
 */
class WrappedCounter {
    /**
     * Integral type used to represent this counter's value.
     */
    using Count = std::uint64_t;

    /**
     * The current state of the counter.
     */
    Count m_couter{0};

    /**
     * The maximum value that the counter can reached before wrapping to 0.
     */
    const Count m_max;

  public:
    /**
     * Constructs a counter that wraps upon advancing past the given maximum.
     *
     * @param max Maximum counter value.
     */
    explicit WrappedCounter(Count max) : m_max{max} {}

    /**
     * Returns the counter state of the counter.
     *
     * @return Counter state.
     */
    Count value() const { return m_couter; }

    /**
     * Returns the maximum value of the counter.
     *
     * @return Max value.
     */
    Count max() const { return m_max; }

    /**
     * Sets the counter to the given value.
     *
     * @param value New counter value.
     * @throws domain_error if the new value does not fall within the
     *          interval of this counter.
     */
    void set_value(Count value)
    {
        if (value > m_max) {
            throw std::domain_error("counter value cannot exceed maximum");
        }
        m_couter = value;
    }


    Count operator++()
    {
        m_couter = m_couter == m_max ? 0 : m_couter + 1;
        return m_couter;
    }

    Count operator--()
    {
        m_couter = m_couter == 0 ? m_max : m_couter - 1;
        return m_couter;
    }

    Count operator++(int)
    {
        auto tmp = m_couter;
        ++(*this);
        return tmp;
    }

    Count operator--(int)
    {
        auto tmp = m_couter;
        --(*this);
        return tmp;
    }
};

void run_button_demo(VirtualMappingBase virtual_base);

} // end namespace

int main()
{
    auto[virtual_base, fd] = Initialize();

    run_button_demo(virtual_base);

    Finalize(virtual_base, fd);
}

namespace {

void run_button_demo(VirtualMappingBase virtual_base)
{
    // Counter holding the state to be written to the board's LEDs.
    WrappedCounter counter{(1u << LED_COUNT) - 1};

    // The state of the DE1SoC's button's during the previous cycle.
    auto previous_button = PushButton::None;

    while (true) {
        // Current state of the board's buttons.
        auto button_press = PushButtonGet(virtual_base);

        if (previous_button == PushButton::Multiple && button_press != PushButton::None) {
            // The user recently pressed multiple buttons. Wait until all
            // buttons have been released before taking any new actions.
            std::this_thread::sleep_for(REFRESH_PERIOD);
            continue;
        }

        // Only take action if the button state has changed.
        if (button_press != previous_button) {
            previous_button = button_press;

            switch (button_press) {
                case (PushButton::None): {
                    break;
                }
                case (PushButton::Button0): {
                    ++counter;
                    break;
                }
                case PushButton::Button1: {
                    --counter;
                    break;
                }
                case PushButton::Button2: {
                    auto new_counter = counter.value() >> 1u;
                    counter.set_value(new_counter);
                    break;
                }
                case PushButton::Button3: {
                    auto new_counter = counter.value() << 1u;
                    // Trim bits that "overflowed" out the of range of valid
                    // LED states.
                    new_counter %= (counter.max() + 1);
                    try {
                        counter.set_value(new_counter);
                    } catch (const std::domain_error&) {
                        counter.set_value(0);
                    }
                    break;
                }
                case PushButton::Multiple: {
                    // Turn off all LEDs.
                    counter.set_value(0);
                }
            }

            WriteAllLeds(virtual_base, static_cast<Register>(counter.value()));
        }

        std::this_thread::sleep_for(REFRESH_PERIOD);
    }

}

} // end namespace


