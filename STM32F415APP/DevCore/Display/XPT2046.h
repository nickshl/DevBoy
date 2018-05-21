//******************************************************************************
//  @file XPT2046.h
//  @author Nicolai Shlapunov
//
//  @details DevCore: XPT2046 Low Level Driver Class, header
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

#ifndef XPT2046_h
#define XPT2046_h

// *****************************************************************************
// ***   Includes   ************************************************************
// *****************************************************************************
#include "DevCfg.h"

// *****************************************************************************
// ***   Defines   *************************************************************
// *****************************************************************************

// *****************************************************************************
// * XPT2046 class. Implements work with XPT2046 resistive touchscreen.
class XPT2046
{
  public:
    // Coefficient for calibration
    const static int32_t COEF = 100;

    // *************************************************************************
    // ***   Constructor   *****************************************************
    // *************************************************************************
    // * This class hasn't task inside. For use this class user must provide
    // * handle to SPI. Previously I think shape SPI between TFT and Touch.
    // * But now it is two different SPI. Anyway main idea it is use this class
    // * in DisplayDrv class for find pressed VisObjects.
    XPT2046(SPI_HandleTypeDef* in_hspi) : hspi(in_hspi) {};

    // *************************************************************************
    // ***   Init   ************************************************************
    // *************************************************************************
    // * Init function. Send init sequence to touchscreen controller.
    void Init(void);

    // *************************************************************************
    // ***   IsTouch   *********************************************************
    // *************************************************************************
    // * Check touched or not by T_IRQ pin. Return true if touched.
    bool IsTouch(void);

    // *************************************************************************
    // ***   GetRawXY   ********************************************************
    // *************************************************************************
    // * Return raw X and Y coordinates. If touched - return true.
    bool GetRawXY(int32_t& x, int32_t& y);

    // *************************************************************************
    // ***   GetXY   ***********************************************************
    // *************************************************************************
    // * Return recalculated using calibration constants X and Y coordinates.
    // * If touched - return true. Can be used for second calibration.
    bool GetXY(int32_t& x, int32_t& y);

    // *************************************************************************
    // ***   SetCalibrationConsts   ********************************************
    // *************************************************************************
    // * Set calibration constants. Must be call for calibration touchscreen.
    void SetCalibrationConsts(int32_t nkx, int32_t nky, int32_t nbx, int32_t nby);

  private:
    // Turn touchscreen ON
    const static uint8_t TON = 0x80;
    // Empty byte
    const static uint8_t EMP = 0x00;
    // Request X coordinate
    const static uint8_t CHX = 0x90;
    // Request Y coordinate
    const static uint8_t CHY = 0xD0;

    // Handle to SPI used for touchscreen
    SPI_HandleTypeDef* hspi = nullptr;

    // Display width and height offset
    int32_t kx = -1097, ky = -1499;
    // Display width and height coefficient
    int32_t bx = 334, by = 259;
    
    // *************************************************************************
    // ***   SpiWrite   ********************************************************
    // *************************************************************************
    // * Write byte to SPI
    inline void SpiWrite(uint8_t c);

    // *************************************************************************
    // ***   SpiWriteRead   ****************************************************
    // *************************************************************************
    // * Write/read byte from/to SPI
    inline uint8_t SpiWriteRead(uint8_t c);
};

#endif
