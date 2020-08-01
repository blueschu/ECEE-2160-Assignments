//
// Created by brian on 8/1/20.
//

#include "de1socfpga.h"

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
}
