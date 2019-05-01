//******************************************************************************
//  @file InputTest.cpp
//  @author Nicolai Shlapunov
//
//  @details Application: Input Test Application Class, implementation
//
//  @copyright Copyright (c) 2017, Devtronic & Nicolai Shlapunov
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
#include "InputTest.h"

// *****************************************************************************
// ***   Get Instance   ********************************************************
// *****************************************************************************
InputTest& InputTest::GetInstance(void)
{
   static InputTest input_test;
   return input_test;
}

// *****************************************************************************
// ***   Test get function   ***************************************************
// *****************************************************************************
char* InputTest::GetMenuStr(void* ptr, char * buf, uint32_t n, uint32_t add_param)
{
//  InputTest* it = (InputTest*)ptr;
  snprintf(buf, n, "%lu", add_param);
  return buf;
}

// *****************************************************************************
// ***   Application Loop   ****************************************************
// *****************************************************************************
Result InputTest::Loop()
{
  char str_left[32] = {"\0"};
  String left_str(str_left, 30-2, 20 - 14, COLOR_MAGENTA, String::FONT_8x12);
  char str_right[32] = {"\0"};
  String right_str(str_right, 190-2, 20 - 14, COLOR_MAGENTA, String::FONT_8x12);

  Box box_left(30-2, 20, 100+4, 100+4, COLOR_YELLOW);
  Circle circle_left(30-1, 20-1, 3, COLOR_RED, true);
  char str_left_data[128] = {"\0"};
  String left_str_data(str_left_data, 0, 200, COLOR_MAGENTA, String::FONT_6x8);

  Box box_right(190-2, 20, 100+4, 100+4, COLOR_YELLOW);
  Circle circle_right(190-1, 20-1, 3, COLOR_RED, true);
  char str_right_data[128] = {"\0"};
  String right_str_data(str_right_data, 0, 212, COLOR_MAGENTA, String::FONT_6x8);

  if(input_drv.GetDeviceType(InputDrv::EXT_LEFT) == InputDrv::EXT_DEV_JOY)
  {
    sprintf(str_left, "JOYSTICK");
    left_str.Show(30);

  	box_left.Show(10);
  	circle_left.Show(20);
    left_str_data.Show(30);
  }
  if(input_drv.GetDeviceType(InputDrv::EXT_LEFT) == InputDrv::EXT_DEV_ENC)
  {
    sprintf(str_left, "ENCODER");
    left_str.Show(30);
  }
  if(input_drv.GetDeviceType(InputDrv::EXT_LEFT) == InputDrv::EXT_DEV_BTN)
  {
    sprintf(str_left, "BUTTONS");
    left_str.Show(30);
  }

  if(input_drv.GetDeviceType(InputDrv::EXT_RIGHT) == InputDrv::EXT_DEV_JOY)
  {
    sprintf(str_left, "JOYSTICK");
    left_str.Show(30);

    box_right.Show(10);
    circle_right.Show(20);
    right_str_data.Show(30);
  }
  if(input_drv.GetDeviceType(InputDrv::EXT_RIGHT) == InputDrv::EXT_DEV_ENC)
  {
    sprintf(str_right, "ENCODER");
    right_str.Show(30);
  }
  if(input_drv.GetDeviceType(InputDrv::EXT_RIGHT) == InputDrv::EXT_DEV_BTN)
  {
    sprintf(str_right, "BUTTONS");
    right_str.Show(30);
  }

  int32_t x = 0;
  int32_t y = 0;

  while(1)
  {
    if(input_drv.GetDeviceType(InputDrv::EXT_LEFT) == InputDrv::EXT_DEV_JOY)
    {
   	  // Get values for left
      input_drv.GetJoystickState(InputDrv::EXT_LEFT, x, y);
 		  circle_left.Move(30-2 + (x * 100) / 4095, 20-2 + (y * 100) / 4095);
 		  sprintf(str_left_data, "LEFT:  X=%4li, Y=%4li", x, y);
    }

    if(input_drv.GetDeviceType(InputDrv::EXT_RIGHT) == InputDrv::EXT_DEV_JOY)
    {
 		  // Get values for right
 		  input_drv.GetJoystickState(InputDrv::EXT_RIGHT, x, y);
 		  circle_right.Move(190-2 + (x * 100) / 4095, 20-2 + (y * 100) / 4095);
 		  sprintf(str_right_data, "RIGHT: X=%4li, Y=%4li", x, y);
    }

		// Update Display
		display_drv.UpdateDisplay();
    // Pause
		RtosTick::DelayTicks(50U);

    // Exit by touch
    if(display_drv.IsTouched() == true)
    {
   	  break;
    }
  }

  // Always run
  return Result::RESULT_OK;
}
