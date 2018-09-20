//******************************************************************************
//  @file Application.h
//  @author Nicolai Shlapunov
//
//  @details Application: User Application Class, header
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

#ifndef Application_h
#define Application_h

// *****************************************************************************
// ***   Includes   ************************************************************
// *****************************************************************************
#include "DevCfg.h"
#include "AppTask.h"
#include "DisplayDrv.h"
#include "InputDrv.h"
#include "SoundDrv.h"
#include "UiEngine.h"

#include "IIic.h"

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
class Application : public AppTask
{
  public:
    // *************************************************************************
    // ***   Get Instance   ****************************************************
    // *************************************************************************
    static Application& GetInstance(void);

    // *************************************************************************
    // ***   Application Loop   ************************************************
    // *************************************************************************
    virtual Result Loop();

  private:
    // Display driver instance
    DisplayDrv& display_drv = DisplayDrv::GetInstance();
    // Input driver instance
    InputDrv& input_drv = InputDrv::GetInstance();
    // Sound driver instance
    SoundDrv& sound_drv = SoundDrv::GetInstance();

    // *************************************************************************
    // ***   I2C Ping function   ***********************************************
    // *************************************************************************
    Result IicPing(IIic& iic);

    // *************************************************************************
    // ***   ProcessUserInput   ************************************************
    // *************************************************************************
    static char* GetMenuStr(void* ptr, char* buf, uint32_t n, uint32_t add_param);

    // *************************************************************************
    // ***   Private constructor   *********************************************
    // *************************************************************************
    Application() : AppTask(APPLICATION_TASK_STACK_SIZE, APPLICATION_TASK_PRIORITY,
                            "Application") {};
};

// *****************************************************************************
// ***   CheckBox Class   ******************************************************
// *****************************************************************************
class SoundControlBox : public Image
{
  public:
    // *************************************************************************
    // ***   Constructor   *****************************************************
    // *************************************************************************
    SoundControlBox(int32_t x, int32_t y, bool mute_flag = true);

    // *************************************************************************
    // ***   Put line in buffer   **********************************************
    // *************************************************************************
    virtual void Action(VisObject::ActionType action, int32_t tx, int32_t ty);

  private:
    // Mute flag
    bool mute = false;
    
    // Sound driver instance
    SoundDrv& sound_drv = SoundDrv::GetInstance();
};

#endif
