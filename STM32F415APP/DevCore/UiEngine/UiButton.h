//******************************************************************************
//  @file UiButton.h
//  @author Nicolai Shlapunov
//
//  @details DevCore: UI Button Visual Object Class, header
//
//  @section LICENSE
//
//   Software License Agreement (Modified BSD License)
//
//   Copyright (c) 2016, Devtronic & Nicolai Shlapunov
//   All rights reserved.
//
//   Redistribution and use in source and binary forms, with or without
//   modification, are permitted provided that the following conditions are met:
//
//   1. Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//   2. Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//   3. Neither the name of the Devtronic nor the names of its contributors
//      may be used to endorse or promote products derived from this software
//      without specific prior written permission.
//   4. Redistribution and use of this software other than as permitted under
//      this license is void and will automatically terminate your rights under
//      this license.
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
//  @section SUPPORT
//
//   Devtronic invests time and resources providing this open source code,
//   please support Devtronic and open-source hardware/software by
//   donations and/or purchasing products from Devtronic.
//
//******************************************************************************

#ifndef UiButton_h
#define UiButton_h

// *****************************************************************************
// ***   Includes   ************************************************************
// *****************************************************************************
#include "DevCfg.h"
#include "DisplayDrv.h"
#include "VisObject.h"

// *****************************************************************************
// ***   Button Class   ********************************************************
// *****************************************************************************
class UiButton : public VisObject
{
  public:
    // *************************************************************************
    // ***   Constructor   *****************************************************
    // *************************************************************************
    UiButton() {};

    // *************************************************************************
    // ***   Constructor   *****************************************************
    // *************************************************************************
    UiButton(const char* str_in, int32_t x, int32_t y, int32_t w, int32_t h,
             bool is_active = false);

    // *************************************************************************
    // ***   SetParams   *******************************************************
    // *************************************************************************
    void SetParams(const char* str_in, int32_t x, int32_t y, int32_t w, int32_t h,
                   bool is_active = false);

    // *************************************************************************
    // ***   Set callback function   *******************************************
    // *************************************************************************
    void SetCallback(void (*clbk)(void* ptr, void* param_ptr, uint32_t param),
                     void* clbk_ptr, void* clbk_param_ptr, uint32_t clbk_param);

    // *************************************************************************
    // ***   Put line in buffer   **********************************************
    // *************************************************************************
    virtual void DrawInBufH(uint16_t* buf, int32_t n, int32_t row, int32_t y = 0);

    // *************************************************************************
    // ***   Put line in buffer   **********************************************
    // *************************************************************************
    virtual void DrawInBufW(uint16_t* buf, int32_t n, int32_t line, int32_t x = 0);

    // *************************************************************************
    // ***   Put line in buffer   **********************************************
    // *************************************************************************
    virtual void Action(VisObject::ActionType action, int32_t tx, int32_t ty);

  private:
    // Callback params
    void (*callback)(void* ptr, void* param_ptr, uint32_t param) = nullptr;
    void* ptr = nullptr;
    void* param_ptr = nullptr;
    uint32_t param = 0U;
    // String pointer
    const char* str = nullptr;
    // Box for button
    Box box;
    // String for button
    String string;
};

#endif // UiButton_h
