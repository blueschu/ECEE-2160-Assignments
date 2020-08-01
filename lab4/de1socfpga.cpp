//
// Created by brian on 8/1/20.
//

#include "de1socfpga.h"

#include <stdexcept>        // for std::runtime_error

DE1SoCfgpa::DE1SoCfgpa()
{
    using namespace posix_api;

    auto fd = File("/dev/mem", FileFlag::ReadWrite | FileFlag::Sync);
    m_memory_mapping = MemoryMapping(
        fd,
        LW_BRIDGE_SPAN,
        MemoryFlag::Write | MemoryFlag::Read,
        LW_BRIDGE_BASE
    );
    if (!m_memory_mapping) {
        throw std::runtime_error("failed to create memory mapping");
    }
}

DE1SoCfgpa::Register DE1SoCfgpa::ReadRegister(std::size_t offset) const {
    return *m_memory_mapping.access_memory<Register>(offset);
}

void DE1SoCfgpa::WriteRegister(std::size_t offset, DE1SoCfgpa::Register value)
{
    *m_memory_mapping.access_memory<Register>(offset) = value;
}
