//******************************************************************************
//  @file AppCfg.h
//  @author Nicolai Shlapunov
//
//  @details Application: Application Config file, header
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

#ifndef AppCfg_h
#define AppCfg_h

// *****************************************************************************
// ***   Includes   ************************************************************
// *****************************************************************************
#include "DevCfg.h"
#include "adc.h"
#include "spi.h"
#include "tim.h"
#include "usb_device.h"

// *****************************************************************************
// ***   Configuration   *******************************************************
// *****************************************************************************

// *** Tasks stack sizes   *****************************************************
const static uint16_t APPLICATION_TASK_STACK_SIZE = 1024U;
// *** Tasks priorities   ******************************************************
const static uint32_t APPLICATION_TASK_PRIORITY = tskIDLE_PRIORITY + 2U;
// *****************************************************************************

#endif
