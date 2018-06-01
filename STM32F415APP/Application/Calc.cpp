//******************************************************************************
//  @file Calc.cpp
//  @author Nicolai Shlapunov
//
//  @details Application: Calculator Application Class, implementation
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
#include "Calc.h"

// *****************************************************************************
// ***   Get Instance   ********************************************************
// *****************************************************************************
Calc& Calc::GetInstance(void)
{
   static Calc calc;
   return calc;
}

// *****************************************************************************
// ***   Test get function   ***************************************************
void Calc::Callback(void* ptr, void* param_ptr, uint32_t param)
{
  Calc* calc = (Calc*)ptr;
  const char* str = (const char*)param_ptr;

  // We havn't string pointer only on the Clear button
  if(str == nullptr)
  {
    calc->a = 0;
    calc->b = 0;
    calc->op = '\0';
  }
  else
  {
    // If number pressed
    if((str[0] >= '0') && (str[0] <= '9'))
    {
      // And we can add it
      if(calc->b < 999999999)
      {
        // Get digit from character
        int32_t tmp = str[0] - '0';
        // Shift value and add new digit
        calc->b = calc->b*10 + tmp;
      }
    }
    else
    {
      // Check previous operation
      switch(calc->op)
      {
        case '\0':
          calc->a = calc->b;
          break;
        case '/':
          calc->a /= calc->b;
          break;
        case '*':
          calc->a *= calc->b;
          break;
        case '-':
          calc->a -= calc->b;
          break;
        case '+':
          calc->a += calc->b;
          break;
      }
      // Other operations than '=' save operation and clear b variable
      if(str[0] != '=')
      {
        // Save operation
        calc->op = str[0];
        // Clear b variable for second argument
        calc->b = 0;
      }
      else
      {
        // Store result in b variable for show
        calc->b = calc->a;
        // Clear operation - only b variable will be on the screen
        calc->op = '\0';
      }
    }
  }
	calc->GenerateStr();
}

// *************************************************************************
// ***   GenerateStr   *****************************************************
// *************************************************************************
void Calc::GenerateStr()
{
  // No operation - show b variable only
	if(op == 0)
	{
		snprintf(str, sizeof(str), "%li", b);
	}
	// Present operation, not present b - do not show 0 for b
	else if(b == 0)
	{
		snprintf(str, sizeof(str), "%li %c", a, op);
	}
	// Show both arguments and operation
	else
	{
		snprintf(str, sizeof(str), "%li %c %li", a, op, b);
	}
}

// *****************************************************************************
// ***   Application task   ****************************************************
// *****************************************************************************
Result Calc::Loop()
{
  // Calculate buttons dimensions based on space
	int32_t space = 10;
	int32_t btn_h = (display_drv.GetScreenH()-space*6)/5;
	int32_t btn_w = (display_drv.GetScreenW()-space*5)/4;

	// Result string and button for clear
	snprintf(str, sizeof(str), "0                        ");
	result.SetParams(str, space, space, display_drv.GetScreenW()-space*2, btn_h, true);
	result.SetCallback(&Callback, this, nullptr, 0);
	result.Show(1000);

	// Buttons pad layout
	char btn_str[4*4][2] = {"/", "7", "8", "9",
	                        "*", "4", "5", "6",
	                        "-", "1", "2", "3",
	                        "+", "=", "0", "."};
	// Create buttons
	for(uint32_t i=0; i < NumberOf(btn); i++)
	{
	  btn[i].SetParams(btn_str[i], space+(btn_w+space)*(i%4), space+(btn_h+space)*(1+i/4),
	                   btn_w, btn_h, true);
	  btn[i].SetCallback(&Callback, this, (void*)btn_str[i], i);
	  btn[i].Show(1000);
	}

	// Infinite cycle. All calculations done in DispayDrv task,
	// so there we only check exit.
  while(1)
  {
		// Update Display
		display_drv.UpdateDisplay();
    // Pause
		RtosTick::DelayTicks(50U);

    // Exit by press
    if(input_drv.GetEncoderButtonState(InputDrv::EXT_LEFT, InputDrv::ENC_BTN_ENT) == true)
    {
   	  break;
    }
  }

	// Hide result
  result.Hide();
  // Hide buttons
  for(uint32_t i=0; i < NumberOf(btn); i++)
  {
    btn[i].Hide();
  }

  // Always run
  return Result::RESULT_OK;
}
