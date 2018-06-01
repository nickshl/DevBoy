//******************************************************************************
//  @file Pong.h
//  @author Nicolai Shlapunov
//
//  @details Application: Pong Application Class, header
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

#ifndef Pong_h
#define Pong_h

// *****************************************************************************
// ***   Includes   ************************************************************
// *****************************************************************************
#include "DevCfg.h"
#include "AppTask.h"
#include "DisplayDrv.h"
#include "InputDrv.h"
#include "SoundDrv.h"
#include "UiEngine.h"

// *****************************************************************************
// ***   Local const variables   ***********************************************
// *****************************************************************************

// *****************************************************************************
// ***   Defines   *************************************************************
// *****************************************************************************

// *****************************************************************************
// ***   Application Class   ***************************************************
// *****************************************************************************
class Pong : public AppTask
{
  public:
    // *************************************************************************
    // ***   Get Instance   ****************************************************
    // *************************************************************************
    static Pong& GetInstance(void);

    // *************************************************************************
    // ***   Pong Loop   *******************************************************
    // *************************************************************************
    virtual Result Loop();

  private:
    // Max score
    static const uint8_t MAX_SCORE = 5U;
    
    // Game over flag
    bool game_over = false;
    // Round flag
    bool round = true;

    // Direction for x and y
    int8_t x_dir = 1;
    int8_t y_dir = 1;
    
    // Ball speed
    int8_t speed = 5;

    // Encoder 1 count
    int32_t enc_left_cnt = 0;
    // Encoder 2 count
    int32_t enc_right_cnt = 0;
    // Button states
    bool btn_states[InputDrv::BTN_MAX] = { false };
    // Encoder variable
    int32_t last_enc_left_val = 0;
    // Encoder variable
    int32_t last_enc_right_val = 0;
  
    // Display driver instance
    DisplayDrv& display_drv = DisplayDrv::GetInstance();
    // Input driver instance
    InputDrv& input_drv = InputDrv::GetInstance();
    // Sound driver instance
    SoundDrv& sound_drv = SoundDrv::GetInstance();
    
    // *************************************************************************
    // ***   MoveBall   ********************************************************
    // *************************************************************************
    bool MoveBall(Circle &ball, Box &box_left, Box &box_right);

    // *************************************************************************
    // ** Private constructor. Only GetInstance() allow to access this class. **
    // *************************************************************************
    Pong() : AppTask(APPLICATION_TASK_STACK_SIZE, APPLICATION_TASK_PRIORITY,
                     "Pong") {};
};

#endif
