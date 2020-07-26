/**
 * ECEE 2160 Lab Assignment 3 prelab utilities for memory-mapped I/O.
 *
 * Functions were adapted from a set of C-style utility function provided
 * by the instructor. The original version of these functions can be found
 * in `instructor/prelab_fpga.cpp`.
 *
 * Author:  Brian Schubert
 * Date:    2020-07-23
 *
 * References
 * ==========
 *
 *  - https://stackoverflow.com/questions/13881487/should-i-worry-about-the-alignment-during-pointer-casting
 *  - https://stackoverflow.com/questions/32062894/take-advantage-of-arm-unaligned-memory-access-while-writing-clean-c-code
 */


#include "prelab_fpga.h"

// This headers must precede the posix headers due to implicit dependencies.
#include <cstdlib>

// Wrap POSIX API headers in a namespace to make the origin of symbols clearer.
namespace posix_api {

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

} // end namespace posix_api

#include <cstddef>      // for std::byte
#include <iostream>     // for std::cerr
#include <stdexcept>    // for std::out_of_range

// When defined, runtime pointer alignment checks will be added to
// the function `access_register`.
#define CHECK_ALIGNMENT

// Using anonymous namespace to given symbols internal linkage.
namespace {

/**
 * Helper function for accessing mapped physical addresses from the given
 * virtual memory base and byte offset.
 *
 * Note that the reg_offset MUST be a multiple of sizeof(Register).
 *
 * @param virtual_base  The base pointer to the virtual memory mapped to
 *                      physical addresses.
 * @param reg_offset    The offset in bytes (CHAR_BIT width) to the desired
 *                      hardware register.
 * @return              Virtual address pointer to the physical register.
 */
volatile Register*
access_register(VirtualMappingBase virtual_base, std::size_t reg_offset)
{
    // Cast base pointer to virtual memory mapping to std::byte to allow
    // byte-based pointer arithmetic.
    const auto register_ptr = static_cast<std::byte*>(virtual_base) + reg_offset;

#ifdef CHECK_ALIGNMENT
    // Sanity check for whether the register pointer is properly aligned.
    // The processor on the DE1-SoC (ARMv7) allows unaligned reads/writes,
    // so we do not concern ourselves with raising an error in the event of
    // a misaligned register pointer.
    // However, this check is still useful for detecting accidental uses of
    // invalid reg_offset arguments.
    if (0 != reinterpret_cast<std::uintptr_t>(register_ptr) % alignof(Register)) {
        std::cerr << "WARNING: Attempting unaligned access to " << register_ptr
                  << " (base offset=" << reg_offset << ")\n";
    }
#endif

    // Return calculated device address as a pointer to a hardware resister.
    return reinterpret_cast<volatile Register*>(register_ptr);
    // Note: according to the final draft of the C++17 standard, the result of
    // the reinterpret_cast above is unspecified if register_ptr does not meet
    // the alignment requirements of Register [8.2.1/7,8.2.9/13]. In practice,
    // the cast still "works" with misaligned pointers on the DE1-SoC.
}

} // end namespace

void RegisterWrite(VirtualMappingBase virtual_base, std::size_t reg_offset, Register value)
{
    *access_register(virtual_base, reg_offset) = value;
}

Register RegisterRead(VirtualMappingBase virtual_base, std::size_t reg_offset)
{
    return *access_register(virtual_base, reg_offset);
}

std::pair<VirtualMappingBase, FileDescriptor> Initialize()
{
    // Open /dev/mem character device to give access to physical addresses.
    FileDescriptor fd = posix_api::open("/dev/mem", (O_RDWR | O_SYNC));

    // Check for errors in opening /dev/mem.
    if (fd == FNCTL_OPEN_FAILED) {
        // Print error message and exit with non-zero status.
        std::cerr << "ERROR: could not open /dev/mem.\n";
        std::exit(EXIT_FAILURE);
    }

    // Get a mapping from physical addresses to virtual addresses.
    VirtualMappingBase virtual_base = posix_api::mmap(
        // Let the kernel choose the virtual address.
        nullptr,
        // Ensure the mapping covers the necessary width.
        LW_BRIDGE_SPAN,
        // Set memory protection to allow reads and writes.
        (PROT_READ | PROT_WRITE),
        MAP_SHARED,
        fd,
        // Physical address that the mapping should be directed to.
        LW_BRIDGE_BASE
    );
    // Check for errors in acquiring memory mapping.
    if (virtual_base == MAP_FAILED) {
        // Print error message, close file descriptor, and exit
        // with non-zero status.
        std::cerr << "ERROR: mmap() failed.\n";
        posix_api::close(fd);
        std::exit(EXIT_FAILURE);
    }
    return {virtual_base, fd};
}

void Finalize(VirtualMappingBase& virtual_base, FileDescriptor& fd)
{
    // Deallocate the mapped memory.
    if (posix_api::munmap(virtual_base, LW_BRIDGE_SPAN) != 0) {
        std::cout << "ERROR: munmap() failed.\n";
        std::exit(EXIT_FAILURE);
    }
    // Set base pointer to nullptr to reduce chance of accidental misuse.
    virtual_base = nullptr;

    // Release the file descriptor for /dev/mem/.
    posix_api::close(fd);
    // Set file descriptor to the failed sentinel to prevent accidental misuse.
    fd = FNCTL_OPEN_FAILED;
}

/*
 * The following two functions were implemented for the prelab assignment.
 */

void Write1Led(VirtualMappingBase virtual_base, std::size_t led_index, bool state)
{
    if (led_index >= LED_COUNT) {
        throw std::out_of_range("invalid LED index.");
    }

    // Bit mask identifying the controlling bit for the specified LED.
    const Register led_mask{1u << led_index};
    // The current state of the LEDs.
    Register led_state = RegisterRead(virtual_base, LEDR_BASE);

    if (state) { // Set an LED to ON.
        led_state |= led_mask;
    } else { // Set an LED to OFF.
        led_state &= ~led_mask;
    }

    RegisterWrite(virtual_base, LEDR_BASE, led_state);
}

bool Read1Switch(VirtualMappingBase virtual_base, std::size_t switch_index)
{
    if (switch_index >= SWITCH_COUNT) {
        throw std::out_of_range("invalid switch index.");
    }

    // The current state of the switches.
    Register switch_state = RegisterRead(virtual_base, SW_BASE);

    // Shift the switch state so that the target switch's bit is the LSB.
    switch_state >>= switch_index;

    // Return the state of the LSB.
    return switch_state & 1u;
}
