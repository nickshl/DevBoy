//******************************************************************************
//  @file String.h
//  @author Nicolai Shlapunov
//
//  @details DevCore: String Visual Object Class, header
//
//  @section LICENSE
//
//   Software License Agreement (BSD License)
//
//   Copyright (c) 2016, Devtronic & Nicolai Shlapunov
//   All rights reserved.
//
//   Redistribution and use in source and binary forms, with or without
//   modification, are permitted provided that the following conditions are met:
//   1. Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//   2. Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//   3. Neither the name of the Devtronic nor the names of its contributors
//      may be used to endorse or promote products derived from this software
//      without specific prior written permission.
//
//   THIS SOFTWARE IS PROVIDED BY DEVTRONIC ''AS IS'' AND ANY EXPRESS OR IMPLIED
//   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
//   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
//   IN NO EVENT SHALL DEVTRONIC BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
//   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//******************************************************************************

#ifndef Strings_h
#define Strings_h

// *****************************************************************************
// ***   Includes   ************************************************************
// *****************************************************************************
#include "DevCfg.h"
#include "VisObject.h"

// *****************************************************************************
// ***   String Class   ********************************************************
// *****************************************************************************
class String : public VisObject
{
  public:
    // *************************************************************************
    // ***   Enum with all fonts types   ***************************************
    // *************************************************************************
    typedef enum 
    {
      FONT_4x6,
      FONT_6x8,
      FONT_8x8,
      FONT_8x12,
      FONT_12x16,
      FONTS_MAX
    } FontType;

    // *************************************************************************
    // ***   Constructor   *****************************************************
    // *************************************************************************
    String() {};
 
    // *************************************************************************
    // ***   Constructor   *****************************************************
    // *************************************************************************
    String(const char* str, int32_t x, int32_t y, uint32_t tc, FontType ft = FONT_8x8);

    // *************************************************************************
    // ***   Constructor   *****************************************************
    // *************************************************************************
    String(const char* str, int32_t x, int32_t y, uint32_t tc, uint32_t bgc, FontType ft = FONT_8x8);

    // *************************************************************************
    // ***   SetParams   *******************************************************
    // *************************************************************************
    void SetParams(const char* str, int32_t x, int32_t y, uint32_t tc, FontType ft);

    // *************************************************************************
    // ***   SetParams   *******************************************************
    // *************************************************************************
    void SetParams(const char* str, int32_t x, int32_t y, uint32_t tc, uint32_t bgc, FontType ft);

    // *************************************************************************
    // ***   SetString   *******************************************************
    // *************************************************************************
    void SetString(const char* str);

    // *************************************************************************
    // ***   SetColor   ********************************************************
    // *************************************************************************
    void SetColor(uint32_t tc, uint32_t bgc = 0U, bool is_trnsp = true);

    // *************************************************************************
    // ***   Put line in buffer   **********************************************
    // *************************************************************************
    virtual void DrawInBufH(uint16_t* buf, int32_t n, int32_t row, int32_t y = 0);
    
    // *************************************************************************
    // ***   Put line in buffer   **********************************************
    // *************************************************************************
    virtual void DrawInBufW(uint16_t* buf, int32_t n, int32_t line, int32_t x = 0);

    // *************************************************************************
    // ***   GetFontW   ********************************************************
    // *************************************************************************
    static uint32_t GetFontW(FontType ft);

    // *************************************************************************
    // ***   GetFontH   ********************************************************
    // *************************************************************************
    static uint32_t GetFontH(FontType ft);

  private:
    // Pointer to string
    // FIX ME: must be changed for prevent changing string during drawing
    const uint8_t* string = nullptr;
    // Text color
    uint16_t txt_color = 0;
    // Background color
    uint16_t bg_color = 0;
    // Font type
    FontType font_type = FONT_8x8;
    // Is background transparent ?
    bool transpatent_bg = false;

    typedef struct
    {
      uint8_t w; // Width of character
      uint8_t h; // Height of character
      uint8_t bytes_per_char; // Bytes Per Char
      const uint8_t* font_data; // Pointer to font data
    } FontProfile;

    // Fonts structures. One for all String classes.
    static const FontProfile fonts[FONTS_MAX];
};

#endif
