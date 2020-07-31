/**
 * ECEE 2160 Lab Assignment 3 DE1-SoC interface class.
 *
 * Many symbols declared in this file are repeated in `prelab/prelab_fgpa.h`.
 * These two interfaces should not be used simultaneously.
 *
 * Completed for lab assignment 3.5.
 *
 * All symbol names were specified by the lab instructions.
 *
 * Author:  Brian Schubert
 * Date:    2020-07-26
 *
 * References
 * ===========
 *
 *  [1] https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
 */

#ifndef ECEE_2160_LAB_REPORTS_DE1SOCFPGA_H
#define ECEE_2160_LAB_REPORTS_DE1SOCFPGA_H

// Make sure we don't accidentally mix this header's declarations with those
// in ./prelab/prelab_fpga.h.
#ifdef ECEE_2160_LAB_REPORTS_PRELAB_FPGA_H
#error "The prelab header must not be used with the Assignment 5 header"
#endif

#include <cstdint>      // for std::uint32_t, std::size_t
#include <stdexcept>    // for std::runtime_error
#include <utility>      // for std::exchange

/**
 * Exception class thrown when initialization a memory mapping on the DE1-SoC
 * fails.
 */
class DE1SoCInitializationError : public std::runtime_error {
  public:
    // Use the base class constructor.
    using std::runtime_error::runtime_error;
};

/**
 * An interface to memory-mapped devices on the DE1-SoC board.
 *
 * Implemented for assignment 5.
 *
 * Note: this class is responsible for managing two different resources: a file
 * descriptor for /dev/mem, and a physical-to-virtual memory mapping.
 *
 */
class DE1SoCfpga {

    /**
     * Pointer type indicating the start of a mapping between physical memory
     * and virtual memory.
     */
    using VirtualMappingBase = void*;

    /**
     * A file descriptor for interface with fnctl.h.
     */
    using FileDescriptor = int;

    /// Value returned by 'open' from fnctl.h upon failing to open a file.
    constexpr static inline FileDescriptor FNCTL_OPEN_FAILED = -1;

    /**
     * Pointer type indicating the start of a mapping between physical memory
     * and virtual memory.
     */
    VirtualMappingBase m_virtual_base;

    /**
     * A file descriptor for /dev/mem acquired from posix api.
     */
    FileDescriptor m_fd;

  public:
    /**
     * Integral type for FPGA device registers.
     *
     * Per documentation from the instructor, we can expect all registers
     * to occupy precisely 32 bits on the DE1-SoC.
     */
    using Register = std::uint32_t;

    /**
     * Initializes a physical-to-virtual memory-mapping.
     *
     * @throws DE1SoCInitializationError When opening /dev/mem or acquiring
     *              a memory mapping from POSIX api fails.
     */
    DE1SoCfpga();

    /// Destructor.
    /// Not declared virtual since we do not expect this class to be inherited.
    ~DE1SoCfpga() noexcept;

    /**
     * Write a 4-byte value from the specified mapped I/O location.
     *
     * This function was based on an example provided by the instructor.
     *
     * @param reg_offset Offset to device to the register relative to the mapping base..
     * @return Register value to be written.
     */
    void RegisterWrite(std::size_t reg_offset, Register value)
    {
        *access_register(reg_offset) = value;
    }

    /**
     * Read a 4-byte value from the specified mapped I/O location.
     *
     * This function was based on an example provided by the instructor.
     *
     * @param reg_offset Offset to device to the register relative to the mapping base..
     * @return Register value read.
     */
    Register RegisterRead(std::size_t reg_offset)
    {
        return *access_register(reg_offset);
    }

    /*
     * Copy/move constructors/assignments were not required for this assignment.
     *
     * To prevent accidents, we explicitly disallow the compiler from generating
     * them [C.21,C.81 in 1].
     */
    DE1SoCfpga(const DE1SoCfpga&) = delete;

    DE1SoCfpga& operator=(const DE1SoCfpga&) = delete;

    DE1SoCfpga& operator=(DE1SoCfpga&&) = delete;

    // Move constructor required in assignment 5.
    DE1SoCfpga(DE1SoCfpga&& other) noexcept
        : m_virtual_base{std::exchange(other.m_virtual_base, nullptr)},
          m_fd{std::exchange(other.m_fd, FNCTL_OPEN_FAILED)} {};

  private:
    volatile Register* access_register(std::size_t reg_offset);

};

#endif //ECEE_2160_LAB_REPORTS_DE1SOCFPGA_H
