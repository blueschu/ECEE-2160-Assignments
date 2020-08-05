/**
 * ECEE 2160 Lab Assignment 4 - POSIX API utilities.
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
 * [so-unaligned-2] https://stackoverflow.com/questions/32062894/take-advantage-of-arm-unaligned-memory-access-while-writing-clean-c-code
 * [cpp-underlying] https://en.cppreference.com/w/cpp/types/underlying_type
 * [isocpp-guidelines] https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
 *
 */

#ifndef ECEE_2160_LAB_REPORTS_POSIX_API_H
#define ECEE_2160_LAB_REPORTS_POSIX_API_H

// When defined, debug info will be printed to standard error if an error
// is encountered.
//#define POSIX_API_PRINT_DEBUG

#include <cstddef>          // for std::byte
#include <stdexcept>        // for std::runtime_error
#include <type_traits>      // for std::underlying_type
#include <utility>          // for std::exchange

#ifdef POSIX_API_PRINT_DEBUG
#include <iostream>
#include <cerrno>
#include <cstring>
#endif

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
     * A file descriptor acquired from POSIX api.
     */
    Descriptor m_fd;

  public:

    /**
     * Constructs a `File` holding a POSIX file descriptor for the given file.
     *
     * @param file_name C-string file name.
     * @param flags POSIX file opening flags.
     */
    File(const char* file_name, FileFlag flags)
    {
        using Flag = std::underlying_type<FileFlag>::type;
        m_fd = raw_posix::open(file_name, static_cast<Flag>(flags));
    }

    // Destructor. Marked noexcept per C.37 [isocpp-guidelines].
    ~File() noexcept
    {
        if (m_fd != k_failed) {
            // Release the file descriptor.
            raw_posix::close(m_fd);
            // Set file descriptor to the failed sentinel.
            m_fd = k_failed;
        }
    }

    // Prevent copying from lvalue, C.21, C.81. [isocpp-guidelines].
    File(const File&) = delete;

    // Move constructor, C.21.
    File(File&& other) noexcept
        : m_fd{std::exchange(other.m_fd, k_failed)} {}

    // Prevent copying from lvalue, C.21, C.81. [isocpp-guidelines].
    File& operator=(const File&) = delete;

    // Move assignment, C.21.
    File& operator=(File&& other) noexcept
    {
        m_fd = std::exchange(other.m_fd, k_failed);
        return *this;
    }

    /**
     * Returns this File's posix file descriptor.
     */
    Descriptor raw_descriptor() const
    {
        return m_fd;
    }

    // Boolean conversion operator.
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

/**
 * A mapping between this process' virtual address space and the contents
 * of some file.
 *
 * Note: this class retains a system file descriptor for the duration of its
 * life. To minimize system resource use, the lifespan of MemoryMapping should
 * be as short as possible.
 */
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
     * Creates a new memory mapping into the given file.
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
     * This constructor always uses MAP_SHARED for the memory sharing flag.
     *
     * If this constructor fails to acquire a memory mapping, using operator
     * bool() on this object will return `false`. If the macro
     * POSIX_API_PRINT_DEBUG is defined, additional debug info will be printed
     * to stderr on memory mapping failure.
     *
     * @param fd POSIX file to be mapped.
     * @param bridge_span Mapping span.
     * @param protection_flags POSIX memory protection flags
     * @param bridge_base Starting address for the memory mapping.
     */
    MemoryMapping(
        const File& fd,
        std::size_t bridge_span,
        MemoryFlag protection_flags,
        std::uintptr_t bridge_base
    ) : m_map_span{bridge_span}
    {
        using Flag = std::underlying_type<MemoryFlag>::type;

        // Get a mapping from physical addresses to virtual addresses.
        m_virtual_base = raw_posix::mmap(
            // Let the kernel choose the virtual address.
            nullptr,
            bridge_span,
            static_cast<Flag>(protection_flags),
            MAP_SHARED,
            fd.raw_descriptor(),
            static_cast<off_t>(bridge_base)
        );
    }

    // Destructor. Marked noexcept per C.37 [isocpp-guidelines].
    ~MemoryMapping() noexcept
    {
        if (m_virtual_base != MAP_FAILED) {

            // Deallocate the mapped memory.
            if (raw_posix::munmap(m_virtual_base, m_map_span) != 0) {
#ifdef POSIX_API_PRINT_DEBUG
                std::cerr << "Failed to deallocate memory mapping: " << std::strerror(errno) << '\n';
#endif
                // Destructor cannot fail; terminate program.
                std::terminate();
            }
            // Set base pointer to nullptr to reduce chance of accidental misuse.
            m_virtual_base = MAP_FAILED;
        }
    }

    // Prevent copying from lvalue, C.21, C.81. [isocpp-guidelines].
    MemoryMapping(const MemoryMapping&) = delete;

    // Move constructor, C.21 [isocpp-guidelines].
    MemoryMapping(MemoryMapping&& other) noexcept
        : m_virtual_base{std::exchange(other.m_virtual_base, MAP_FAILED)},
          m_map_span{std::exchange(other.m_map_span, 0)} {}

    // Prevent copying from lvalue, C.21, C.81. [isocpp-guidelines].
    MemoryMapping& operator=(const MemoryMapping&) = delete;

    // Move assignment, C.21 [isocpp-guidelines].
    MemoryMapping& operator=(MemoryMapping&& other) noexcept
    {
        m_virtual_base = std::exchange(other.m_virtual_base, MAP_FAILED);
        m_map_span = std::exchange(other.m_map_span, 0);
        return *this;
    }

    /**
     * Returns `true` if this object holds a valid memory mapping into a file.
     */
    explicit operator bool()
    {
        return m_virtual_base != MAP_FAILED;
    }

    /**
     * Returns a pointer to the virtual address corresponding to the physical
     * memory location that is offset from the mapping base by the given offset.
     *
     *
     * @tparam T Integral type uses to represent the value at the target location.
     * @param offset Offset from memory mapping base of target location.
     * @return Virtual address pointer to corresponding to the target.
     * @throws MemoryMappingError if the memory mapping does not exist, if the
     *          target location falls outside of the memory mapping, or if
     *          the target location is not aligned on a `T` boundary.
     */
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

        // Check if the offset pointer is properly aligned [so-unaligned-1].
        if (0 != reinterpret_cast<std::uintptr_t>(offset_ptr) % alignof(T)) {
            throw MemoryMappingError("memory access unaligned");
        }

        return access_memory_unchecked<T>(offset);
    }

    /**
     * Returns a pointer to the virtual address corresponding to the physical
     * memory location that is offset from the mapping base by the given offset,
     * without checking the validity of the access.
     *
     * @tparam T Integral type uses to represent the value at the target location.
     * @param offset Offset from memory mapping base of target location.
     * @return Virtual address pointer to corresponding to the target.
     */
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
