//
// Created by brian on 8/1/20.
//

// https://stackoverflow.com/questions/1008019/c-singleton-design-pattern

#include "de1soc_device.h"
#include "de1soc_properties.h"

DE1SoCHardwareDevice::Register DE1SoCHardwareDevice::read_register(std::size_t offset)
{
    return *get_memory_mapping().access_memory<Register>(offset);
}

void DE1SoCHardwareDevice::write_register(std::size_t offset, DE1SoCHardwareDevice::Register value)
{
    *get_memory_mapping().access_memory<Register>(offset) = value;
}

posix_api::MemoryMapping& DE1SoCHardwareDevice::get_memory_mapping()
{
    // Memory mapping initialized on first use.
    static auto memory_mapping = []() {
        using namespace posix_api;

        const auto fd = File("/dev/mem", FileFlag::ReadWrite | FileFlag::Sync);

        auto mapping = MemoryMapping(
            fd,
            de1soc::bridge_span,
            MemoryFlag::Write | MemoryFlag::Read,
            de1soc::bridge_base
        );

        if (!mapping) {
            throw MemoryMappingError(
                "failed to open memory mapping to DE1-SoC board physical memory"
            );
        }
        return mapping;
    }();

    return memory_mapping;
}
