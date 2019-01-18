//******************************************************************************
//  @file DevCfg.h
//  @author Nicolai Shlapunov
//
//  @details DevCore: Config file, header
//
//  @section LICENSE
//
//   Software License Agreement (Modified BSD License)
//
//   Copyright (c) 2016, Devtronic & Nicolai Shlapunov
//   All rights reserved.
//
//   Redistribution and use in source and binary forms, with or without
//   modification, are permitted provided that the following conditions are met:
//
//   1. Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//   2. Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//   3. Neither the name of the Devtronic nor the names of its contributors
//      may be used to endorse or promote products derived from this software
//      without specific prior written permission.
//   4. Redistribution and use of this software other than as permitted under
//      this license is void and will automatically terminate your rights under
//      this license.
//
//   THIS SOFTWARE IS PROVIDED BY DEVTRONIC ''AS IS'' AND ANY EXPRESS OR IMPLIED
//   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
//   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
//   IN NO EVENT SHALL DEVTRONIC BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
//   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//  @section SUPPORT
//
//   Devtronic invests time and resources providing this open source code,
//   please support Devtronic and open-source hardware/software by
//   donations and/or purchasing products from Devtronic.
//
//******************************************************************************

#ifndef DevCfg_h
#define DevCfg_h

// *****************************************************************************
// ***   Includes   ************************************************************
// *****************************************************************************
#include "Result.h"
#include "Rtos.h"

// Include for all hardware stuff
#include "main.h"
#include "stm32f4xx.h"

// ***   ADC   *****************************************************************
#ifdef HAL_ADC_MODULE_ENABLED
#include "adc.h"
#else
typedef uint32_t ADC_HandleTypeDef; // Dummy ADC handle for compilation
#endif
// ***   SPI   *****************************************************************
#ifdef HAL_SPI_MODULE_ENABLED
#include "spi.h"
#else
typedef uint32_t SPI_HandleTypeDef; // Dummy SPI handle for compilation
#endif
// ***   I2C   *****************************************************************
#ifdef HAL_I2C_MODULE_ENABLED
#include "i2c.h"
#else
typedef uint32_t I2C_HandleTypeDef; // Dummy I2C handle for compilation
#endif
// ***   TIM   *****************************************************************
#ifdef HAL_TIM_MODULE_ENABLED
#include "tim.h"
#else
typedef uint32_t TIM_HandleTypeDef; // Dummy TIM handle for compilation
#endif
// ***   DAC   *****************************************************************
#ifdef HAL_DAC_MODULE_ENABLED
#include "dac.h"
#else
typedef uint32_t DAC_HandleTypeDef; // Dummy DAC handle for compilation
#endif

#include "usb_device.h"

// *****************************************************************************
// ***   Configuration   *******************************************************
// *****************************************************************************

// ***   SPI Handles   *********************************************************
#ifdef HAL_SPI_MODULE_ENABLED
  // Display SPI handle
  static SPI_HandleTypeDef* const TFT_HSPI = &hspi1;
  // Touchscreen SPI handle
  static SPI_HandleTypeDef* const TOUCH_HSPI = &hspi1;
#endif
// ***   I2C Handles   *********************************************************
#ifdef HAL_I2C_MODULE_ENABLED
  // BME280 I2C handle
  static I2C_HandleTypeDef& BME280_HI2C = hi2c1;
#endif
// ***   TIM Handles   *********************************************************
#ifdef HAL_TIM_MODULE_ENABLED
  // Sound Timer handle
  static TIM_HandleTypeDef* const SOUND_HTIM = &htim4;
  // Sound Timer channel
  static const uint32_t SOUND_CHANNEL = TIM_CHANNEL_2;
#endif

// *** Applications tasks stack sizes   ****************************************
const static uint16_t APPLICATION_TASK_STACK_SIZE = 1024U;
const static uint16_t EXAMPLE_MSG_TASK_STACK_SIZE = configMINIMAL_STACK_SIZE;
// *** Applications tasks priorities   *****************************************
const static uint8_t APPLICATION_TASK_PRIORITY = tskIDLE_PRIORITY + 2U;
const static uint8_t EXAMPLE_MSG_TASK_PRIORITY = tskIDLE_PRIORITY + 2U;
// *****************************************************************************

// *** System tasks stack sizes   **********************************************
const static uint16_t DISPLAY_DRV_TASK_STACK_SIZE = 256U;
const static uint16_t INPUT_DRV_TASK_STACK_SIZE   = configMINIMAL_STACK_SIZE;
const static uint16_t SOUND_DRV_TASK_STACK_SIZE   = configMINIMAL_STACK_SIZE;
// *** System tasks priorities   ***********************************************
const static uint8_t DISPLAY_DRV_TASK_PRIORITY = tskIDLE_PRIORITY + 1U;
const static uint8_t INPUT_DRV_TASK_PRIORITY   = tskIDLE_PRIORITY + 2U;
const static uint8_t SOUND_DRV_TASK_PRIORITY   = tskIDLE_PRIORITY + 3U;
// *****************************************************************************

// *****************************************************************************
// ***   Macroses   ************************************************************
// *****************************************************************************

// Number of array elements
#define NumberOf(x) (sizeof(x)/sizeof((x)[0]))

// Break macro - useful for debugging
#define Break() asm volatile("bkpt #0")

// *****************************************************************************
// ***   Overloaded operators   ************************************************
// *****************************************************************************
void* operator new(size_t sz);
void* operator new[](size_t sz);
void operator delete(void* p);
void operator delete[](void* p);
void* operator new(size_t size, void* p);
void* operator new[](size_t size, void* p);
void operator delete(void*, void*);
void operator delete[](void*, void*);

#endif
