/**
 * ECEE 2160 Lab Assignment 3 prelab utilities for memory-mapped I/O.
 *
 * Functions were adapted from a set of C-style utility function provided
 * by the instructor. The original version of these functions can be found
 * in `instructor/prelab_fpga_original.cpp`.
 *
 * All function names were specified by the prelab instructions.
 *
 * Author:  Brian Schubert
 * Date:    2020-07-23
 */

#ifndef ECEE_2160_LAB_REPORTS_PRELAB_FPGA_H
#define ECEE_2160_LAB_REPORTS_PRELAB_FPGA_H

#include <cstdint>      // for std::uint32_t, std::size_t
#include <utility>      // for std::pair

/**
 * Integral type for FPGA device registers.
 *
 * Per documentation from the instructor, we can expect all registers
 * to occupy precisely 32 bits on the DE1-SoC.
 */
using Register = std::uint32_t;

/**
 * Pointer type indicating the start of a mapping between physical memory
 * and virtual memory.
 */
using VirtualMappingBase = void*;

/**
 * A file descriptor for interface with fnctl.h.
 */
using FileDescriptor = int;

/// Physical base address of FPGA Devices.
constexpr inline std::size_t LW_BRIDGE_BASE{0xFF'20'00'00};

/// Length of memory-mapped IO window
constexpr inline std::size_t LW_BRIDGE_SPAN{0x00'00'50'00};

/// Base offset to LED registers.
constexpr inline std::size_t LEDR_BASE{0x00'00'00'00};

/// Base offset to switch registers.
constexpr inline std::size_t SW_BASE{0x00'00'00'40};

/// Base offset to push button registers.
constexpr inline std::size_t KEY_BASE{0x00'00'00'50};

/// The number of LEDs on the DE1-SoC board.
constexpr inline std::size_t LED_COUNT{10};

/// The number of switches on the DE1-SOC board.
constexpr inline std::size_t SWITCH_COUNT{10};


/// Value returned by 'open' from fnctl.h upon failing to open a file.
constexpr inline FileDescriptor FNCTL_OPEN_FAILED = -1;

/**
 * Write a 4-byte value from the specified mapped I/O location.
 *
 * This function was based on an example provided by the instructor.
 *
 * @param virtual_base Base of virtual-to-physical memory mapping.
 * @param reg_offset Offset to device to the register relative to the mapping base..
 * @return Register value to be written.
 */
void RegisterWrite(VirtualMappingBase virtual_base, std::size_t reg_offset, Register value);

/**
 * Read a 4-byte value from the specified mapped I/O location.
 *
 * This function was based on an example provided by the instructor.
 *
 * @param virtual_base Base of virtual-to-physical memory mapping.
 * @param reg_offset Offset to device to the register relative to the mapping base..
 * @return Register value read.
 */
Register RegisterRead(VirtualMappingBase virtual_base, std::size_t reg_offset);

/**
 * Initialize general-purpose I/O.
 * - Opens access to physical memory /dev/mem
 * - Maps memory into virtual address space
 *
 * This function immediately terminates upon failure to open /dev/mem or
 * failure to acquire a memory mapping.
 *
 * This function was based on an example provided by the instructor.
 *
 * @return Pair consisting of  1) Address to virtual memory which is mapped
 *         to physical, and 2) fnctl file descriptor for /dev/mem.
 */
std::pair<VirtualMappingBase, FileDescriptor> Initialize();

/**
 * Close general-purpose I/O.
 *
 * This function was based on an example provided by the instructor.
 *
 * @param virtual_base Virtual address where I/O was mapped.
 * @param fd File descriptor previously returned by fntcl's 'open'.
 */
void Finalize(VirtualMappingBase& virtual_base, FileDescriptor& fd);

/*
 * The following two functions were implemented for the prelab assignment.
 */

/**
 * Sets the LED at the given index to the specified state.
 *
 * This function was implemented as part of the prelab assignment.
 *
 * @param virtual_base Virtual address where I/O was mapped.
 * @param led_index Index of the LED.
 * @param state Desired state of the LED (true for on, false for off).
 */
void Write1Led(VirtualMappingBase virtual_base, std::size_t led_index, bool state);

/**
 * Reads the state of the switch at the given index to the specified state.
 *
 * This function was implemented as part of the prelab assignment.
 *
 * @param virtual_base Virtual address where I/O was mapped.
 * @param led_index Index of the LED.
 * @param state Desired state of the LED (true for on, false for off).
 */
bool Read1Switch(VirtualMappingBase virtual_base, std::size_t switch_index);

#endif //ECEE_2160_LAB_REPORTS_PRELAB_FPGA_H
