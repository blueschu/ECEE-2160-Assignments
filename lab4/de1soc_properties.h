/**
 * ECEE 2160 Lab Assignment 4 DE1-SoC board properties.
 *
 * This header defines several constants for the DE1-SoC board.
 *
 * Author:  Brian Schubert
 * Date:    2020-08-03
 *
 * In contrast with the style used in previous labs, we declare all global
 * constants in lower_snake_case instead of UPPER_SNAKE_CASE to avoid name
 * collisons with macros (which there are quite a few of from the posix headers).
 */

#ifndef ECEE_2160_LAB_REPORTS_DE1SOC_PROPERTIES_H
#define ECEE_2160_LAB_REPORTS_DE1SOC_PROPERTIES_H

/// Namespace for DE1-SoC board properties.
namespace de1soc {

/// Physical base address of FPGA Devices.
constexpr inline std::size_t bridge_base{0xFF'20'00'00};

/// Length of memory-mapped IO window
constexpr inline std::size_t bridge_span{0x00'00'50'00};

/// Base offset to LED registers.
constexpr inline std::size_t ledr_base{0x00'00'00'00};

/// Base offset to switch registers.
constexpr inline std::size_t switch_base{0x00'00'00'40};

/// Base offset to push button registers.
constexpr inline std::size_t key_base{0x00'00'00'50};

/// Base offset to register for seven segment display's 0-3.
constexpr inline std::size_t hex3_hex0_base{0x00'00'00'20};

/// Base offset to register for seven segment display's 4-5.
constexpr inline std::size_t hex5_hex4_base{0x00'00'00'30};

/// The number of LED's on the DE1-SoC board.
constexpr inline std::size_t led_count{10};

/// The number of switches on the DE1-SOC board.
constexpr inline std::size_t switch_count{10};

/// The number of push buttons on the DE1-SOC board.
constexpr inline std::size_t push_button_count{4};

/// The number of seven segment displays on the DE1-SoC board.
constexpr inline std::size_t seven_segment_display_count{6};
}

#endif //ECEE_2160_LAB_REPORTS_DE1SOC_PROPERTIES_H
