//
// Created by brian on 8/3/20.
//

#include <stdexcept>            // for std::out_of_range

template<std::size_t N>
void LedArray<N>::update_leds()
{
    write_register(m_base_offset, m_led_state);
}

template<std::size_t N>
void LedArray<N>::write(std::size_t index, bool state)
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

template<std::size_t N>
void LedArray<N>::write_all(Register led_bits)
{
    m_led_state = led_bits;
    update_leds();
}
