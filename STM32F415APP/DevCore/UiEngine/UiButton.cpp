//******************************************************************************
//  @file UiButton.cpp
//  @author Nicolai Shlapunov
//
//  @details DevCore: UI Button Visual Object Class, implementation
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
#include "UiButton.h"

// *****************************************************************************
// *****************************************************************************
// ***   CheckBox   ************************************************************
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// ***   Constructor   *********************************************************
// *****************************************************************************
UiButton::UiButton(const char* str_in, int32_t x, int32_t y, int32_t w, int32_t h,
                   bool is_active)
{
  SetParams(str_in, x, y, w, h, is_active);
}

// *****************************************************************************
// ***   SetParams   ***********************************************************
// *****************************************************************************
void UiButton::SetParams(const char* str_in, int32_t x, int32_t y, int32_t w, int32_t h,
                         bool is_active)
{
  // Clear callback
  callback = nullptr;
  // Save string
  str = str_in;
  // VisObject variables
  x_start = x;
  y_start = y;
  x_end = x + w;
  y_end = y + h;
  width = w;
  height = h;
  active = is_active;
  // Set box params
  box.SetParams(x, y, w, h, COLOR_WHITE);
  // Set string params
  string.SetParams(str, x, y, COLOR_WHITE, String::FONT_8x12);
  string.Move((w-string.GetWidth())/2, (h-string.GetHeight())/2, true);
}

// *************************************************************************
// ***   Set callback function   *******************************************
// *************************************************************************
void UiButton::SetCallback(void (*clbk)(void* ptr, void* param_ptr, uint32_t param),
                           void* clbk_ptr, void* clbk_param_ptr, uint32_t clbk_param)
{
  callback = clbk;
  ptr = clbk_ptr;
  param_ptr = clbk_param_ptr;
  param = clbk_param;
}

// *****************************************************************************
// ***   Put line in buffer   **************************************************
// *****************************************************************************
void UiButton::DrawInBufW(uint16_t* buf, int32_t n, int32_t line, int32_t start_x)
{
  box.DrawInBufW(buf, n, line, start_x);
  string.DrawInBufW(buf, n, line, start_x);
}

// *****************************************************************************
// ***   Put line in buffer   **************************************************
// *****************************************************************************
void UiButton::DrawInBufH(uint16_t* buf, int32_t n, int32_t row, int32_t start_y)
{
  box.DrawInBufH(buf, n, row, start_y);
  string.DrawInBufH(buf, n, row, start_y);
}

// *****************************************************************************
// ***   Action   **************************************************************
// *****************************************************************************
void UiButton::Action(VisObject::ActionType action, int32_t tx, int32_t ty)
{
  // Switch for process action
  switch(action)
  {
    // Touch action
    case VisObject::ACT_TOUCH:  // Fall thru
    case VisObject::ACT_MOVEIN:
      // Set box params
      box.SetParams(x_start, y_start, width, height, COLOR_WHITE, true);
      // Set string params
      string.SetColor(COLOR_BLACK);
      break;

    // Untouch action
    case VisObject::ACT_UNTOUCH:
      // Call callback
      if(callback != nullptr)
      {
        callback(ptr, param_ptr, param);
      }
      // No break here, because other actions the same as Move Out
    case VisObject::ACT_MOVEOUT:
      // Set box params
      box.SetParams(x_start, y_start, width, height, COLOR_WHITE, false);
      // Set string params
      string.SetColor(COLOR_WHITE);
      break;

    case VisObject::ACT_MAX:
    default:
      break;
  }
}
