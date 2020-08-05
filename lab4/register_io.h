/**
 * ECEE 2160 Lab Assignment 4 - Abstract memory-mapped I/O base class.
 *
 * Author:  Brian Schubert
 * Date:    2020-08-03
 */

#ifndef ECEE_2160_LAB_REPORTS_REGISTER_IO_H
#define ECEE_2160_LAB_REPORTS_REGISTER_IO_H

#include <type_traits>          // for std::is_abstract_v

/**
 * An abstract base class for memory-mapped I/O classes.
 *
 * This class represents a generic interface to reading and writing control
 * registers on generic boards.
 *
 * @tparam Reg  The integral type used to represent a register on the target board.
 */
template<typename Reg>
class RegisterIO {
  public:
    /**
     * Integral type for control registers.
     */
    using Register = Reg;

    /**
     * Read a register value from the specified mapped I/O location.
     *
     * Added nodiscard attribute per clang-tidy recommendation.
     *
     * @param reg_offset Offset to the target register relative to the mapping base.
     * @return Register value read.
    */
    [[nodiscard]]
    virtual Register read_register(std::size_t offset) const = 0;

    /**
     * Write a register value from the specified mapped I/O location.
     *
     * @param reg_offset Offset to the target register relative to the mapping base.
     * @return Register value to be written.
    */
    virtual void write_register(std::size_t offset, Register value) = 0;

    // Destructor. Marked noexcept per C.37.
    virtual ~RegisterIO() noexcept = default;
};

static_assert(
    std::is_abstract_v<RegisterIO<nullptr_t>>,
    "RegisterIO is expected to be abstract"
);

#endif //ECEE_2160_LAB_REPORTS_REGISTER_IO_H
