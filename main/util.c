//
// Copyright 2026 Miles Richmond
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
//

#include "util.h"
#include <string.h>

font_char_t char_to_font(char c) {
  switch (c) {
  case '\0':
  case ' ':
    return FONT_CHAR_SPACE;
  case '-':
    return FONT_CHAR_DASH;
  case '.':
    return FONT_CHAR_DECIMAL;

  case '0':
    return FONT_CHAR_0;
  case '1':
    return FONT_CHAR_1;
  case '2':
    return FONT_CHAR_2;
  case '3':
    return FONT_CHAR_3;
  case '4':
    return FONT_CHAR_4;
  case '5':
    return FONT_CHAR_5;
  case '6':
    return FONT_CHAR_6;
  case '7':
    return FONT_CHAR_7;
  case '8':
    return FONT_CHAR_8;
  case '9':
    return FONT_CHAR_9;

  case 'A':
  case 'a':
    return FONT_CHAR_A;

  case 'B':
  case 'b':
    return FONT_CHAR_b;

  case 'C':
    return FONT_CHAR_C;
  case 'c':
    return FONT_CHAR_c;

  case 'D':
  case 'd':
    return FONT_CHAR_d;

  case 'e':
  case 'E':
    return FONT_CHAR_E;

  case 'F':
  case 'f':
    return FONT_CHAR_F;

  case 'H':
    return FONT_CHAR_H;
  case 'h':
    return FONT_CHAR_h;

  case 'N':
  case 'n':
    return FONT_CHAR_n;

  case 'R':
  case 'r':
    return FONT_CHAR_r;

  case 'O':
  case 'o':
    return FONT_CHAR_o;

  case 'Y':
  case 'y':
    return FONT_CHAR_y;

  default:
    return FONT_CHAR_INVALID;
  }
}
