//******************************************************************************
//  @file AppTask.cpp
//  @author Nicolai Shlapunov
//
//  @details DevCore: Application Task Base Class, implementation
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
#include "AppTask.h"

// *****************************************************************************
// ***   IncStatic variables   *************************************************
// *****************************************************************************
static SemaphoreHandle_t mutex = nullptr;
static uint32_t cnt = 0U;

// *****************************************************************************
// ***   Change counter   ******************************************************
// *****************************************************************************
void ChangeCnt(bool is_up)
{
  // If it first run
  if(mutex == nullptr)
  {
    // Initialize mutex
    mutex = xSemaphoreCreateMutex();
  }
  // Take semaphore before change counter
  xSemaphoreTake(mutex, portMAX_DELAY);
  // Check direction
  if(is_up == true)
  {
    // Increment counter
    cnt++;
  }
  else
  {
    // Decrement counter
    cnt--;
  }
  // Give semaphore after changes
  xSemaphoreGive(mutex);
  // If it last run
  if(mutex == nullptr)
  {
    // Delete mutex
    vSemaphoreDelete(mutex);
    // Clear handle
    mutex = nullptr;
  }
}

// *****************************************************************************
// ***   Wrapper for use class function   **************************************
// *****************************************************************************
extern "C" void TaskFunWrapper(void* parm)
{
  // Increment counter before call Setup()
  ChangeCnt(true);
  // Call virtual Setup() function from AppTask class
  (static_cast<AppTask*>(parm))->Setup(nullptr);
  // Decrement counter after call Setup()
  ChangeCnt(false);
  // Pause for give other tasks run Setup()
  vTaskDelay(1U);
  // Pause while other tasks run Setup() before executing any Loop()
  while(cnt) vTaskDelay(1U);
  // Call virtual Loop() function from AppTask class in the loop
  while((static_cast<AppTask*>(parm))->Loop(nullptr));
  // Delete task after exit
  vTaskDelete(nullptr);
}

// *****************************************************************************
// ***   Create task function   ************************************************
// *****************************************************************************
void AppTask::CreateTask(const char * const pcName,
                         const uint16_t usStackDepth,
                         UBaseType_t uxPriority)
{
  // Create task: function - TaskFunWrapper(), parameter - pointer "this"
  xTaskCreate(TaskFunWrapper, pcName, usStackDepth, this, uxPriority, nullptr);
}
