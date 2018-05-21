//******************************************************************************
//  @file ILI9341.h
//  @author Nicolai Shlapunov
//
//  @details DevCore: ILI9341 Low Level Driver Class, header
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

#ifndef ILI9341_h
#define ILI9341_h

// *****************************************************************************
// ***   Includes   ************************************************************
// *****************************************************************************
#include <DevCfg.h>

// *****************************************************************************
// ***   Enums   ***************************************************************
// *****************************************************************************

// Color definitions
enum Color
{
  COLOR_BLACK           = 0x0000, //   0,   0,   0
  COLOR_VERYDARKGREY    = 0xEF7B, //  32,  32,  32
  COLOR_DARKGREY        = 0xEF7B, //  64,  64,  64
  COLOR_GREY            = 0xEF7B, // 128, 128, 128
  COLOR_LIGHTGREY       = 0x18C6, // 192, 192, 192
  COLOR_WHITE           = 0xFFFF, // 255, 255, 255

  COLOR_VERYDARKRED     = 0x0018, //  32,   0,   0
  COLOR_DARKRED         = 0x0038, //  64,   0,   0
  COLOR_MEDIUMRED       = 0x0078, // 128,   0,   0
  COLOR_LIGHTRED        = 0x00B8, // 192,   0,   0
  COLOR_RED             = 0x00F8, // 255,   0,   0

  COLOR_VERYDARKGREEN   = 0xE000, //   0,  32,   0
  COLOR_DARKGREEN       = 0xE001, //   0,  64,   0
  COLOR_MEDIUMGREEN     = 0xE003, //   0, 128,   0
  COLOR_LIGHTGREEN      = 0xE005, //   0, 192,   0
  COLOR_GREEN           = 0xE007, //   0, 255,   0

  COLOR_VERYDARKBLUE    = 0x0300, //   0,   0,  32
  COLOR_DARKBLUE        = 0x0700, //   0,   0,  64
  COLOR_MEDIUMBLUE      = 0x0F00, //   0,   0, 128
  COLOR_LIGHTBLUE       = 0x1700, //   0,   0, 192
  COLOR_BLUE            = 0x1F00, //   0,   0, 255

  COLOR_VERYDARKYELLOW  = 0xE018, //  32,  32,   0
  COLOR_DARKYELLOW      = 0xE039, //  64,  64,   0
  COLOR_MEDIUMYELLOW    = 0xE07B, // 128, 128,   0
  COLOR_LIGHTYELLOW     = 0xE0BD, // 192, 192,   0
  COLOR_YELLOW          = 0xE0FF, // 255, 255,   0

  COLOR_VERYDARKCYAN    = 0xE300, //   0,  32,  32
  COLOR_DARKCYAN        = 0xE701, //   0,  64,  64
  COLOR_MEDIUMCYAN      = 0xEF03, //   0, 128, 128
  COLOR_LIGHTCYAN       = 0xF705, //   0, 192, 192
  COLOR_CYAN            = 0xFF07, //   0, 255, 255

  COLOR_VERYDARKMAGENTA = 0x0318, //  32,   0,  32
  COLOR_DARKMAGENTA     = 0x0738, //  64,   0,  64
  COLOR_MEDIUMMAGENTA   = 0x0F78, // 128,   0, 128
  COLOR_LIGHTMAGENTA    = 0x17B8, // 192,   0, 192
  COLOR_MAGENTA         = 0x1FF8, // 255,   0, 255
};

class ILI9341
{
  public:
    // *************************************************************************
    // ***   Constructor   *****************************************************
    // *************************************************************************
    ILI9341(SPI_HandleTypeDef* in_hspi);

    // *************************************************************************
    // ***   Write byte to SPI   ***********************************************
    // *************************************************************************
    inline void SpiWrite(uint8_t c);

    // *************************************************************************
    // ***   Write byte stream to SPI   ****************************************
    // *************************************************************************
    void SpiWriteStream(uint8_t* data, uint32_t n);

    // *************************************************************************
    // ***   Write command to SPI   ********************************************
    // *************************************************************************
    inline void WriteCommand(uint8_t c);

    // *************************************************************************
    // ***   Write data to SPI   ***********************************************
    // *************************************************************************
    inline void WriteData(uint8_t c);

    // *************************************************************************
    // ***   Write data steram to SPI   ****************************************
    // *************************************************************************
    void WriteDataStream(uint8_t* data, uint32_t n);

    // *************************************************************************
    // ***   Check SPI transfer status  ****************************************
    // *************************************************************************
    bool IsTransferComplete(void);

    // *************************************************************************
    // ***   Pull up CS line for LCD  ******************************************
    // *************************************************************************
    void StopTransfer(void);

    // *************************************************************************
    // ***   Init screen   *****************************************************
    // *************************************************************************
    void Init(void);

    // *************************************************************************
    // ***   Set output window   ***********************************************
    // *************************************************************************
    void SetAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

    // *************************************************************************
    // ***   Pass 8-bit (each) R,G,B, get back 16-bit packed color   ***********
    // *************************************************************************
    uint16_t GetColor565(uint8_t r, uint8_t g, uint8_t b);

    // *************************************************************************
    // ***   Set screen orientation   ******************************************
    // *************************************************************************
    void SetRotation(uint8_t r);

    // *************************************************************************
    // ***   Write color to screen   *******************************************
    // *************************************************************************
    void PushColor(uint16_t color);

    // *************************************************************************
    // ***   Draw one pixel on  screen   ***************************************
    // *************************************************************************
    void DrawPixel(int16_t x, int16_t y, uint16_t color);

    // *************************************************************************
    // ***   Draw vertical line   **********************************************
    // *************************************************************************
    void DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);

    // *************************************************************************
    // ***   Draw horizontal line   ********************************************
    // *************************************************************************
    void DrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

    // *************************************************************************
    // ***   Fill full screen   ************************************************
    // *************************************************************************
    void FillScreen(uint16_t color);

    // *************************************************************************
    // ***   Fill rectangle on screen   ****************************************
    // *************************************************************************
    void FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

    // *************************************************************************
    // ***   Invert display   **************************************************
    // *************************************************************************
    void InvertDisplay(bool invert);

    // *************************************************************************
    // ***   Read data from SPI   **********************************************
    // *************************************************************************
    inline uint8_t SpiRead(void);

    // *************************************************************************
    // ***   Read data from display   ******************************************
    // *************************************************************************
    inline uint8_t ReadData(void);

    // *************************************************************************
    // ***   Send read command ad read result   ********************************
    // *************************************************************************
    uint8_t ReadCommand(uint8_t c);

    // *************************************************************************
    // ***   Return screen width   *********************************************
    // *************************************************************************
    inline int32_t GetWidth(void) {return width;}

    // *************************************************************************
    // ***   Return screen height   ********************************************
    // *************************************************************************
    inline int32_t GetHeight(void) {return height;}

    // *************************************************************************
    // ***   Return byte(s) per pixel   ****************************************
    // *************************************************************************
    inline int32_t GetBytesPerPixel(void) {return byte_per_pixel;}

    // *************************************************************************
    // ***   Return max line   *************************************************
    // *************************************************************************
    static constexpr int32_t GetMaxLine(void) {return TFT_WIDTH > TFT_HEIGHT ? TFT_WIDTH : TFT_HEIGHT;}

    // *************************************************************************
    // ***   Return max line   *************************************************
    // *************************************************************************
    static constexpr int32_t GetMaxBpp(void) {return TFT_BPP;}

  private:

    // Display width
    static const int32_t TFT_WIDTH = 320;
    // Display height
    static const int32_t TFT_HEIGHT = 240;
    // Display byte per pixel
    static const int32_t TFT_BPP = 2;

    // Handle to screen SPI
    SPI_HandleTypeDef* hspi = nullptr;

    // Width
    int32_t width = TFT_WIDTH;
    // Height
    int32_t height = TFT_HEIGHT;
    // Byte(s) per pixel
    int32_t byte_per_pixel = TFT_BPP;

    // Rotation
    uint32_t rotation = 0U;
};

#endif
