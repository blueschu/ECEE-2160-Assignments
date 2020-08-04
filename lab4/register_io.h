/**
 * ECEE 2160 Lab Assignment 4 - Abstract memory-mapped I/O base class.
 *
 * Author:  Brian Schubert
 * Date:    2020-08-03
 */

#ifndef ECEE_2160_LAB_REPORTS_REGISTER_IO_H
#define ECEE_2160_LAB_REPORTS_REGISTER_IO_H

/**
 * An abstract base class for memory-mapped I/O classes.
 *
 * This class represents a generic interface to reading and writing to control
 * registers on generic boards.
 *
 * @tparam Reg The integral type used to represent a register on the
 *             target board.
 */
template<typename Reg>
class RegisterIO {
  public:
    /**
     * Integral type for control registers.
     */
    using Register = Reg;


    /**
     * Read a 4-byte value from the specified mapped I/O location.
     *
     * Added nodiscard attribute per clang-tidy recommendation.
     *
     * @param reg_offset Offset to device to the register relative to the mapping base..
     * @return Register value read.
    */
    [[nodiscard]]
    virtual Register read_register(std::size_t offset) const = 0;

    /**
     * Write a 4-byte value from the specified mapped I/O location.
     *
     * This function was based on an example provided by the instructor.
     *
     * @param reg_offset Offset to device to the register relative to the mapping base..
     * @return Register value to be written.
    */
    virtual void write_register(std::size_t offset, Register value) = 0;
};

#endif //ECEE_2160_LAB_REPORTS_REGISTER_IO_H
