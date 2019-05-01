//******************************************************************************
//  @file AppMain.cpp
//  @author Nicolai Shlapunov
//
//  @details Application: Main file, header
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
#include "DevCfg.h"
// Objects
#include "StHalSpi.h"
#include "StHalGpio.h"
#include "ILI9341.h"
#include "XPT2046.h"
// Tasks
#include "DisplayDrv.h"
#include "InputDrv.h"
#include "SoundDrv.h"
#include "ExampleMsgTask.h"
// Application
#include "Application.h"

// *****************************************************************************
// ***   Objects   *************************************************************
// *****************************************************************************

// GPIOs
static StHalGpio display_cs(LCD_CS_GPIO_Port, LCD_CS_Pin, IGpio::OUTPUT);
static StHalGpio display_dc(LCD_DC_GPIO_Port, LCD_DC_Pin, IGpio::OUTPUT);
static StHalGpio touch_cs(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, IGpio::OUTPUT);
static StHalGpio touch_irq(T_IRQ_GPIO_Port, T_IRQ_Pin, IGpio::INPUT);
// Interfaces
static StHalSpi spi1(hspi1);
// Other
static ILI9341 display(320, 240, spi1, display_cs, display_dc);
static XPT2046 touch(spi1, touch_cs, touch_irq);

// *****************************************************************************
// ***   Main function   *******************************************************
// *****************************************************************************
extern "C" void AppMain(void)
{
  // Init Display Driver Task
  DisplayDrv::GetInstance().SetDisplayDrv(&display);
  DisplayDrv::GetInstance().SetTouchDrv(&touch);
  DisplayDrv::GetInstance().InitTask();
  // Init Input Driver Task
  InputDrv::GetInstance().InitTask(nullptr, &hadc2);
  // Init Sound Driver Task
  SoundDrv::GetInstance().InitTask(&htim4);

  // Init Messages Test Task
  ExampleMsgTask::GetInstance().InitTask();

  // Init Application Task
  Application::GetInstance().InitTask();
}

// *****************************************************************************
// ***   Stack overflow hook function   ****************************************
// *****************************************************************************
extern "C" void vApplicationStackOverflowHook(TaskHandle_t* px_task, signed portCHAR* pc_task_name)
{
  while(1);
}

// *****************************************************************************
// ***   Malloc failed hook function   *****************************************
// *****************************************************************************
extern "C" void vApplicationMallocFailedHook(void)
{
  while(1);
}
