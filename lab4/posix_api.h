/**
 * ECEE 2160 Lab Assignment 4 POSIX API utilities.
 *
 * This is a header-only library that serves as a partial C++ wrapper around
 * the POSIX api required in this lab.
 *
 * Author:  Brian Schubert
 * Date:    2020-08-01
 *
 * References
 * ===========
 *
 * [fcntl]      https://pubs.opengroup.org/onlinepubs/7908799/xsh/fcntl.h.html
 * [mman]       https://pubs.opengroup.org/onlinepubs/7908799/xsh/mmap.html
 * [so-mmap-1]  https://stackoverflow.com/questions/55344174/c-close-a-open-file-read-with-mmap
 * [so-mmap-2]  https://stackoverflow.com/questions/17490033/do-i-need-to-keep-a-file-open-after-calling-mmap-on-it
 * [so-unaligned-1] https://stackoverflow.com/questions/13881487/should-i-worry-about-the-alignment-during-pointer-casting
 * [so-unaligned-2]  https://stackoverflow.com/questions/32062894/take-advantage-of-arm-unaligned-memory-access-while-writing-clean-c-code
 *
 */

#ifndef ECEE_2160_LAB_REPORTS_POSIX_API_H
#define ECEE_2160_LAB_REPORTS_POSIX_API_H

#include <cstddef>          // for std::byte
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace posix_api { // Note: the namespace `posix` is reserved.

// Wrap POSIX API headers in a namespace to avoid namespace pollution.
namespace raw_posix {
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
}

/**
 * File opening flags exposed by fcntl [fcntl].
 *
 * We explicitly define the underlying type to be `int` since that is the
 * type that is ultimately accepted by fcntl for file opening flags.
 *
 * Note: we only define flags for the symbolic constants used in this lab.
 */
enum class FileFlag : int {
    ReadWrite = O_RDWR,
    Sync = O_SYNC,
};

/**
 * Lightweight handle class for a posix file descriptor.
 */
class File {
  public:
    /**
     * Integral type used by fcntl to represent file descriptors.
     */
    using Descriptor = int;

  private:
    /// Value returned by 'open' from fnctl.h upon failing to open a file.
    constexpr static inline Descriptor k_failed{-1};

    /**
     * A file descriptor acquired from posix api.
     */
    Descriptor m_fd;

  public:

    File(const char* file_name, FileFlag flags)
    {
        using Flag = std::underlying_type<FileFlag>::type;
        m_fd = raw_posix::open(file_name, static_cast<Flag>(flags));
    }

    ~File() noexcept
    {
        if (m_fd != k_failed) {
            // Release the file descriptor.
            raw_posix::close(m_fd);
            // Set file descriptor to the failed sentinel.
            m_fd = k_failed;
        }
    }

    File(const File&) = delete;

    File(File&& other) noexcept
        : m_fd{std::exchange(other.m_fd, k_failed)} {}

    File& operator=(const File&) = delete;

    File& operator=(File&& other) noexcept
    {
        m_fd = std::exchange(other.m_fd, k_failed);
        return *this;
    }

    Descriptor raw_descriptor() const
    {
        return m_fd;
    }

    explicit operator bool() const
    {
        return m_fd != k_failed;
    }

};

/**
 * Memory protection flags exposed by sys/mman [mman].
 *
 * We explicitly define the underlying type to be `int` since that is the
 * type that is ultimately accepted by mmap for memory protection flags.
 *
 * Note: we only define flags for the symbolic constants used in this lab.
 */
enum class MemoryFlag : int {
    Read = PROT_READ,
    Write = PROT_WRITE,
};

/**
 * Error class thrown by MemoryMapping for invalid memory operations.
 */
class MemoryMappingError : public std::runtime_error {
    // Use base class constructor.
    using std::runtime_error::runtime_error;
};

class MemoryMapping {

    /**
     * Pointer type indicating the start of a mapping between physical memory
     * and virtual memory acquired from mmap.
     */
    void* m_virtual_base{MAP_FAILED};

    /**
     * Width of the memory mapping.
     */
    std::size_t m_map_span{0};

  public:
    MemoryMapping() = default;

    /**
     *
     * The file descriptor can be safely passed by reference since we don't
     * care if it is closed while the MemoryMapping instance is in use. Per the
     * POSIX documentation [mman]:
     *
     * > The mmap() function adds an extra reference to the file associated with
     * > the file descriptor fildes which is not removed by a subsequent close()
     * > on that file descriptor. This reference is removed when there are no
     * > more mappings to the file.
     *
     * This constructor always used MAP_SHARED for the memory sharing flag.
     *
     * @param fd
     * @param bridge_span
     * @param protection_flags
     * @param bridge_base
     */
    explicit MemoryMapping(
        const File& fd,
        std::size_t bridge_span,
        MemoryFlag protection_flags,
        std::size_t bridge_base
    ) : m_map_span{bridge_base}
    {
        using ProtectionFlag = std::underlying_type<MemoryFlag>::type;

        // Get a mapping from physical addresses to virtual addresses.
        m_virtual_base = raw_posix::mmap(
            // Let the kernel choose the virtual address.
            nullptr,
            bridge_span,
            static_cast<ProtectionFlag>(protection_flags),
            MAP_SHARED,
            fd.raw_descriptor(),
            static_cast<off_t>(m_map_span)
        );

        // Check for errors in acquiring memory mapping.
        // todo decide on error handling
        if (m_virtual_base == MAP_FAILED) {
            throw MemoryMappingError("failed to acquire memory mapping");
        }
    }

    ~MemoryMapping() noexcept
    {
        if (m_virtual_base != MAP_FAILED) {

            // Deallocate the mapped memory.
            if (raw_posix::munmap(m_virtual_base, m_map_span) != 0) {
                // Destructor cannot fail; terminate program.
                std::terminate();
            }
            // Set base pointer to nullptr to reduce chance of accidental misuse.
            m_virtual_base = MAP_FAILED;
        }
    }

    MemoryMapping(const MemoryMapping&) = delete;

    MemoryMapping(MemoryMapping&& other) noexcept
        : m_virtual_base{std::exchange(other.m_virtual_base, MAP_FAILED)},
          m_map_span{std::exchange(other.m_map_span, 0)} {}

    MemoryMapping& operator=(const MemoryMapping&) = delete;

    MemoryMapping& operator=(MemoryMapping&& other) noexcept
    {
        m_virtual_base = std::exchange(other.m_virtual_base, MAP_FAILED);
        m_map_span = std::exchange(other.m_map_span, 0);
        return *this;
    }

    // Boolean conversion operator.
    explicit operator bool() {
        return m_virtual_base != MAP_FAILED;
    }

    template<typename T>
    volatile T* access_memory(std::size_t offset)
    {
        // Check that this mapping has a physical destination.
        if (m_virtual_base == MAP_FAILED) {
            throw MemoryMappingError("memory mapping does not exist");
        }

        // Check that the requested memory address is contained in this mapping.
        if (offset > m_map_span) {
            throw MemoryMappingError("memory access outside of mapping span");
        }

        // Cast base pointer to virtual memory mapping to std::byte to allow
        // byte-based pointer arithmetic.
        const auto offset_ptr = static_cast<std::byte*>(m_virtual_base) + offset;

        // Check of the offset pointer is properly aligned [so-unaligned-1].
        if (0 != reinterpret_cast<std::uintptr_t>(offset_ptr) % alignof(T)) {
            throw MemoryMappingError("memory access unaligned");
        }

        return access_memory_unchecked<T>(offset);
    }

    template<typename T>
    volatile T* access_memory_unchecked(std::size_t offset)
    {
        // Cast base pointer to virtual memory mapping to std::byte to allow
        // byte-based pointer arithmetic.
        const auto offset_ptr = static_cast<std::byte*>(m_virtual_base) + offset;

        return reinterpret_cast<volatile T*>(offset_ptr);
        // Note: according to the final draft of the C++17 standard, the result of
        // the reinterpret_cast above is unspecified if register_ptr does not meet
        // the alignment requirements of Register [8.2.1/7,8.2.9/13]. In practice,
        // the cast still "works" with misaligned pointers on the DE1-SoC.
    }

};

// Bitwise-or operator overload for FileFlag enumeration.
inline FileFlag operator|(FileFlag lhs, FileFlag rhs)
{
    using T = std::underlying_type<FileFlag>::type;
    return static_cast<FileFlag>(
        static_cast<T>(lhs) | static_cast<T>(rhs)
    );
}

// Bitwise-or operator overload for MemoryFlag enumeration.
inline MemoryFlag operator|(MemoryFlag lhs, MemoryFlag rhs)
{
    using T = std::underlying_type<MemoryFlag>::type;
    return static_cast<MemoryFlag>(
        static_cast<T>(lhs) | static_cast<T>(rhs)
    );
}

}

#endif //ECEE_2160_LAB_REPORTS_POSIX_API_H
