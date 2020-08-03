//
// Created by brian on 8/3/20.
//

#include <stdexcept>            // for std::out_of_range

template<std::size_t N>
bool SwitchArray<N>::read(std::size_t index) const
{
    if (index >= N) {
        throw std::out_of_range("index must not exceed switch array range");
    }
    // The current state of the switches.
    Register switch_state = read_all();

    // Shift the switch state so that the target switch's bit is the LSB.
    switch_state >>= index;

    // Return the state of the LSB.
    return switch_state & 1u;
}

template<std::size_t N>
typename SwitchArray<N>::State SwitchArray<N>::read_all() const
{
    return read_register(m_base_offset);
}
