//
// Created by brian on 8/1/20.
//

#ifndef ECEE_2160_LAB_REPORTS_DE1SOC_fpga_H
#define ECEE_2160_LAB_REPORTS_DE1SOC_fpga_H

#include "posix_api.h"

class DE1SoCfgpa {

    /**
     * Physical-to-virtual memory mapping on the DE1-SoC board.
     *
     * Declared as mutable since it was desirable to still allow register
     * reads from logically constant instances of DE1SoCfgpa.
     */
    mutable posix_api::MemoryMapping m_memory_mapping;

  public:

    /// Physical base address of FPGA Devices.
    constexpr static inline std::size_t LW_BRIDGE_BASE{0xFF'20'00'00};

    /// Length of memory-mapped IO window
    constexpr static inline std::size_t LW_BRIDGE_SPAN{0x00'00'50'00};


    /**
     * Integral type for FPGA device registers.
     *
     * Per documentation from the instructor, we can expect all registers
     * to occupy precisely 32 bits on the DE1-SoC.
     */
    using Register = std::uint32_t;

    DE1SoCfgpa();

    Register ReadRegister(std::size_t offset) const;

    void WriteRegister(std::size_t offset, Register value);

};

#endif //ECEE_2160_LAB_REPORTS_DE1SOC_fpga_H
