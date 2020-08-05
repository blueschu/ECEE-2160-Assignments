/**
 * ECEE 2160 Lab Assignment 4 - DE1-SoC specialized register I/O.
 *
 * Author:  Brian Schubert
 * Date:    2020-08-01
 */

#include "de1soc_register_io.h"

DE1SoCRegisterIO::Register DE1SoCRegisterIO::read_register(std::size_t offset) const
{
    return *m_memory_mapping.access_memory<Register>(offset);
}

void DE1SoCRegisterIO::write_register(std::size_t offset, DE1SoCRegisterIO::Register value)
{
    *m_memory_mapping.access_memory<Register>(offset) = value;
}

DE1SoCRegisterIO DE1SoCRegisterIO::new_connection()
{
    using namespace posix_api;

    // POSIX file descriptor to /dev/mem with read/write access.
    const auto fd = File("/dev/mem", FileFlag::ReadWrite | FileFlag::Sync);

    // Mapping between this process' virtual hardware space and the devices
    // physical memory. This call duplicates the file descriptor fd, so it can
    // be safely destructed.
    auto mapping = MemoryMapping(
        fd,
        de1soc_config::bridge_span,
        MemoryFlag::Write | MemoryFlag::Read,
        de1soc_config::bridge_base
    );

    if (!mapping) {
        throw MemoryMappingError(
            "failed to open memory mapping to DE1-SoC board physical memory"
        );
    }

    return DE1SoCRegisterIO{std::move(mapping)};
}

