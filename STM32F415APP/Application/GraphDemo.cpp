//******************************************************************************
//  @file GraphDemo.cpp
//  @author Nicolai Shlapunov
//
//  @details Application: Graphic Demo Application Class, implementation
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
#include "GraphDemo.h"

#include <math.h>

// *****************************************************************************
// ***   Get Instance   ********************************************************
// *****************************************************************************
GraphDemo& GraphDemo::GetInstance(void)
{
   static GraphDemo gd;
   return gd;
}

// *****************************************************************************
// ***   GraphDemo Loop   ******************************************************
// *****************************************************************************
Result GraphDemo::Loop()
{
  static VisObjectRandomMover* pointer_list[60];
  uint32_t list_item_cnt = 0;

  Circle circle1(150-30, 120+30, 30, COLOR_MAGENTA, true);
  circle1.Show(40);

  Circle circle2(150, 120, 30, COLOR_BLUE);
  circle2.Show(50);

  Line line1(46, 34, 126, 210, COLOR_GREEN);
  line1.Show(30);
  Line line2(46, 34, 310, 126, COLOR_CYAN);
  line2.Show(30);

  String str1("Hello World!", 0, 10, COLOR_MAGENTA, String::FONT_4x6);
  str1.Show(70);
  String str2("Hello World!", 0, 20, COLOR_CYAN, String::FONT_6x8);
  str2.Show(80);
  String str3("Hello World!", 0, 30, COLOR_YELLOW, String::FONT_8x8);
  str3.Show(90);
  String str4("Hello World!", 0, 50, COLOR_GREEN,COLOR_MAGENTA, String::FONT_8x12);
  str4.Show(100);
  String str5("Hello World!", 0, 70, COLOR_RED, String::FONT_12x16);
  str5.Show(110);

  Box box1(0, 0, 100, 10, COLOR_RED, true);
  box1.Show(10);
  Box box2(100, 70, 20, 10, COLOR_YELLOW);
  box2.Show(20);

  pointer_list[list_item_cnt++] = new VisObjectRandomMover(circle1);
  pointer_list[list_item_cnt++] = new VisObjectRandomMover(circle2);
  pointer_list[list_item_cnt++] = new VisObjectRandomMover(line1);
  pointer_list[list_item_cnt++] = new VisObjectRandomMover(line2);
  pointer_list[list_item_cnt++] = new VisObjectRandomMover(str1);
  pointer_list[list_item_cnt++] = new VisObjectRandomMover(str2);
  pointer_list[list_item_cnt++] = new VisObjectRandomMover(str3);
  pointer_list[list_item_cnt++] = new VisObjectRandomMover(str4);
  pointer_list[list_item_cnt++] = new VisObjectRandomMover(str5);
  pointer_list[list_item_cnt++] = new VisObjectRandomMover(box1);
  pointer_list[list_item_cnt++] = new VisObjectRandomMover(box2);

  // Infinite loop
  while (1)
  {
    // Lock Display
    if(display_drv.LockDisplay() == Result::RESULT_OK)
    {
      // Move all objects
      for(uint32_t i=0; i < list_item_cnt; i++) pointer_list[i]->Process();
      // Unlock Display
      display_drv.UnlockDisplay();
      // Update Display
      display_drv.UpdateDisplay();
      // Pause for switch to Display Task
      RtosTick::DelayTicks(1U);
    }
  }
  // Always run
  return Result::RESULT_OK;
}

// *************************************************************************
// ***   Constructor   *****************************************************
// *************************************************************************
VisObjectRandomMover::VisObjectRandomMover(VisObject& obj) : object(obj)
{
  x_dir = 1 + rand()%6;
  y_dir = 1 + rand()%6;
  speed = 1;
}

// *************************************************************************
// ***   Constructor   *****************************************************
// *************************************************************************
void VisObjectRandomMover::Process(void)
{
  int32_t dx=0, dy=0;
  int32_t mid_x = (object.GetEndX() + object.GetStartX())/2;
  int32_t mid_y = (object.GetEndY() + object.GetStartY())/2;
  // X move
  if((mid_x+x_dir >= 0) && (mid_x+x_dir < display_drv.GetScreenW()))
  {
    dx = x_dir;
  }
  else
  {
    x_dir = -x_dir;
  }
  
  // Y move
  if((mid_y+y_dir >= 0) && (mid_y+y_dir < display_drv.GetScreenH()))
  {
    dy = y_dir;
  }
  else
  {
    y_dir = -y_dir;
  }
  
  // Move object
  object.Move(dx, dy, true);
}
