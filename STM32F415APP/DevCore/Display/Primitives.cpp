//******************************************************************************
//  @file Primitives.cpp
//  @author Nicolai Shlapunov
//
//  @details DevCore: Primitives Visual Object Classes(Box, Line, Circle), implementation
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
#include "Primitives.h"

#include <cstdlib> // for abs()

// *****************************************************************************
// *****************************************************************************
// ***   Box   *****************************************************************
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// ***   Constructor   *********************************************************
// *****************************************************************************
Box::Box(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t c, bool is_fill)
{
  SetParams(x, y, w, h, c, is_fill);
}

// *****************************************************************************
// ***   SetParams   ***********************************************************
// *****************************************************************************
void Box::SetParams(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t c, bool is_fill)
{
  color = c;
  x_start = x;
  y_start = y;
  x_end = x + w - 1;
  y_end = y + h - 1;
  width = w;
  height = h;
  rotation = 0;
  fill = is_fill;
}

// *****************************************************************************
// ***   Put line in buffer   **************************************************
// *****************************************************************************
void Box::DrawInBufW(uint16_t* buf, int32_t n, int32_t line, int32_t start_x)
{
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
    if(end >= n) end = n - 1;
    // Have sense draw only if end pointer in buffer
    if(end > 0)
    {
      // If fill or first/last line - must be solid
      if(fill || line == y_start || line == y_end)
      {
        for(int32_t i = start; i <= end; i++) buf[i] = color;
      }
      else
      {
        if(x_start - start_x >= 0) buf[start] = color;
        if(x_end   - start_x <  n) buf[end]   = color;
      }
    }
  }
}

// *****************************************************************************
// ***   Put line in buffer   **************************************************
// *****************************************************************************
void Box::DrawInBufH(uint16_t* buf, int32_t n, int32_t row, int32_t start_y)
{
  // Draw only if needed
  if((row >= x_start) && (row <= x_end))
  {
    // Find start x position
    int32_t start = y_start - start_y;
    // Prevent write in memory before buffer
    if(start < 0) start = 0;
    // Find start x position
    int32_t end = y_end - start_y;
    // Prevent buffer overflow
    if(end >= n) end = n - 1;
    // Have sense draw only if end pointer in buffer
    if(end > 0)
    {
      // If fill or first/last row - must be solid
      if(fill || row == x_start || row == x_end)
      {
        for(int32_t i = start; i <= end; i++) buf[i] = color;
      }
      else
      {
        if(y_start - start_y >= 0) buf[start] = color;
        if(y_end   - start_y <  n) buf[end]   = color;
      }
    }
  }
}

// *****************************************************************************
// *****************************************************************************
// ***   Line   ****************************************************************
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// ***   Constructor   *********************************************************
// *****************************************************************************
Line::Line(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t c)
{
  SetParams(x1, y1, x2, y2, c);
}

// *****************************************************************************
// ***   SetParams   ***********************************************************
// *****************************************************************************
void Line::SetParams(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t c)
{
  color = c;
  x_start = x1;
  y_start = y1;
  x_end = x2;
  y_end = y2;
  width  = (x1 < x2) ? (x2 - x1) : (x1 - x2);
  height = (y1 < y2) ? (y2 - y1) : (y1 - y2);
  rotation = 0;
}

// *****************************************************************************
// ***   Put line in buffer   **************************************************
// *****************************************************************************
void Line::DrawInBufW(uint16_t* buf, int32_t n, int32_t line, int32_t start_x)
{
  // Draw only if needed
  if((line >= y_start) && (line <= y_end))
  {
    const int32_t deltaX = abs(x_end - x_start);
    const int32_t deltaY = abs(y_end - y_start);
    const int32_t signX = x_start < x_end ? 1 : -1;
    const int32_t signY = y_start < y_end ? 1 : -1;

    int32_t error = deltaX - deltaY;
    
    int32_t x = x_start - start_x;
    int32_t y = y_start;

    int32_t end_x = x_end - start_x;
    while((x != end_x || y != y_end) && (y != line))
    {
      const int32_t error2 = error * 2;
      if(error2 > -deltaY) 
      {
        error -= deltaY;
        x += signX;
      }
      if(error2 < deltaX) 
      {
        error += deltaX;
        y += signY;
      }
    }

    while((x != end_x || y != y_end) && (y == line))
    {
      if((x >= 0) && (x < n)) buf[x] = color;
      const int32_t error2 = error * 2;
      if(error2 > -deltaY) 
      {
        error -= deltaY;
        x += signX;
      }
      if(error2 < deltaX) 
      {
        break;
      }
    }
  }
}

// *****************************************************************************
// ***   Put line in buffer   **************************************************
// *****************************************************************************
void Line::DrawInBufH(uint16_t* buf, int32_t n, int32_t row, int32_t start_y)
{
  // FIX ME: implement for Vertical Update Mode too
}

// *****************************************************************************
// *****************************************************************************
// ***   Circle   **************************************************************
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// ***   Constructor   *********************************************************
// *****************************************************************************
Circle::Circle(int32_t x, int32_t y, int32_t r, int32_t c, bool is_fill)
{
  SetParams(x, y, r, c, is_fill);
}

// *****************************************************************************
// ***   SetParams   ***********************************************************
// *****************************************************************************
void Circle::SetParams(int32_t x, int32_t y, int32_t r, int32_t c, bool is_fill)
{
  color = c;
  radius = r;
  x_start = x - r;
  y_start = y - r;
  x_end = x + r;
  y_end = y + r;
  width = r*2;
  height = r*2;
  rotation = 0;
  fill = is_fill;
}

// *****************************************************************************
// ***   Put line in buffer   **************************************************
// *****************************************************************************
void Circle::DrawInBufW(uint16_t* buf, int32_t n, int32_t line, int32_t start_x)
{
  // Draw only if needed
  if((line >= y_start) && (line <= y_end))
  {
    int32_t x = 0;
    int32_t y = radius;
    int32_t x0 = x_start + radius - start_x;
    int32_t y0 = y_start + radius;
    int32_t delta = 1 - 2 * radius;
    int32_t error = 0;
    bool line_drawed = false;
    
    while(y >= 0) 
    {
      if( (y0 + y == line) || (y0 - y == line) )
      {
        if(fill)
        {
          int32_t i = x0 - x;
          if(i < 0) i = 0;
          if(x0 + x < n) n = x0 + x;
          for(;i < n; i++)
          {
            buf[i] = color;
          }
          break;
        }
        else
        {
          int32_t xl = x0 - x;
          int32_t xr = x0 + x;
          if((xl > 0) && (xl < n)) buf[xl] = color;
          if((xr > 0) && (xr < n)) buf[xr] = color;
        }
        line_drawed = true;
      }
      else
      {
        if(line_drawed == true) break;
      }
      error = 2 * (delta + y) - 1;
      if(delta < 0 && error <= 0)
      {
        ++x;
        delta += 2 * x + 1;
        continue;
      }
      error = 2 * (delta - x) - 1;
      if(delta > 0 && error > 0)
      {
        --y;
        delta += 1 - 2 * y;
        continue;
      }
      ++x;
      delta += 2 * (x - y);
      --y;
    }
  }
}

// *****************************************************************************
// ***   Put line in buffer   **************************************************
// *****************************************************************************
void Circle::DrawInBufH(uint16_t* buf, int32_t n, int32_t row, int32_t start_y)
{
  if((row >= x_start) && (row <= x_end))
  {
    // Find start x position
    int32_t start = y_start - start_y;
    // Prevent write in memory before buffer
    if(start < 0) start = 0;
    // Find start x position
    int32_t end = y_end - start_y;
    // Prevent buffer overflow
    if(end > n) end = n;
    // Have sense draw only if end pointer in buffer
    if(end > 0)
    {
      for(int32_t i = start; i < end; i++) buf[i] = color;
    }
  }
}
