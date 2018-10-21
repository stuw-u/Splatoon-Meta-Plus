

/*
 *
 * SquidSquare
 *
 * created with FontCreator
 * written by F. Maximilian Thiele
 *
 * http://www.apetech.de/fontCreator
 * me@apetech.de
 *
 * File Name           : SquidSquare
 * Date                : 26.02.2018
 * Font size in bytes  : 3686--
 * Font width          : 3
 * Font height         : -5
 * Font first char     : 0
 * Font last char      : 128
 * Font used chars     : 128
 *
 * The font data are defined as
 *
 * struct _FONT_ {
 *     uint16_t   font_Size_in_Bytes_over_all_included_Size_it_self;
 *     uint8_t    font_Width_in_Pixel_for_fixed_drawing;
 *     uint8_t    font_Height_in_Pixel_for_all_characters;
 *     unit8_t    font_First_Char;
 *     uint8_t    font_Char_Count;
 *
 *     uint8_t    font_Char_Widths[font_Last_Char - font_First_Char +1];
 *                  // for each character the separate width in pixels,
 *                  // characters < 128 have an implicit virtual right empty row
 *
 *     uint8_t    font_data[];
 *                  // bit field of all characters
 */

#include <Arduino.h>

const uint8_t SquidSquare[] = {
    3,5, //width and height //3,5
    
    // font data
    0x00, 0x00, 0x00, // 0
    0x00, 0x00, 0x00, // 1
    0x00, 0x00, 0x00, // 2
    0x00, 0x00, 0x00, // 3
    0x00, 0x00, 0x00, // 4
    0x00, 0x00, 0x00, // 5
    0x00, 0x00, 0x00, // 6
    0x00, 0x00, 0x00, // 7
    0x00, 0x00, 0x00, // 8
    0x00, 0x00, 0x00, // 9
    0x00, 0x00, 0x00, // 10
    0x00, 0x00, 0x00, // 11
    0x00, 0x00, 0x00, // 12
    0x00, 0x00, 0x00, // 13
    0x00, 0x00, 0x00, // 14
    0x00, 0x00, 0x00, // 15
    0x00, 0x00, 0x00, // 16
    0x00, 0x00, 0x00, // 17
    0x00, 0x00, 0x00, // 18
    0x00, 0x00, 0x00, // 19
    0x00, 0x00, 0x00, // 20
    0x00, 0x00, 0x00, // 21
    0x00, 0x00, 0x00, // 22
    0x00, 0x00, 0x00, // 23
    0x00, 0x00, 0x00, // 24
    0x00, 0x00, 0x00, // 25
    0x00, 0x00, 0x00, // 26
    0x00, 0x00, 0x00, // 27
    0x00, 0x00, 0x00, // 28
    0x00, 0x00, 0x00, // 29
    0x00, 0x00, 0x00, // 30
    0x00, 0x00, 0x00, // 31
    0x00, 0x00, 0x00, // 32
    0x00, 0x17, 0x00, // 33
    0x03, 0x00, 0x03, // 34
    0x1F, 0x0A, 0x1F, // 35
    0x06, 0x1F, 0x0A, // 36
    0x19, 0x04, 0x13, // 37
    0x1A, 0x15, 0x12, // 38
    0x00, 0x03, 0x00, // 39
    0x0E, 0x11, 0x00, // 40
    0x00, 0x11, 0x0E, // 41
    0x01, 0x02, 0x01, // 42
    0x04, 0x0E, 0x04, // 43
    0x18, 0x00, 0x00, // 44
    0x10, 0x10, 0x10, // 45
    0x10, 0x00, 0x00, // 46
    0x18, 0x04, 0x03, // 47
    0x1F, 0x11, 0x1F, // 48
    0x02, 0x1F, 0x00, // 49
    0x1D, 0x15, 0x17, // 50
    0x15, 0x15, 0x1F, // 51
    0x07, 0x04, 0x1F, // 52
    0x17, 0x15, 0x1D, // 53
    0x1F, 0x15, 0x1D, // 54
    0x01, 0x1D, 0x03, // 55
    0x1F, 0x15, 0x1F, // 56
    0x17, 0x15, 0x1F, // 57
    0x00, 0x12, 0x00, // 58
    0x00, 0x1A, 0x00, // 59
    0x04, 0x0A, 0x0A, // 60
    0x0A, 0x0A, 0x0A, // 61
    0x0A, 0x0A, 0x04, // 62
    0x01, 0x15, 0x03, // 63
    0x0D, 0x11, 0x0F, // 64
    0x1F, 0x05, 0x1F, // 65
    0x1F, 0x15, 0x1B, // 66
    0x1F, 0x11, 0x11, // 67
    0x1F, 0x11, 0x0E, // 68
    0x1F, 0x15, 0x15, // 69
    0x1F, 0x05, 0x05, // 70
    0x1F, 0x11, 0x1D, // 71
    0x1F, 0x04, 0x1F, // 72
    0x11, 0x1F, 0x11, // 73
    0x08, 0x11, 0x0F, // 74
    0x1F, 0x04, 0x1B, // 75
    0x1F, 0x10, 0x10, // 76
    0x1F, 0x06, 0x1F, // 77
    0x1F, 0x01, 0x1F, // 78
    0x1F, 0x11, 0x1F, // 79
    0x1F, 0x05, 0x07, // 80
    B00001111, B00001001, B00010111, // 81
    0x1F, 0x05, 0x1B, // 82
    0x17, 0x15, 0x1D, // 83
    0x01, 0x1F, 0x01, // 84
    0x1F, 0x10, 0x1F, // 85
    0x0F, 0x10, 0x0F, // 86
    0x1F, 0x0C, 0x1F, // 87
    0x1B, 0x04, 0x1B, // 88
    0x03, 0x1C, 0x03, // 89
    0x19, 0x15, 0x13, // 90
    0x1F, 0x11, 0x00, // 91
    0x03, 0x04, 0x18, // 92
    0x00, 0x11, 0x1F, // 93
    0x02, 0x01, 0x02, // 94
    0x10, 0x10, 0x10, // 95
    0x01, 0x02, 0x00, // 96
    0x00, 0x00, 0x00, // 97
    0x00, 0x00, 0x00, // 98
    0x00, 0x00, 0x00, // 99
    0x00, 0x00, 0x00, // 100
    0x00, 0x00, 0x00, // 101
    0x00, 0x00, 0x00, // 102
    0x00, 0x00, 0x00, // 103
    0x00, 0x00, 0x00, // 104
    0x00, 0x00, 0x00, // 105
    0x00, 0x00, 0x00, // 106
    0x00, 0x00, 0x00, // 107
    0x00, 0x00, 0x00, // 108
    0x00, 0x00, 0x00, // 109
    0x00, 0x00, 0x00, // 110
    0x00, 0x00, 0x00, // 111
    0x00, 0x00, 0x00, // 112
    0x00, 0x00, 0x00, // 113
    0x00, 0x00, 0x00, // 114
    0x00, 0x00, 0x00, // 115
    0x00, 0x00, 0x00, // 116
    0x00, 0x00, 0x00, // 117
    0x00, 0x00, 0x00, // 118
    0x00, 0x00, 0x00, // 119
    0x00, 0x00, 0x00, // 120
    0x00, 0x00, 0x00, // 121
    0x00, 0x00, 0x00, // 122
    0x04, 0x1B, 0x11, // 123
    0x00, 0x1F, 0x00, // 124
    0x11, 0x1B, 0x04, // 125
    0x04, 0x04, 0x04, // 126
    0x00, 0x00, 0x00 // 127

    
};
