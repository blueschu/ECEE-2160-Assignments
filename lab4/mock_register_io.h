/**
 * ECEE 2160 Lab Assignment 4 - Mock register I/O.
 *
 * Author:  Brian Schubert
 * Date:    2020-08-04
 *
 * References
 * ==========
 *
 *  - https://en.cppreference.com/w/cpp/container/map
 *  - https://stackoverflow.com/questions/2273330/
 */

#ifndef ECEE_2160_LAB_REPORTS_MOCK_REGISTER_IO_H
#define ECEE_2160_LAB_REPORTS_MOCK_REGISTER_IO_H

#include <cstddef>          // for std::uint32_t
#include <iostream>         // for std::cout
#include <iomanip>          // for std::hex
#include <map>              // for std::map

/**
 * Mock handle class for interacting with hardware devices.
 */
class MockRegisterIO : public RegisterIO<std::uint32_t> {

    /**
     * Mapping between register offsets and their values.
     *
     * We use std::map instead of std::unordered_map since a sequential
     * view of mapped addresses may be desirable in the future.
     *
     * This variable is declared mutable since map::operator[] inserts a
     * zero value if the lookup fails.
     */
    mutable std::map<size_t, Register> m_mock_memory{};

    /// Helper struct to reset an output stream's format flags on destruction.
    struct FlagSaver {
        std::ostream& m_out;
        const std::ios::fmtflags m_flags;

        explicit FlagSaver(std::ostream& out) : m_out{out}, m_flags{out.flags()} {}

        ~FlagSaver() { m_out.flags(m_flags); }
    };

  public:

    /// Creates a MockRegisterIO with empty registers.
    MockRegisterIO() = default;

    /// Creates a MockRegisterIO with the given register memory.
    explicit MockRegisterIO(std::map<size_t, Register> register_memory)
        : m_mock_memory{std::move(register_memory)} {}

    /**
     * Read a 4-byte value from the specified mapped I/O location.
     *
     * Added nodiscard attribute per clang-tidy recommendation.
     *
     * @param reg_offset Offset to device to the register relative to the mapping base..
     * @return Register value read.
    */
    [[nodiscard]]
    Register read_register(std::size_t offset) const override
    {
        FlagSaver _{std::cout};
        std::cout << std::hex;
        // Inserts register of 0 if a register does not already exist.
        auto value = m_mock_memory[offset];
        std::cout << "Read register " << offset << "(0x" << value << ")\n";
        return value;
    }

    /**
     * Write a 4-byte value from the specified mapped I/O location.
     *
     * @param reg_offset Offset to device to the register relative to the mapping base..
     * @return Register value to be written.
    */
    void write_register(std::size_t offset, Register value) override
    {
        FlagSaver _{std::cout};
        std::cout << std::hex;
        std::cout << "Write register " << offset << " (0x" << value << ")\n";
        m_mock_memory[offset] = value;
    }

};

#endif //ECEE_2160_LAB_REPORTS_MOCK_REGISTER_IO_H
