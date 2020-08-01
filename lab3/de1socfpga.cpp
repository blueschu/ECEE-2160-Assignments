#include "de1socfpga.h"

// This headers must precede the posix headers due to implicit dependencies.
#include <cstdlib>

// Wrap POSIX API headers in a namespace to make the origin of symbols clearer.
namespace posix_api { // Note: the namespace `posix` is reserved.
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
}

#include <cstddef>      // for std::byte


// When defined, runtime pointer alignment checks will be added to
// the function `access_register`.
#define CHECK_ALIGNMENT

#ifdef CHECK_ALIGNMENT
#include <iostream>     // std::cerr is required when alignment checks are enabled
#endif

// Using anonymous namespace to given symbols internal linkage.
namespace {

/// Physical base address of FPGA Devices.
constexpr inline std::size_t LW_BRIDGE_BASE{0xFF'20'00'00};

/// Length of memory-mapped IO window
constexpr inline std::size_t LW_BRIDGE_SPAN{0x00'00'50'00};

} // end namespace

/**
 * Helper function for accessing mapped physical addresses from the given
 * virtual memory base and byte offset.
 *
 * Note that the reg_offset MUST be a multiple of sizeof(Register).
 *
 * @param reg_offset    The offset in bytes (CHAR_BIT width) to the desired
 *                      hardware register.
 * @return              Virtual address pointer to the physical register.
 */
volatile DE1SoCfpga::Register*
DE1SoCfpga::access_register(std::size_t reg_offset)
{
    // Cast base pointer to virtual memory mapping to std::byte to allow
    // byte-based pointer arithmetic.
    const auto register_ptr = static_cast<std::byte*>(m_virtual_base) + reg_offset;

#ifdef CHECK_ALIGNMENT
    // Sanity check for whether the register pointer is properly aligned.
    // The processor on the DE1-SoC (ARMv7) allows unaligned reads/writes,
    // so we do not concern ourselves with raising an error in the event of
    // a misaligned register pointer.
    // However, this check is still useful for detecting accidental uses of
    // invalid reg_offset arguments.
    if (0 != reinterpret_cast<std::uintptr_t>(register_ptr) % alignof(DE1SoCfpga::Register)) {
        std::cerr << "WARNING: Attempting unaligned access to " << register_ptr
                  << " (base offset=" << reg_offset << ")\n";
    }
#endif

    // Return calculated device address as a pointer to a hardware resister.
    return reinterpret_cast<volatile DE1SoCfpga::Register*>(register_ptr);
    // Note: according to the final draft of the C++17 standard, the result of
    // the reinterpret_cast above is unspecified if register_ptr does not meet
    // the alignment requirements of Register [8.2.1/7,8.2.9/13]. In practice,
    // the cast still "works" with misaligned pointers on the DE1-SoC.
}

DE1SoCfpga::DE1SoCfpga()
{
    // Open /dev/mem character device to give access to physical addresses.
    m_fd = posix_api::open("/dev/mem", (O_RDWR | O_SYNC));

    // Check for errors in opening /dev/mem.
    if (m_fd == FNCTL_OPEN_FAILED) {
        throw DE1SoCInitializationError("failed to open /dev/mem.");
    }

    // Get a mapping from physical addresses to virtual addresses.
    m_virtual_base = posix_api::mmap(
        // Let the kernel choose the virtual address.
        nullptr,
        // Ensure the mapping covers the necessary width.
        LW_BRIDGE_SPAN,
        // Set memory protection to allow reads and writes.
        (PROT_READ | PROT_WRITE),
        MAP_SHARED,
        m_fd,
        // Physical address that the mapping should be directed to.
        LW_BRIDGE_BASE
    );
    // Check for errors in acquiring memory mapping.
    if (m_virtual_base == MAP_FAILED) {
        posix_api::close(m_fd);
        throw DE1SoCInitializationError("failed to acquire memory mapping");
    }
}

DE1SoCfpga::~DE1SoCfpga() noexcept
{
    if (m_virtual_base != MAP_FAILED) {

        // Deallocate the mapped memory.
        if (posix_api::munmap(m_virtual_base, LW_BRIDGE_SPAN) != 0) {
            // Destructor cannot fail; terminate program.
            std::terminate();
        }
        // Set base pointer to nullptr to reduce chance of accidental misuse.
        m_virtual_base = MAP_FAILED;
    }

    if (m_fd != FNCTL_OPEN_FAILED) {
        // Release the file descriptor for /dev/mem/.
        posix_api::close(m_fd);
        // Set file descriptor to the failed sentinel to prevent accidental misuse.
        m_fd = FNCTL_OPEN_FAILED;
    }

}



