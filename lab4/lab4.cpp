//
// Created by brian on 8/1/20.
//

#include "posix_api.h"

int main()
{
    using namespace posix_api;
    auto fd = File("/dev/mem", FileFlag::ReadWrite | FileFlag::Sync);

    auto mem_mapping = MemoryMapping(fd, 1, MemoryFlag::Read | MemoryFlag::Write, 1);
}
