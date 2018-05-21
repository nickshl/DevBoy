//******************************************************************************
//  @file UiCheckbox.cpp
//  @author Nicolai Shlapunov
//
//  @details DevCore: UI Checkbox Visual Object Class, implementation
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
#include "UiCheckbox.h"

// *****************************************************************************
// ***   Constructor   *********************************************************
// *****************************************************************************
UiCheckbox::UiCheckbox(int32_t x, int32_t y, bool is_checked, bool is_active)
{
  x_start = x;
  y_start = y;
  x_end = x + 13 - 1;
  y_end = y + 13 - 1;
  width = 13;
  height = 13;
  active = is_active;
  checked = is_checked;
}

// *****************************************************************************
// ***   Put line in buffer   **************************************************
// *****************************************************************************
void UiCheckbox::DrawInBufW(uint16_t* buf, int32_t n, int32_t line, int32_t start_x)
{
  uint16_t color;
  // Draw only if needed
  if((line >= y_start) && (line <= y_end))
  {
    // Find start x position
    int32_t start = x_start - start_x;
    // Prevent write in memory before buffer
    if(start < 0) start = 0;
    // Find start x position
    int32_t end = x_end - start_x;
    // Prevent buffer overflow
    if(end > n) end = n;
    if(checked) color = COLOR_YELLOW;
    else           color = COLOR_MAGENTA;
    // Have sense draw only if end pointer in buffer
    if(x_end > 0)
    {
      // If fill or first/last row - must be solid
      if(true || line == y_start || line == y_end)
      {
        for(int32_t i = start; i <= end; i++) buf[i] = color;
      }
    }
  }
}

// *****************************************************************************
// ***   Put line in buffer   **************************************************
// *****************************************************************************
void UiCheckbox::DrawInBufH(uint16_t* buf, int32_t n, int32_t row, int32_t start_y)
{
  // FIX ME: implement for Vertical Update Mode too 
}

// *****************************************************************************
// ***   Action   **************************************************************
// *****************************************************************************
void UiCheckbox::Action(VisObject::ActionType action, int32_t tx, int32_t ty)
{
  // Switch for process action
  switch(action)
  {
    // Touch action
    case VisObject::ACT_TOUCH:
      // Change checked state
      checked = !checked;
      break;
  
    // Untouch action 
    case VisObject::ACT_UNTOUCH:
      break;

    case VisObject::ACT_MAX:
    default:
      break;
  }
}
