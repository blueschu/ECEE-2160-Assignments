//
// Created by brian on 8/1/20.
//

#include "seven_segment_display.h"
#include "led_array.h"
#include "de1soc_properties.h"

int main()
{
    SevenSegmentDisplay seven_segment_display;

    seven_segment_display.show_number(-18589);
    LedArray<de1soc::led_count> led_array;

//    seven_segment_display.write_display_character(2, '1');
}
