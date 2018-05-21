//******************************************************************************
//  @file AppTask.h
//  @author Nicolai Shlapunov
//
//  @details DevCore: Application Task Base Class, header
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

#ifndef AppTask_h
#define AppTask_h

// *****************************************************************************
// ***   Includes   ************************************************************
// *****************************************************************************
#include "DevCfg.h"
#include "semphr.h"

// *****************************************************************************
// * AppTask class. This class is wrapper for call C++ function from class.
// *****************************************************************************
class AppTask
{
  public:
    // *************************************************************************
    // ***   Virtual destructor - prevent warning   ****************************
    // *************************************************************************
    virtual ~AppTask() {};

    // *************************************************************************
    // ***   Create task function   ********************************************
    // *************************************************************************
    // * This function creates new task in FreeRTOS, provide extern C 
    // * TaskFunWrapper() function and pointer to class as parameter. When 
    // * TaskFunWrapper() called from FreeRTOS, it use pointer to class from
    // * parameter to call virtual Task() function.
    void CreateTask(const char * const pcName,
                    const uint16_t usStackDepth,
                    UBaseType_t uxPriority);

    // *************************************************************************
    // ***   Setup function   **************************************************
    // *************************************************************************
    // * Empty function by default - some tasks may not have Setup() actions
    virtual void Setup(void *pvParameters) {};

    // *************************************************************************
    // ***   Loop function   ***************************************************
    // *************************************************************************
    // * Pure virtual Loop() function - each derived class must implement it
    virtual bool Loop(void *pvParameters) = 0;

  private:
};

#endif
