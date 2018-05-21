//******************************************************************************
//  @file Pong.h
//  @author Nicolai Shlapunov
//
//  @details Application: Pong Application Class, implementation
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
#include "Pong.h"

#define BOX_W 10
#define BOX_H 50

#define TICK_MS (50U)
#define SPEED_PIX_PER_S (50U)
#define SPEED ((SPEED_PIX_PER_S * 1000U) / (1000U / TICK_MS))

// *****************************************************************************
// ***   Get Instance   ********************************************************
// *****************************************************************************
Pong& Pong::GetInstance(void)
{
   static Pong pong;
   return pong;
}

// *****************************************************************************
// ***   Init User Application Task   ******************************************
// *****************************************************************************
void Pong::InitTask(void)
{
  // Create task
  CreateTask("Pong", APPLICATION_TASK_STACK_SIZE, APPLICATION_TASK_PRIORITY);
}

// *****************************************************************************
// ***   Application Loop   ****************************************************
// *****************************************************************************
bool Pong::Loop(void* pvParameters)
{
  if(   (input_drv.GetDeviceType(InputDrv::EXT_LEFT) == InputDrv::EXT_DEV_BTN)
     || (input_drv.GetDeviceType(InputDrv::EXT_RIGHT) == InputDrv::EXT_DEV_BTN) )
  {
    UiMsgBox msg_box("Buttons module can't be used", "Error!");
    msg_box.Run(3000U);
  }
  else
  {
    uint8_t left_score = 0U;
    uint8_t right_score = 0U;
    // Init last encoders & buttons values
    (void) input_drv.GetEncoderState(InputDrv::EXT_LEFT,  last_enc_left_val);
    (void) input_drv.GetEncoderState(InputDrv::EXT_RIGHT, last_enc_right_val);

    // Initialize random seed
    srand(xTaskGetTickCount());

    char scr_str[32] = {" 0 : 0 "};
    String score_str(scr_str, (display_drv.GetScreenW() - strlen(scr_str)*String::GetFontW(String::FONT_12x16))/2,
                     16, COLOR_WHITE, String::FONT_12x16);
    score_str.Show(32768);

    // Init time variable
    uint32_t last_wake_time = xTaskGetTickCount();

    Circle ball(150-30, 120+30, 5, COLOR_MAGENTA, true);
    ball.Show(32768+1);

    Box box_left(0, 0, BOX_W, BOX_H, COLOR_WHITE, true);
    Box box_right(display_drv.GetScreenW() - BOX_W, 0, BOX_W, BOX_H, COLOR_WHITE, true);
    box_left.Show(32768+2);
    box_right.Show(32768+2);

    // Update Display
    display_drv.UpdateDisplay();
    // Pause until next tick
    vTaskDelayUntil(&last_wake_time, 200U);
        
    // Clear Game Over flag before start game
    game_over = false;

    // Game cycle
    while(game_over == false)
    {
        // Lock Display
        display_drv.LockDisplay();

        if(input_drv.GetDeviceType(InputDrv::EXT_LEFT) == InputDrv::EXT_DEV_ENC)
        {
          // Get encoder 1 count since last call
          enc_left_cnt = input_drv.GetEncoderState(InputDrv::EXT_LEFT, last_enc_left_val);
          // Process result
          if(enc_left_cnt != 0)
          {
            box_left.Move(0, enc_left_cnt*abs(enc_left_cnt)*3, true);
            if(box_left.GetStartY() < 0)
            {
              box_left.Move(box_left.GetStartX(), 0);
            }
            if(box_left.GetEndY() > (int32_t)display_drv.GetScreenH())
            {
              box_left.Move(box_left.GetStartX(), display_drv.GetScreenH() - BOX_H);
            }
          }
        }

        if(input_drv.GetDeviceType(InputDrv::EXT_RIGHT) == InputDrv::EXT_DEV_ENC)
        {
          // Get encoder 2 count since last call
          enc_right_cnt = input_drv.GetEncoderState(InputDrv::EXT_RIGHT, last_enc_right_val);
          // Process result
          if(enc_right_cnt != 0)
          {
            box_right.Move(0, -enc_right_cnt*abs(enc_right_cnt)*3, true);
            if(box_right.GetStartY() < 0)
            {
              box_right.Move(box_right.GetStartX(), 0);
            }
            if(box_right.GetEndY() > (int32_t)display_drv.GetScreenH())
            {
              box_right.Move(box_right.GetStartX(), display_drv.GetScreenH() - BOX_H);
            }
          }
        }

        if(input_drv.GetDeviceType(InputDrv::EXT_LEFT) == InputDrv::EXT_DEV_JOY)
        {
          int32_t x = 0;
          int32_t y = 0;
          // Get encoder 1 count since last call
          input_drv.GetJoystickState(InputDrv::EXT_LEFT, x, y);
          // Calculate position
          int32_t pos = ((display_drv.GetScreenH() - BOX_H) * y) / 0xFFF;
          // Move box
          box_left.Move(box_left.GetStartX(), pos);
        }

        if(input_drv.GetDeviceType(InputDrv::EXT_RIGHT) == InputDrv::EXT_DEV_JOY)
        {
          int32_t x = 0;
          int32_t y = 0;
          // Get encoder 1 count since last call
          input_drv.GetJoystickState(InputDrv::EXT_RIGHT, x, y);
          // Calculate position
          int32_t pos = ((display_drv.GetScreenH() - BOX_H) * y) / 0xFFF;
          // Move box
          box_right.Move(box_right.GetStartX(), pos);
        }

        if(speed == 0)
        {
          // Variable for port - Find port for start game
          InputDrv::PortType port = (x_dir > 0) ? InputDrv::EXT_LEFT : InputDrv::EXT_RIGHT;

          // If any encoder button pressed
          if(   (input_drv.GetDeviceType(port) == InputDrv::EXT_DEV_ENC)
             && (input_drv.GetEncoderButtonState(port, InputDrv::ENC_BTN_ENT) == true) )
          {
            speed = 5;
          }
          if(   (input_drv.GetDeviceType(port) == InputDrv::EXT_DEV_JOY)
             && (input_drv.GetJoystickButtonState(port) == true) )
          {
            speed = 5;
          }
        }

        // Move ball and check miss
        if(MoveBall(ball, box_left, box_right) == true)
        {
          // Check player missed
          if(x_dir > 0) left_score++;
          else          right_score++;
          // Restore ball in the center
          ball.Move(display_drv.GetScreenW()/2, display_drv.GetScreenH()/2);
          // Change direction
        	x_dir = -x_dir;
        	// Clear speed
        	speed = 0;
        }

        // Check Game Over
        if((right_score > MAX_SCORE) || (left_score > MAX_SCORE))
        {
          game_over = true;
        }
          
        // Create score string
        sprintf(scr_str, "%2u : %-2u", left_score, right_score);
        // Unlock Display
        display_drv.UnlockDisplay();
        // Update Display
        display_drv.UpdateDisplay();
        // Pause until next tick
        vTaskDelayUntil(&last_wake_time, TICK_MS);
    }
    box_left.Hide();
    box_right.Hide();
    ball.Hide();
  }

  // Always run
  return true;
}

// *****************************************************************************
// ***   MoveBall   ************************************************************
// *****************************************************************************
bool Pong::MoveBall(Circle &ball, Box &box_left, Box &box_right)
{
  bool ret = false;
  int32_t dx=0, dy=0;
  
  uint8_t move = speed;
  
  while(move--)
  {
    // Check collision with right box
    if(   (ball.GetStartX()+dx+x_dir == box_left.GetEndX())
       && (ball.GetStartY()+dy < box_left.GetEndY())
       && (ball.GetEndY()+dy   > box_left.GetStartY()) )
    {
      x_dir = -x_dir;
      speed++;
    }
    // Check collision with left box
    else if(   (ball.GetEndX()+dx+x_dir == box_right.GetStartX())
            && (ball.GetStartY()+dy < box_right.GetEndY())
            && (ball.GetEndY()+dy   > box_right.GetStartY()) )
    {
      x_dir = -x_dir;
      speed++;
    }
    else if((ball.GetStartX()+x_dir >= 0) && (ball.GetEndX()+x_dir < display_drv.GetScreenW()))
    {
      dx += x_dir;
    }
    else
    {
      // Round over
      ret = true;
      break;
    }
    
    // Y move
    if((ball.GetStartY()+dy+y_dir >= 0) && (ball.GetEndY()+dy+y_dir < display_drv.GetScreenH()))
    {
      dy += y_dir;
    }
    else
    {
      y_dir = -y_dir;
    }
  }
  
  // Move object
  ball.Move(dx, dy, true);
  
  return ret;
}
