//
// Created by brian on 8/3/20.
//

#include "led_array.h"

#include "de1soc_properties.h"

template<std::size_t N>
void LedArray<N>::update_leds()
{
    auto reg_value = static_cast<Register>(m_led_state.to_ulong());
    write_register(de1soc::ledr_base, reg_value);
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
void LedArray<N>::write_all(unsigned long led_bits)
{
    m_led_state = led_bits;
    update_leds();
}
