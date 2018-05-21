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
#include "AppCfg.h"
#include "DisplayDrv.h"
#include "InputDrv.h"
#include "Application.h"
#include "GraphDemo.h"
#include "Tetris.h"
#include "Gario.h"

// *****************************************************************************
// ***   Main function   *******************************************************
// *****************************************************************************
extern "C" void AppMain(void)
{
  // Init Display Driver Task
  DisplayDrv::GetInstance().InitTask();
  // Init Input Driver Task
  InputDrv::GetInstance().InitTask(nullptr, &hadc2);
  // Init Sound Driver Task
  SoundDrv::GetInstance().InitTask(&htim4);

  // Init Application Task
  Application::GetInstance().InitTask();
}

// *****************************************************************************
// ***   Stack overflow hook function   ****************************************
// *****************************************************************************
extern "C" void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed portCHAR *pcTaskName)
{
  while(1);
}
