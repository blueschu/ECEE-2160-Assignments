/**
 * ECEE 2160 Lab Assignment 4 - DE1-SoC specialized register I/O.
 *
 * Author:  Brian Schubert
 * Date:    2020-08-01
 */

#ifndef ECEE_2160_LAB_REPORTS_DE1SOC_REGISTER_IO_H
#define ECEE_2160_LAB_REPORTS_DE1SOC_REGISTER_IO_H

#include "de1soc_properties.h"
#include "posix_api.h"
#include "register_io.h"

/**
 * Handle class for interacting with hardware devices on the DE1-SoC board.
 *
 * In contrast with the RegisterIO base class, this class is aware of the
 * properties of the DE1-SoC board, such as the word size of its control
 * registers.
 *
 * For general use, only one instance of this class should be created to
 * minimize wasted system resources.
 */
class DE1SoCRegisterIO : public RegisterIO<de1soc_config::Register> {

    /**
     * Physical-to-virtual memory mapping on the DE1-SoC board.
     *
     * Declared as mutable since it was desirable to still allow register
     * reads from logically constant instances of DE1SoCfgpa.
     */
    mutable posix_api::MemoryMapping m_memory_mapping;

  public:

    /**
     * Constructors a DE1SoCRegisterIO with the provided memory mapping serving
     * as the devices physical memory.
     *
     * @param memory_mapping Physical memory access on DE1-SoC board.
     */
    explicit DE1SoCRegisterIO(posix_api::MemoryMapping memory_mapping)
        : m_memory_mapping{std::move(memory_mapping)} {}

    /**
     * Acquires a memory mapping to /dev/mem through POSIX api system calls
     * and returns a DE1SoCRegisterIO with access to the boards physical memory.
     *
     * @return DE1SoCRegisterIO with physical memory access.
     */
    [[nodiscard]]
    static DE1SoCRegisterIO new_connection();

    /**
     * Read a 4-byte value from the specified mapped I/O location.
     *
     * This function was based on an example provided by the instructor.
     *
     * Added nodiscard attribute per clang-tidy recommendation.
     *
     * @param reg_offset Offset to device to the register relative to the mapping base..
     * @return Register value read.
    */
    [[nodiscard]]
    Register read_register(std::size_t offset) const override;

    /**
     * Write a 4-byte value from the specified mapped I/O location.
     *
     * This function was based on an example provided by the instructor.
     *
     * @param reg_offset Offset to device to the register relative to the mapping base..
     * @return Register value to be written.
    */
    void write_register(std::size_t offset, Register value) override;

};

#endif //ECEE_2160_LAB_REPORTS_DE1SOC_REGISTER_IO_H
