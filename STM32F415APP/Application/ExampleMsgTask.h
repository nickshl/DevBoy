//******************************************************************************
//  @file ExampleMsgTask.h
//  @author Nicolai Shlapunov
//
//  @details Application: Example Message Task Class, header
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

#ifndef MsgTaskTest_h
#define MsgTaskTest_h

// *****************************************************************************
// ***   Includes   ************************************************************
// *****************************************************************************
#include "DevCfg.h"
#include "AppTask.h"

// *****************************************************************************
// ***   Application Class   ***************************************************
// *****************************************************************************
class ExampleMsgTask : public AppTask
{
  public:
    // *************************************************************************
    // ***   Get Instance   ****************************************************
    // *************************************************************************
    static ExampleMsgTask& GetInstance(void);

    // *************************************************************************
    // ***   TimerExpired function   *******************************************
    // *************************************************************************
    virtual Result TimerExpired();

    // *************************************************************************
    // ***   ProcessMessage function   *****************************************
    // *************************************************************************
    virtual Result ProcessMessage();

  private:
    // Timer period
    static const uint32_t TASK_TIMER_PERIOD_MS = 1000U;

    // Task queue message types
    enum TaskQueueMsgType
    {
       TASK_TIMER_MSG
    };

    // Task queue message struct
    struct TaskQueueMsg
    {
      TaskQueueMsgType type;
    };

    // Buffer for received task message
    TaskQueueMsg rcv_msg;

    // *************************************************************************
    // ***   Private constructor   *********************************************
    // *************************************************************************
    ExampleMsgTask() : AppTask(EXAMPLE_MSG_TASK_STACK_SIZE, EXAMPLE_MSG_TASK_PRIORITY,
                               "ExampleMsgTask", 8U, sizeof(TaskQueueMsg), &rcv_msg,
                               TASK_TIMER_PERIOD_MS) {};
};

#endif
