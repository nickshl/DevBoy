//******************************************************************************
//  @file ExampleMsgTask.cpp
//  @author Nicolai Shlapunov
//
//  @details Application: Example Message Task Class, implementation
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

// *****************************************************************************
// ***   Includes   ************************************************************
// *****************************************************************************
#include "ExampleMsgTask.h"

// *****************************************************************************
// ***   Get Instance   ********************************************************
// *****************************************************************************
ExampleMsgTask& ExampleMsgTask::GetInstance(void)
{
   static ExampleMsgTask example_msg_task;
   return example_msg_task;
}

// *****************************************************************************
// ***   TimerExpired function   ***********************************************
// *****************************************************************************
Result ExampleMsgTask::TimerExpired()
{
  TaskQueueMsg msg;
  msg.type = TASK_TIMER_MSG;
  Result result = SendTaskMessage(&msg);
  return result;
}

// *****************************************************************************
// ***   ProcessMessage function   *********************************************
// *****************************************************************************
Result ExampleMsgTask::ProcessMessage()
{
  Result result = Result::ERR_NULL_PTR;

  switch(rcv_msg.type)
  {
    case TASK_TIMER_MSG:
      result = Result::RESULT_OK;
      break;

    default:
      result = Result::ERR_INVALID_ITEM;
      break;
  }

  return result;
}

