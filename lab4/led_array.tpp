/**
 * ECEE 2160 Lab Assignment 4 - Generic memory-mapped LED I/O.
 *
 * Author:  Brian Schubert
 * Date:    2020-08-03
 */

#include <stdexcept>            // for std::out_of_range

template<std::size_t N, typename Reg>
void LedArray<N, Reg>::update_leds()
{
    m_register_io->write_register(m_base_offset, m_led_state);
}

template<std::size_t N, typename Reg>
void LedArray<N, Reg>::write(std::size_t index, bool state)
{
    if (index >= N) {
        throw std::out_of_range("index must not exceed led array range");
    }

    // Bit mask identifying the controlling bit for the specified LED.
    const Register led_mask{1u << index};

    if (state) {
        m_led_state |= led_mask;
    } else {
        m_led_state &= led_mask;
    }
    update_leds();
}

template<std::size_t N, typename Reg>
void LedArray<N, Reg>::write_all(Register led_bits)
{
    m_led_state = led_bits;
    update_leds();
}
