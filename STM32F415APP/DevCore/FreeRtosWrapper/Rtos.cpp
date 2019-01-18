//******************************************************************************
//  @file Rtos.cpp
//  @author Nicolai Shlapunov
//
//  @details DevCore: FreeRTOS Wrapper, implementation
//
//  @copyright Copyright (c) 2018, Devtronic & Nicolai Shlapunov
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
#include "Rtos.h"

// *****************************************************************************
// ***   TaskCreate   **********************************************************
// *****************************************************************************
Result Rtos::TaskCreate(TaskFunction& function, const char* task_name,
                        const uint16_t stack_depth, void* param_ptr,
                        uint8_t priority)
{
  Result result = Result::ERR_TASK_CREATE;

  // Create task: function - TaskFunWrapper(), parameter - pointer "this"
  BaseType_t res = xTaskCreate(&function, task_name, stack_depth, param_ptr, priority, nullptr);
  // Check result
  if(res == pdPASS)
  {
    result = Result::RESULT_OK;
  }

  return result;
}

// *************************************************************************
// ***   TaskDelete   ******************************************************
// *************************************************************************
void Rtos::TaskDelete(TaskHandle_t task)
{
  // Delete task
  vTaskDelete(task);
}

// *****************************************************************************
// ***   Determine whether we are in thread mode or handler mode   *************
// *****************************************************************************
bool Rtos::IsInHandlerMode(void)
{
  return __get_IPSR() != 0;
}

// *****************************************************************************
// ***   SuspendScheduler   ****************************************************
// *****************************************************************************
void Rtos::SuspendScheduler()
{
  vTaskSuspendAll();
}

// *****************************************************************************
// ***   ResumeScheduler   *****************************************************
// *****************************************************************************
void Rtos::ResumeScheduler()
{
  (void) xTaskResumeAll();
}

// *****************************************************************************
// ***   EnterCriticalSection   ************************************************
// *****************************************************************************
void Rtos::EnterCriticalSection()
{
  taskENTER_CRITICAL();
}

// *****************************************************************************
// ***   ExitCriticalSection   *************************************************
// *****************************************************************************
void Rtos::ExitCriticalSection()
{
  taskEXIT_CRITICAL();
}

// *****************************************************************************
// ***   DisableInterrupts   ***************************************************
// *****************************************************************************
void Rtos::DisableInterrupts()
{
  taskDISABLE_INTERRUPTS();
}

// *****************************************************************************
// ***   EnableInterrupts   ****************************************************
// *****************************************************************************
void Rtos::EnableInterrupts()
{
  taskENABLE_INTERRUPTS();
}
