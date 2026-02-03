#pragma once

#include <stdint.h>

typedef enum font_char_t {
  FONT_CHAR_SPACE = 0x00,
  FONT_CHAR_DECIMAL = 0x80,
  FONT_CHAR_DASH = 0x40,
  FONT_CHAR_DEGREE = 0x63,
  FONT_CHAR_COLON = 0xFF,
  FONT_CHAR_0 = 0x3F,
  FONT_CHAR_1 = 0x06,
  FONT_CHAR_2 = 0x5B,
  FONT_CHAR_3 = 0x4F,
  FONT_CHAR_4 = 0x66,
  FONT_CHAR_5 = 0x6D,
  FONT_CHAR_6 = 0x7D,
  FONT_CHAR_7 = 0x07,
  FONT_CHAR_8 = 0x7F,
  FONT_CHAR_9 = 0x6F,
  FONT_CHAR_A = 0x77,
  FONT_CHAR_C = 0x39,
  FONT_CHAR_E = 0x79,
  FONT_CHAR_F = 0x71,
  FONT_CHAR_H = 0x76,
  FONT_CHAR_b = 0x7C,
  FONT_CHAR_c = 0x58,
  FONT_CHAR_d = 0x5E,
  FONT_CHAR_h = 0x74,
  FONT_CHAR_n = 0x54,
  FONT_CHAR_r = 0x50,
  FONT_CHAR_o = 0x5C,
  FONT_CHAR_y = 0x6E,
  FONT_CHAR_INVALID = 0x49,
} font_char_t;

font_char_t char_to_font(char c);
