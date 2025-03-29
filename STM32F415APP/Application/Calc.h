//******************************************************************************
//  @file Calc.h
//  @author Nicolai Shlapunov
//
//  @details Application: Calculator Application Class, header
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

#ifndef Calc_h
#define Calc_h

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
#define BG_Z (100)

// *****************************************************************************
// ***   Application Class   ***************************************************
// *****************************************************************************
class Calc : public AppTask
{
  public:
    // *************************************************************************
    // ***   Get Instance   ****************************************************
    // *************************************************************************
    static Calc& GetInstance(void);

    // *************************************************************************
    // ***   Application Loop   ************************************************
    // *************************************************************************
    virtual Result Loop();

  private:

    // Operand 1
    int32_t a = 0;
    // Operand 2
    int32_t b = 0;
    // Operation
    char op = '\0';
    // String for result
    char str[128];

    // Buttons
    UiButton result;
    UiButton btn[4*4];

    // Display driver instance
    DisplayDrv& display_drv = DisplayDrv::GetInstance();
    // Input driver instance
    InputDrv& input_drv = InputDrv::GetInstance();
    // Sound driver instance
    SoundDrv& sound_drv = SoundDrv::GetInstance();

    // *************************************************************************
    // ***   Callback   ********************************************************
    // *************************************************************************
    static Result Callback(Calc* calc, void* ptr);

    // *************************************************************************
    // ***   GenerateStr   *****************************************************
    // *************************************************************************
    void GenerateStr();

    // *************************************************************************
    // ***   Private constructor   *********************************************
    // *************************************************************************
    Calc() : AppTask(APPLICATION_TASK_STACK_SIZE, APPLICATION_TASK_PRIORITY,
                     "Calc") {};
};

#endif
