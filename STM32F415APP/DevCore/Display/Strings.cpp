//******************************************************************************
//  @file String.cpp
//  @author Nicolai Shlapunov
//
//  @details DevCore: String Visual Object Class, implementation
//
//  @copyright Copyright (c) 2016, Devtronic & Nicolai Shlapunov
//             All rights reserved.
//
//  @section SUPPORT
//
//   Devtronic invests time and resources providing this open source code,
//   please support Devtronic and open-source hardware/software by
//   donations and/or purchasing products from Devtronic.
//
//******************************************************************************

// *****************************************************************************
// ***   Includes   ************************************************************
// *****************************************************************************
#include "Strings.h"
#include "Fonts.h"

#include <cstring> // for strlen()

// *****************************************************************************
// *****************************************************************************
// ***   Strings   *************************************************************
// *****************************************************************************
// *****************************************************************************
const String::FontProfile String::fonts[FONTS_MAX] =
//  W   H BPP  Pointer to data
{ { 4,  6,  6, (const uint8_t*)font4x6},
  { 6,  8,  8, (const uint8_t*)font6x8},
  { 8,  8,  8, (const uint8_t*)font8x8},
  { 8, 12, 12, (const uint8_t*)font8x12},
  {12, 16, 32, (const uint8_t*)font12x16} };

// *****************************************************************************
// ***   Constructor   *********************************************************
// *****************************************************************************
String::String(const char* str, int32_t x, int32_t y, uint32_t tc, FontType ft)
{
  SetParams(str, x, y, tc, ft);
}

// *****************************************************************************
// ***   Constructor   *********************************************************
// *****************************************************************************
String::String(const char* str, int32_t x, int32_t y, uint32_t tc, uint32_t bgc, FontType ft)
{
  SetParams(str, x, y, tc, bgc, ft);
}

// *****************************************************************************
// ***   SetParams   ***********************************************************
// *****************************************************************************
void String::SetParams(const char* str, int32_t x, int32_t y, uint32_t tc, FontType ft)
{
  string = (const uint8_t*)str;
  x_start = x;
  y_start = y;
  txt_color = tc;
  bg_color = 0;
  font_type = ft;
  transpatent_bg = true;
  width = fonts[ft].w * strlen(str);
  height = fonts[ft].h;
  x_end = x + width - 1;
  y_end = y + height - 1;
  rotation = 0;
}

// *****************************************************************************
// ***   SetParams   ***********************************************************
// *****************************************************************************
void String::SetParams(const char* str, int32_t x, int32_t y, uint32_t tc, uint32_t bgc, FontType ft)
{
  string = (const uint8_t*)str;
  x_start = x;
  y_start = y;
  txt_color = tc;
  bg_color = bgc;
  font_type = ft;
  transpatent_bg = false;
  width = fonts[ft].w * strlen(str);
  height = fonts[ft].h;
  x_end = x + width - 1;
  y_end = y + height - 1;
  rotation = 0;
}

// *****************************************************************************
// ***   SetColor   ************************************************************
// *****************************************************************************
void String::SetColor(uint32_t tc, uint32_t bgc, bool is_trnsp)
{
  txt_color = tc;
  bg_color = bgc;
  transpatent_bg = is_trnsp;
}

// *****************************************************************************
// ***   SetString   ***********************************************************
// *****************************************************************************
void String::SetString(const char* str)
{
  // Lock object for changes
  LockVisObject();
  //Set new pointer to string
  string = (const uint8_t*)str;
  width = fonts[font_type].w * strlen(str);
  x_end = x_start + width - 1;
  // Unlock object after changes
  UnlockVisObject();
}

// *****************************************************************************
// ***   Put line in buffer   **************************************************
// *****************************************************************************
void String::DrawInBufW(uint16_t* buf, int32_t n, int32_t line, int32_t start_x)
{
  // Draw only if needed
  if((line >= y_start) && (line <= y_end) && (string != nullptr))
  {
    // Current symbol X position
    int32_t x = x_start;
    // Number of bytes need skipped for draw line
    uint32_t skip_bytes = (line - y_start) * fonts[font_type].bytes_per_char / fonts[font_type].h;
    // Pointer to string. Will increment for get characters.
    const uint8_t* str = string;

    // While we have symbols
    while(*str != '\0')
    {
      uint32_t b = 0;
      uint32_t w = 0;
      // Get all symbol line
      for(uint32_t i = 0; i < fonts[font_type].bytes_per_char; i++)
      {
        b |= fonts[font_type].font_data[((uint32_t)(*str)) * fonts[font_type].bytes_per_char + skip_bytes + i] << (i*8);
      }
      // Output symbol line
      while(w < fonts[font_type].w)
      {
        // Put color in buffer only if visible
        if((x >= start_x) && (x < start_x+n))
        {
          if((b&1) == 1)
          {
            buf[x] = txt_color;
          }
          else if(transpatent_bg == false)
          {
            buf[x] = bg_color;
          }
          else
          {
            // Empty statement
          }
        }
        b >>= 1;
        w++;
        x++;
      }
      str++;
    }
  }
}

// *****************************************************************************
// ***   Put line in buffer   **************************************************
// *****************************************************************************
void String::DrawInBufH(uint16_t* buf, int32_t n, int32_t row, int32_t start_y)
{
  // Draw only if needed
  if((row >= x_start) && (row <= x_end) && (string != nullptr))
  {
    // Find line in symbol
    int16_t start = y_start - start_y;
    // Find line in symbol
    int16_t line = (row - x_start);
    
    if(line >= 0)
    {
      // Get symbol
      uint8_t c = string[line / fonts[font_type].w];
      // Find line in symbol
      line %= fonts[font_type].w;
      // Index to symbol in data array
      uint16_t s_idx = c * fonts[font_type].bytes_per_char;
      // Index to symbol in data array
      uint16_t bytes_per_line = fonts[font_type].bytes_per_char / fonts[font_type].h;
      // Get symbols lines
      for(int32_t i = 0; i < fonts[font_type].h; i++)
      {
        uint32_t b = *(uint32_t *)(&fonts[font_type].font_data[s_idx + i*bytes_per_line]);
        if(b & (1U<<line))
        {
          if((start+i > 0) && (start+i < n))
          {
            buf[start+i] = txt_color;
          }
        }
      }
    }
  }
}

// *****************************************************************************
// ***   GetFontW   ************************************************************
// *****************************************************************************
uint32_t String::GetFontW(FontType ft)
{
  // Zero my default
  uint32_t font_w = 0U;
  // If provided valid font number
  if(ft < FONTS_MAX)
  {
    // Get font width
    font_w = fonts[ft].w;
  }
  // Return result
  return font_w;
};

// *****************************************************************************
// ***   GetFontH   ************************************************************
// *****************************************************************************
uint32_t String::GetFontH(FontType ft)
{
  // Zero my default
  uint32_t font_h = 0U;
  // If provided valid font number
  if(ft < FONTS_MAX)
  {
    // Get font height
    font_h = fonts[ft].h;
  }
  // Return result
  return font_h;
};
