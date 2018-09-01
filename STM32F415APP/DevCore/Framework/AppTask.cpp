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
#include "RtosMutex.h"

// *****************************************************************************
// ***   Static variables   ****************************************************
// *****************************************************************************
static RtosMutex startup_mutex;
static uint32_t startup_cnt = 0U;

// *****************************************************************************
// ***   Create task function   ************************************************
// *****************************************************************************
void AppTask::CreateTask()
{
  Result result = Result::RESULT_OK;

  // If interval timer period isn't zero or task queue present
  if((timer.GetTimerPeriod() != 0U) || (task_queue.GetQueueLen() != 0U))
  {
    // Set Control Queue name
    ctrl_queue.SetName(task_name, "Ctrl");
    // Create control queue
    result = ctrl_queue.Create();
  }
  // If task queue present
  if(task_queue.GetQueueLen() != 0U)
  {
    // Set Task Queue name
    task_queue.SetName(task_name, "Task");
    // Create task queue
    result |= task_queue.Create();
  }
  // If interval timer period isn't zero
  if(timer.GetTimerPeriod() != 0U)
  {
    // Create timer
    result |= timer.Create();
  }
  // Create task: function - TaskFunctionCallback(), parameter - pointer to "this"
  result |= Rtos::TaskCreate(TaskFunctionCallback, task_name, stack_size, this, task_priority);

  // Check result
  if(result.IsBad())
  {
    // TODO: implement error handling
    Break();
  }
}

// *****************************************************************************
// ***   SendTaskMessage function   ********************************************
// *****************************************************************************
Result AppTask::SendTaskMessage(const void* task_msg, bool is_priority)
{
  Result result = Result::RESULT_OK;

  // Send task message to front or back of task queue
  if(is_priority == true)
  {
    result = task_queue.SendToFront(task_msg);
  }
  else
  {
    result = task_queue.SendToBack(task_msg);
  }

  // If successful - send message to the control queue
  if(result.IsGood())
  {
    CtrlQueueMsg ctrl_msg;
    ctrl_msg.type = CTRL_TASK_QUEUE_MSG;
    result = SendControlMessage(ctrl_msg, is_priority);
  }

  return result;
}

// *****************************************************************************
// ***   IntLoop function   ****************************************************
// *****************************************************************************
Result AppTask::IntLoop()
{
  Result result = Result::RESULT_OK;

  while(result.IsGood())
  {
    // Buffer for control message
    CtrlQueueMsg ctrl_msg;
    // Read on the control queue
    result = ctrl_queue.Receive(&ctrl_msg, timer.GetTimerPeriod() * 2U);
    // If successful
    if(result.IsGood())
    {
      // Check message type
      switch(ctrl_msg.type)
      {
        case CTRL_TIMER_MSG:
          result = TimerExpired();
          break;

         case CTRL_TASK_QUEUE_MSG:
         {
           // Non blocking read from the task queue
           result = task_queue.Receive(task_msg_ptr, 0U);
           // If successful
           if(result.IsGood())
           {
             // Process it!
             result = ProcessMessage();
           }
           break;
         }

         default:
           result = Result::ERR_INVALID_ITEM;
           break;
      }
    }
  }

  return result;
}

// *****************************************************************************
// ***   TaskFunctionCallback   ************************************************
// *****************************************************************************
void AppTask::TaskFunctionCallback(void* ptr)
{
  Result result = Result::ERR_NULL_PTR;

  if(ptr != nullptr)
  {
    // Set good result
    result = Result::RESULT_OK;
    // Get reference to the task object
    AppTask& app_task = *(static_cast<AppTask*>(ptr));

    // Increment counter before call Setup()
    ChangeCnt(true);
    // Call virtual Setup() function from AppTask class
    app_task.Setup();
    // Decrement counter after call Setup()
    ChangeCnt(false);
    // Pause for give other tasks run Setup()
    RtosTick::DelayTicks(1U);
    // Pause while other tasks run Setup() before executing any Loop()
    while(startup_cnt) RtosTick::DelayTicks(1U);

    // If no timer or queue - just call Loop() function
    if((app_task.timer.GetTimerPeriod() == 0U) && (app_task.task_queue.GetQueueLen() == 0U))
    {
      // Call virtual Loop() function from AppTask class
      while(app_task.Loop() == Result::RESULT_OK);
    }
    else
    {
      // Start task timer if needed
      if(app_task.timer.GetTimerPeriod() != 0U)
      {
        result = app_task.timer.Start();
      }
      // Check result
      if(result.IsGood())
      {
        // Call internal AppTask function
        result = app_task.IntLoop();
      }
      // Stop task timer if needed
      if(app_task.timer.GetTimerPeriod() != 0U)
      {
        result |= app_task.timer.Stop();
      }
    }
  }

  // Check result
  if(result.IsBad())
  {
    // TODO: implement error handling
    Break();
  }

  // Delete task after exit
  Rtos::TaskDelete();
}

// *****************************************************************************
// ***   TimerCallback function   **********************************************
// *****************************************************************************
void AppTask::TimerCallback(void* ptr)
{
  Result result = Result::ERR_NULL_PTR;

  if(ptr != nullptr)
  {
    // Get reference to the task object
    AppTask& task = *((AppTask*)ptr);

    // Create control timer message
    CtrlQueueMsg timer_msg;
    timer_msg.type = CTRL_TIMER_MSG;

    // Send message to the control queue
    result = task.SendControlMessage(timer_msg);
  }

  // Check result
  if(result.IsBad())
  {
    // TODO: implement error handling
    Break();
  }
}

// *****************************************************************************
// ***   SendControlMessage function   *****************************************
// *****************************************************************************
Result AppTask::SendControlMessage(const CtrlQueueMsg& ctrl_msg, bool is_priority)
{
  Result result;

  if(is_priority == true)
  {
    result = ctrl_queue.SendToFront(&ctrl_msg);
  }
  else
  {
    result = ctrl_queue.SendToBack(&ctrl_msg);
  }

  return result;
}

// *****************************************************************************
// ***   Change counter   ******************************************************
// *****************************************************************************
void AppTask::ChangeCnt(bool is_up)
{
  // Take semaphore before change counter
  startup_mutex.Lock();
  // Check direction
  if(is_up == true)
  {
    // Increment counter
    startup_cnt++;
  }
  else
  {
    // Decrement counter
    startup_cnt--;
  }
  // Give semaphore after changes
  startup_mutex.Release();
}
