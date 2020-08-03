//
// Created by brian on 8/1/20.
//

#include "seven_segment_display.h"
#include "led_array.h"
#include "de1soc_properties.h"

int main()
{
    SevenSegmentDisplay<de1soc::seven_segment_display_count> seven_segment_display{
        {de1soc::hex3_hex0_base, de1soc::hex5_hex4_base}
    };

    seven_segment_display.show_number(-18589);
    LedArray<de1soc::led_count> led_array{de1soc::ledr_base};

//    seven_segment_display.write_display_character(2, '1');
}
