//
// Created by brian on 8/1/20.
//

#ifndef ECEE_2160_LAB_REPORTS_DE1SOC_fpga_H
#define ECEE_2160_LAB_REPORTS_DE1SOC_fpga_H

#include "posix_api.h"

/**
 * Handle class for interacting with hardware devices on the DE1-SoC board.
 *
 * Per lab assignment instructions, this class serves as a parent class for
 * all class that need to interact with DE1-SoC hardware.
 *
 * In order to ensure that only one memory mapping is created, we implement
 * a sort-of singleton MemoryMapping object, which is accessible from the member
 * function get_memory_mapping().
 */
class DE1SoCHardwareDevice {

  protected:

    /**
     * Integral type for FPGA device registers.
     *
     * This type is declared as protected since code not contained in a
     * hardware interface class should not need to know the size of the
     * DE1-SoC's registers.
     *
     * Per documentation from the instructor, we can expect all registers
     * to occupy precisely 32 bits on the DE1-SoC.
     */
    using Register = std::uint32_t;

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
    static Register read_register(std::size_t offset);

    /**
     * Write a 4-byte value from the specified mapped I/O location.
     *
     * This function was based on an example provided by the instructor.
     *
     * @param reg_offset Offset to device to the register relative to the mapping base..
     * @return Register value to be written.
    */
    static void write_register(std::size_t offset, Register value);

  private:
    /**
     * Physical-to-virtual memory mapping on the DE1-SoC board.
     *
     * @return Physical memory mapping.
     */
    static posix_api::MemoryMapping& get_memory_mapping();

};

#endif //ECEE_2160_LAB_REPORTS_DE1SOC_fpga_H
