//******************************************************************************
//  @file DisplayDrv.cpp
//  @author Nicolai Shlapunov
//
//  @details DevCore: Display Driver Class, implementation
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
#include "DisplayDrv.h"

// *****************************************************************************
// ***   Get Instance   ********************************************************
// *****************************************************************************
DisplayDrv& DisplayDrv::GetInstance(void)
{
   static DisplayDrv display_drv;
   return display_drv;
}

// *****************************************************************************
// ***   Display Driver Setup   ************************************************
// *****************************************************************************
Result DisplayDrv::Setup()
{
  // Init display driver
  tft.Init();
  // Set mode - mode can be set earlier than Display initialization
  SetUpdateMode(update_mode);

  // If deisplay and touchscreen share same SPI
  if(tft_hspi == touch_hspi)
  {
     // Set prescaler for SPI
     MODIFY_REG(tft_hspi->Instance->CR1, (uint32_t)SPI_CR1_BR_Msk, SPI_BAUDRATEPRESCALER_64);
     // Init touchscreen driver
     touch.Init();
     // Restore prescaler for SPI
     MODIFY_REG(tft_hspi->Instance->CR1, (uint32_t)SPI_CR1_BR_Msk, SPI_BAUDRATEPRESCALER_2);
  }
  else
  {
    // Init touchscreen driver
    touch.Init();
  }

  // Set string parameters
  fps_str.SetParams(str, width/3, height - 6, COLOR_MAGENTA, String::FONT_4x6);
  // Show string if flag is set
  if(DISPLAY_DEBUG_INFO)
  {
    // Max Z
    fps_str.Show(0xFFFFFFFFU);
  }

  // Always ok
  return Result::RESULT_OK;
}

// *****************************************************************************
// ***   Display Driver Loop   *************************************************
// *****************************************************************************
Result DisplayDrv::Loop()
{
  // Variable for find FPS
  uint32_t time_ms = HAL_GetTick();

  // If semaphore doesn't exist or taken within 100 ms - draw screen
  // Time need for update touchscreen state every 100 ms even display not
  // updated
  if(screen_update.Take(100U) == Result::RESULT_OK)
  {
    // Set window for all screen and pointer to first pixel
    LockDisplay();
    // Set address window for all screen
    tft.SetAddrWindow(0, 0, width-1, height-1);
    // For each line/row
    for(int32_t i=0; i < height; i++)
    {
      // Clear half of buffer
      memset(scr_buf[i%2], 0x00, sizeof(scr_buf[0]));
      // Take semaphore before draw line
      line_mutex.Lock();
      // Set pointer to first element
      VisObject* p_obj = object_list;
      // Do for all objects
      while(p_obj != nullptr)
      {
        // Draw object to buf
        if(update_mode) p_obj->DrawInBufH(scr_buf[i%2], width, i);
        else            p_obj->DrawInBufW(scr_buf[i%2], width, i);
        // Set pointer to next object in list
        p_obj = p_obj->p_next;
      }
      // Give semaphore after changes
      line_mutex.Release();
      // Wait until previous transfer complete
      while(tft.IsTransferComplete() == false) taskYIELD();
      // Write stream to LCD
      tft.SpiWriteStream((uint8_t*)scr_buf[i%2], width*tft.GetBytesPerPixel());
      // DO NOT TRY "OPTIMIZE" CODE !!!
      // Two "while" cycles used for generate next line when previous line
      // transfer via SPI to display.
    }
    // Wait until last transfer complete
    while(tft.IsTransferComplete() == false) taskYIELD();
    // Pull up CS
    tft.StopTransfer();
    // Give semaphore after draw frame
    UnlockDisplay();
    // Calculate FPS if debug info is ON
    if(DISPLAY_DEBUG_INFO)
    {
      // FPS in format XX.X
      fps_x10 = (1000 * 10) / (HAL_GetTick() - time_ms);
    }
  }

  bool tmp_is_touch = false;
  int32_t tmp_tx = tx;
  int32_t tmp_ty = ty;
  // Try to take mutex. 1 ms should be enough.
  if(touchscreen_mutex.Lock(1U) == Result::RESULT_OK)
  {
    // Set prescaler for SPI it display share save SPI with touchscreen
    if(tft_hspi == touch_hspi)
    {
      MODIFY_REG(tft_hspi->Instance->CR1, (uint32_t)SPI_CR1_BR_Msk, SPI_BAUDRATEPRESCALER_64);
    }
    // Get touch coordinates
    tmp_is_touch = touch.GetXY(tmp_tx, tmp_ty);
    // Reset prescaler for SPI it display share save SPI with touchscreen
    if(tft_hspi == touch_hspi)
    {
      // Restore prescaler for SPI
      MODIFY_REG(tft_hspi->Instance->CR1, (uint32_t)SPI_CR1_BR_Msk, SPI_BAUDRATEPRESCALER_2);
    }
    // Give semaphore for drawing frame - we can enter in this "if" statement
    // only if mutex taken
    touchscreen_mutex.Release();
  }
  // If touch state changed (move)
  if(is_touch && tmp_is_touch && ((tx != tmp_tx) || (ty != tmp_ty)) )
  {
    // Go thru VisObject list and call Active() function for active object
    // Take semaphore before draw line
    line_mutex.Lock();
    // Set pointer to first element
    VisObject* p_obj = object_list_last;
    // If list not empty
    if(p_obj != nullptr)
    {
      // Do for all objects
      while(p_obj != nullptr)
      {
        // If we found active object
        if(p_obj->active)
        {
          // And touch in this object area
          if(   (tx >= p_obj->GetStartX()) && (tx <= p_obj->GetEndX())
             && (ty >= p_obj->GetStartY()) && (ty <= p_obj->GetEndY())
             && (tmp_tx >= p_obj->GetStartX()) && (tmp_tx <= p_obj->GetEndX())
             && (tmp_ty >= p_obj->GetStartY()) && (tmp_ty <= p_obj->GetEndY()) )
          {
            // Call Action() function for Move
            p_obj->Action(VisObject::ACT_MOVE, tmp_tx, tmp_ty);
            // No need check all other objects - only one object can be touched
            break;
          }
          if(   (tx >= p_obj->GetStartX()) && (tx <= p_obj->GetEndX())
             && (ty >= p_obj->GetStartY()) && (ty <= p_obj->GetEndY())
             && (   ((tmp_tx < p_obj->GetStartX()) || (tmp_tx > p_obj->GetEndX()))
                 || ((tmp_ty < p_obj->GetStartY()) || (tmp_ty > p_obj->GetEndY())) ) )
          {
            // Call Action() function for Move Out
            p_obj->Action(VisObject::ACT_MOVEOUT, tmp_tx, tmp_ty);
          }
          if(   (tmp_tx >= p_obj->GetStartX()) && (tmp_tx <= p_obj->GetEndX())
             && (tmp_ty >= p_obj->GetStartY()) && (tmp_ty <= p_obj->GetEndY())
             && (   ((tx < p_obj->GetStartX()) || (tx > p_obj->GetEndX()))
                 || ((ty < p_obj->GetStartY()) || (ty > p_obj->GetEndY())) ) )
          {
            // Call Action() function for Move In
            p_obj->Action(VisObject::ACT_MOVEIN, tmp_tx, tmp_ty);
          }
        }
        // Get previous object
        p_obj = p_obj->p_prev;
      }
    }
    // Give semaphore after changes
    line_mutex.Release();
  }
  // If touch state changed (touch & release)
  if(is_touch != tmp_is_touch)
  {
    // Go thru VisObject list and call Active() function for active object
    // Take semaphore before draw line
    line_mutex.Lock();
    // Set pointer to first element
    VisObject* p_obj = object_list_last;
    // If list not empty
    if(p_obj != nullptr)
    {
      // Do for all objects
      while(p_obj != nullptr)
      {
        // If we found active object
        if(p_obj->active)
        {
          // And touch in this object area
          if(   (tmp_tx >= p_obj->GetStartX()) && (tmp_tx <= p_obj->GetEndX())
             && (tmp_ty >= p_obj->GetStartY()) && (tmp_ty <= p_obj->GetEndY()) )
          {
            // Call Action() function
            p_obj->Action(tmp_is_touch ? VisObject::ACT_TOUCH : VisObject::ACT_UNTOUCH,
                          tmp_tx, tmp_ty);
            // No need check all other objects - only one object can be touched
            break;
          }
        }
        // Get previous object
        p_obj = p_obj->p_prev;
      }
    }
    // Give semaphore after changes
    line_mutex.Release();
  }
  // Save new touch state
  is_touch = tmp_is_touch;
  tx = tmp_tx;
  ty = tmp_ty;

  // FIX ME: debug code. Should be removed.
  if(DISPLAY_DEBUG_INFO)
  {
    if(is_touch) sprintf(str, "X: %4ld, Y: %4ld", tx, ty);
    else sprintf(str, "FPS: %2lu.%1lu, time: %lu", fps_x10/10, fps_x10%10, RtosTick::GetTimeMs()/1000UL);
    fps_str.SetString(str);
  }

  // Always run
  return Result::RESULT_OK;
}

// *****************************************************************************
// ***   Add Visual Object to object list   ************************************
// *****************************************************************************
Result DisplayDrv::AddVisObjectToList(VisObject* obj, uint32_t z)
{
  Result result = Result::ERR_NULL_PTR;

  if((obj != nullptr) && (obj->p_prev == nullptr) && (obj->p_next == nullptr) && (obj != object_list))
  {
    // Take semaphore before add to list
    line_mutex.Lock();
    // Set object Z
    obj->z = z;
    // Set prev pointer to nullptr
    obj->p_prev = nullptr;
    // Set next pointer to nullptr
    obj->p_next = nullptr;
    // If object list empty
    if(object_list == nullptr)
    {
      // Add object to list
      object_list = obj;
      // Set pointer to last object in the list
      object_list_last = obj;
    }
    else if(object_list->z > z)
    {
      // Set next element to current head element
      obj->p_next = object_list;
      // Set prev element to next after head element
      object_list->p_prev = obj;
      // Set new head for list
      object_list = obj;
    }
    else
    {
      // Set temporary pointer
      VisObject* p_last = object_list;
      // Find last element 
      while((p_last->p_next != nullptr) && (p_last->p_next->z < z)) p_last = p_last->p_next;
      // If it not last element
      if(p_last->p_next != nullptr)
      {
        // Set next pointer in object
        obj->p_next = p_last->p_next;
        // Set prev pointer in object
        obj->p_next->p_prev = obj;
      }
      else
      {
        // Set pointer to last object in the list
        object_list_last = obj;
      }
      // Set next pointer in prev element
      p_last->p_next = obj;
      // Set prev pointer to new object in list
      obj->p_prev = p_last;
    }
    // Give semaphore after changes
    line_mutex.Release();
    // Set return status
    result = Result::RESULT_OK;
  }

  return result;
}

// *****************************************************************************
// ***   Delete Visual Object from object list   *******************************
// *****************************************************************************
Result DisplayDrv::DelVisObjectFromList(VisObject* obj)
{
  Result result = Result::ERR_NULL_PTR;

  if((obj != nullptr) && ((obj->p_prev != nullptr) || (obj->p_next != nullptr) || (obj == object_list)) )
  {
    // Take semaphore before delete from list
    line_mutex.Lock();
    // Remove element from head
    if(obj == object_list)
    {
      // Set pointer to next object or clear pointer if no more elements
      object_list = obj->p_next;
      // Clear previous element in first object
      object_list->p_prev = nullptr;
    }
    else if(obj == object_list_last)
    {
      // Set next pointer in previous object to nullptr
      obj->p_prev->p_next = nullptr;
      // Set pointer to previous object
      object_list_last = obj->p_prev;
    }
    else
    {
      // Remove element from head
      if(obj->p_prev == nullptr) object_list = obj->p_next;
      // Remove element from middle
      else if(obj->p_next != nullptr)
      {
        obj->p_prev->p_next = obj->p_next;
        obj->p_next->p_prev = obj->p_prev;
      }
      // remove element from tail
      else obj->p_prev->p_next = nullptr;
    }
    // Clear pointers in object
    obj->p_prev = nullptr;
    obj->p_next = nullptr;
    // Give semaphore after changes
    line_mutex.Release();
    // Set return status
    result = Result::RESULT_OK;
  }

  return result;
}

// *****************************************************************************
// ***   Lock display   ********************************************************
// *****************************************************************************
Result DisplayDrv::LockDisplay(uint32_t wait_ms)
{
  // Take semaphore for protect draw frame
  Result result = frame_mutex.Lock(wait_ms);
  // Return result
  return result;
}

// *****************************************************************************
// ***   Unlock display   ******************************************************
// *****************************************************************************
Result DisplayDrv::UnlockDisplay(void)
{
  // Give semaphore for drawing frame
  Result result = frame_mutex.Release();
  // Return result
  return result;
}

// *****************************************************************************
// ***   Lock display line   ***************************************************
// *****************************************************************************
Result DisplayDrv::LockDisplayLine(uint32_t wait_ms)
{
  // Take semaphore for protect draw line
  Result result = line_mutex.Lock(wait_ms);
  // Return result
  return result;
}

// *****************************************************************************
// ***   Unlock display line   *************************************************
// *****************************************************************************
Result DisplayDrv::UnlockDisplayLine(void)
{
  // Give semaphore for drawing frame
  Result result = line_mutex.Release();
  // Return result
  return result;
}

// *****************************************************************************
// ***   Update display   ******************************************************
// *****************************************************************************
Result DisplayDrv::UpdateDisplay(void)
{
  // Give semaphore for update screen
  Result result = screen_update.Give();
  // Return result
  return result;
}

// *****************************************************************************
// ***   Set Update Mode   *****************************************************
// *****************************************************************************
void DisplayDrv::SetUpdateMode(bool is_vertical)
{
  // Lock display
  LockDisplay();
  // Wait while transfer complete before change settings
  while(tft.IsTransferComplete() == false);
  // Change Update mode
  if(is_vertical)
  {
    tft.SetRotation(2U);
  }
  else
  {
    tft.SetRotation(3U);
  }
  // Set width and height variables for selected screen update mode
  width = tft.GetWidth();
  height = tft.GetHeight();
  // Save Update mode
  update_mode = is_vertical;
  // Unlock display
  UnlockDisplay();  
}

// *****************************************************************************
// ***   Get Touch X and Y coordinate   ****************************************
// *****************************************************************************
bool DisplayDrv::GetTouchXY(int32_t& x, int32_t& y)
{
  // Result variable
  bool result = false;
  
  // Try to take mutex. 1 ms should be enough.
  if(touchscreen_mutex.Lock(1U) == Result::RESULT_OK)
  {
    // If display driver gets touch coordinates and touch still present
    if(is_touch && touch.IsTouch())
    {
      // Return last values
      x = tx;
      y = ty;
      // Set result
      result = true;
    }
    else
    {
      // If no touch - clear flag for prevent return wrong coordinates if
      // display will touched without reads new coordinates
      is_touch = false;
    }
    // Give semaphore for drawing frame - we can enter in this "if" statement
    // only if mutex taken
    touchscreen_mutex.Release();
  }
  // Return result
  return result;
}

// *************************************************************************
// ***   Check touch   *****************************************************
// *************************************************************************
bool DisplayDrv::IsTouch()
{
	return touch.IsTouch();
}

// *****************************************************************************
// ***   Calibrate Touchscreen   ***********************************************
// *****************************************************************************
void DisplayDrv::TouchCalibrate()
{
  // Box for calibration
  Box background(0, 0, width, height, COLOR_BLACK, true);
  Box box(0, 0, 2, 2, COLOR_WHITE, true);
  int32_t tx;
  int32_t ty;
  int32_t x1, x2;
  int32_t y1, y2;

  // Reset calibration
  touch.SetCalibrationConsts(XPT2046::COEF, XPT2046::COEF, 0, 0);

  // Show background box
  background.Show(0xFFFFFFFFU-1U);
  // Show box
  box.Show(0xFFFFFFFFU);

  // Move box to position
  box.Move(10-1, 10-1);
  // Wait press for get initial coordinates
  while(!GetTouchXY(x1, y1))
  {
    // Update Display
    UpdateDisplay();
    // Delay
    RtosTick::DelayMs(100U);
  }
  // Wait unpress and measure coordinates continuously for averaging
  while(GetTouchXY(tx, ty))
  {
    x1 = (x1 + tx) / 2;
    y1 = (y1 + ty) / 2;
    // Update Display - for update touch coordinates
    UpdateDisplay();
    // Delay
    RtosTick::DelayMs(100U);
  }

  // Move box to position
  box.Move(width - 10 - 1, height - 10 - 1);
  // Wait press for get initial coordinates
  while(!GetTouchXY(x2, y2))
  {
    // Update Display
    UpdateDisplay();
    // Delay
    RtosTick::DelayMs(100U);
  }
  // Wait unpress and measure coordinates continuously for averaging
  while(GetTouchXY(tx, ty))
  {
    x2 = (x2 + tx) / 2;
    y2 = (y2 + ty) / 2;
    // Update Display
    UpdateDisplay();
    // Delay
    RtosTick::DelayMs(100U);
  }

  // Calc coefs
  int32_t kx = ((x2 - x1) * XPT2046::COEF) / (width  - 2*10);
  int32_t ky = ((y2 - y1) * XPT2046::COEF) / (height - 2*10);
  int32_t bx = 10 - (x1 * XPT2046::COEF) / kx;
  int32_t by = 10 - (y1 * XPT2046::COEF) / ky;

  // Save calibration
  touch.SetCalibrationConsts(kx, ky, bx, by);

  // Hide box
  box.Hide();
}
