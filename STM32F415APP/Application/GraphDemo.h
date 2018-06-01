//******************************************************************************
//  @file GraphDemo.h
//  @author Nicolai Shlapunov
//
//  @details Application: Graphic Demo Application Class, header
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

#ifndef GraphDemo_h
#define GraphDemo_h

// *****************************************************************************
// ***   Includes   ************************************************************
// *****************************************************************************
#include "DevCfg.h"
#include "AppTask.h"
#include "DisplayDrv.h"
#include "InputDrv.h"
#include "SoundDrv.h"

// *****************************************************************************
// ***   Local const variables   ***********************************************
// *****************************************************************************

// *****************************************************************************
// ***   Defines   *************************************************************
// *****************************************************************************
#define BG_Z (100)

// *****************************************************************************
// ***   Application Class   ***************************************************
// *****************************************************************************
class GraphDemo : public AppTask
{
  public:
    // *************************************************************************
    // ***   Get Instance   ****************************************************
    // *************************************************************************
    static GraphDemo& GetInstance(void);

    // *************************************************************************
    // ***   Application Loop   ************************************************
    // *************************************************************************
    virtual Result Loop();

  private:
    // Button states
    bool btn_states[InputDrv::BTN_MAX];
    // Init time variable
    int32_t last_enc1_val = 0;
    // Init time variable
    int32_t last_enc2_val = 0;

    // Display driver instance
    DisplayDrv& display_drv = DisplayDrv::GetInstance();

    // Input driver instance
    InputDrv& input_drv = InputDrv::GetInstance();

    // *************************************************************************
    // ***   ProcessUserInput   ************************************************
    // *************************************************************************
    bool ProcessUserInput(void);

    // *************************************************************************
    // ***   Private constructor   *********************************************
    // *************************************************************************
    GraphDemo() : AppTask(APPLICATION_TASK_STACK_SIZE, APPLICATION_TASK_PRIORITY,
                          "GraphDemo") {};
};

// *****************************************************************************
// ***   Application Class   ***************************************************
// *****************************************************************************
class VisObjectRandomMover
{
  public:
    // *************************************************************************
    // ***   Constructor   *****************************************************
    // *************************************************************************
    explicit VisObjectRandomMover(VisObject& obj);

    // *************************************************************************
    // ***   Init Application Task   *******************************************
    // *************************************************************************
    void Process(void);
    
  private:
    VisObject& object;
    int8_t x_dir = 0;
    int8_t y_dir = 0;
    int8_t speed = 0;

    // Display driver instance
    DisplayDrv& display_drv = DisplayDrv::GetInstance();
};

#endif
