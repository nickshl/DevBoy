//******************************************************************************
//  @file Primitives.h
//  @author Nicolai Shlapunov
//
//  @details DevCore: Primitives Visual Object Classes(Box, Line, Circle), header
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

#ifndef Primitives_h
#define Primitives_h

// *****************************************************************************
// ***   Includes   ************************************************************
// *****************************************************************************
#include "DevCfg.h"
#include "VisObject.h"

// *****************************************************************************
// ***   Box Class   ***********************************************************
// *****************************************************************************
class Box : public VisObject
{
  public:
    // *************************************************************************
    // ***   Constructor   *****************************************************
    // *************************************************************************
    Box() {};

    // *************************************************************************
    // ***   Constructor   *****************************************************
    // *************************************************************************
    Box(int32_t x, int32_t y, int32_t w, int32_t h, int32_t c, bool is_fill = false);

    // *************************************************************************
    // ***   SetParams   *******************************************************
    // *************************************************************************
    void SetParams(int32_t x, int32_t y, int32_t w, int32_t h, int32_t c, bool is_fill = false);

    // *************************************************************************
    // ***   Put line in buffer   **********************************************
    // *************************************************************************
    virtual void DrawInBufH(uint16_t* buf, int32_t n, int32_t row, int32_t y = 0);
    
    // *************************************************************************
    // ***   Put line in buffer   **********************************************
    // *************************************************************************
    virtual void DrawInBufW(uint16_t* buf, int32_t n, int32_t line, int32_t x = 0);
    
  private:
    // Box color
    uint16_t color = 0U;
    // Is box fill ?
    bool fill = false;
};

// *****************************************************************************
// ***   Line Class   **********************************************************
// *****************************************************************************
class Line : public VisObject
{
  public:
    // *************************************************************************
    // ***   Constructor   *****************************************************
    // *************************************************************************
    Line() {};

    // *************************************************************************
    // ***   Constructor   *****************************************************
    // *************************************************************************
    Line(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t c);

    // *************************************************************************
    // ***   SetParams   *******************************************************
    // *************************************************************************
    void SetParams(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t c);

    // *************************************************************************
    // ***   Put line in buffer   **********************************************
    // *************************************************************************
    virtual void DrawInBufH(uint16_t* buf, int32_t n, int32_t row, int32_t y = 0);
    
    // *************************************************************************
    // ***   Put line in buffer   **********************************************
    // *************************************************************************
    virtual void DrawInBufW(uint16_t* buf, int32_t n, int32_t line, int32_t x = 0);
    
  private:
    // Line color
    uint16_t color = 0U;
};

// *****************************************************************************
// ***   Circle Class   ********************************************************
// *****************************************************************************
class Circle : public VisObject
{
  public:
    // *************************************************************************
    // ***   Constructor   *****************************************************
    // *************************************************************************
    Circle() {};

    // *************************************************************************
    // ***   Constructor   *****************************************************
    // *************************************************************************
    Circle(int32_t x, int32_t y, int32_t r, int32_t c, bool is_fill = false);

    // *************************************************************************
    // ***   SetParams   **************************************II***************
    // *************************************************************************
    void SetParams(int32_t x, int32_t y, int32_t r, int32_t c, bool is_fill = false);

    // *************************************************************************
    // ***   Put line in buffer   **********************************************
    // *************************************************************************
    virtual void DrawInBufH(uint16_t* buf, int32_t n, int32_t row, int32_t y = 0);

    // *************************************************************************
    // ***   Put line in buffer   **********************************************
    // *************************************************************************
    virtual void DrawInBufW(uint16_t* buf, int32_t n, int32_t line, int32_t x = 0);

  private:
    // Circle color
    uint16_t color = 0U;
    // Circle radius
    int16_t radius = 0;
    // Is box fill ?
    bool fill = false;
};

#endif
