/**
 * ECEE 2160 Lab Assignment 4 - Generic memory-mapped LED I/O.
 *
 * Author:  Brian Schubert
 * Date:    2020-08-03
 */

#include <stdexcept>            // for std::out_of_range

template<std::size_t N, typename Reg>
bool SwitchArray<N, Reg>::read(std::size_t index) const
{
    if (index >= N) {
        throw std::out_of_range("index must not exceed switch array range");
    }
    // The current state of the switches.
    Register switch_state = m_register_io->read_register(m_base_offset);

    // Shift the switch state so that the target switch's bit is the LSB.
    switch_state >>= index;

    // Return the state of the LSB.
    return switch_state & 1u;
}

// Note: trailing return type used to bring State into scope without repeating
// type info. Using conventional return type syntax, we'd need to write out
// "typename SwitchArray<N, Reg>::State"
template<std::size_t N, typename Reg>
auto SwitchArray<N, Reg>::read_all() const -> State
{
    return State{m_register_io->read_register(m_base_offset)};
}
