//******************************************************************************
//  @file DisplayDrv.h
//  @author Nicolai Shlapunov
//
//  @details DevCore: Display Driver Class, header
//
//  @section LICENSE
//
//   Software License Agreement (BSD License)
//
//   Copyright (c) 2016, Devtronic & Nicolai Shlapunov
//   All rights reserved.
//
//   Redistribution and use in source and binary forms, with or without
//   modification, are permitted provided that the following conditions are met:
//   1. Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//   2. Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//   3. Neither the name of the Devtronic nor the names of its contributors
//      may be used to endorse or promote products derived from this software
//      without specific prior written permission.
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
//******************************************************************************

#ifndef DisplayDrv_h
#define DisplayDrv_h

// *****************************************************************************
// ***   Includes   ************************************************************
// *****************************************************************************
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "DevCfg.h"
#include "AppTask.h"
#include "RtosMutex.h"
#include "RtosSemaphore.h"

#include "ILI9341.h"
#include "XPT2046.h"
#include "VisObject.h"
#include "Primitives.h"
#include "Strings.h"
#include "Image.h"
#include "TiledMap.h"

// *****************************************************************************
// ***   Display Driver Class   ************************************************
// *****************************************************************************
class DisplayDrv : public AppTask
{
  public:
    // *************************************************************************
    // ***   Get Instance   ****************************************************
    // *************************************************************************
    static DisplayDrv& GetInstance(void);

    // *************************************************************************
    // ***   Display Driver Setup   ********************************************
    // *************************************************************************
    virtual Result Setup();

    // *************************************************************************
    // ***   Display Driver Loop   *********************************************
    // *************************************************************************
    virtual Result Loop();

    // *************************************************************************
    // ***   Add Visual Object to object list   ********************************
    // *************************************************************************
    Result AddVisObjectToList(VisObject* obj, uint32_t z);

    // *************************************************************************
    // ***   Delete Visual Object from object list   ***************************
    // *************************************************************************
    Result DelVisObjectFromList(VisObject* obj);

    // *************************************************************************
    // ***   Lock display   ****************************************************
    // *************************************************************************
    Result LockDisplay(uint32_t wait_ms = portMAX_DELAY);

    // *************************************************************************
    // ***   Unlock display   **************************************************
    // *************************************************************************
    Result UnlockDisplay(void);

    // *************************************************************************
    // ***   Lock display line   ***********************************************
    // *************************************************************************
    Result LockDisplayLine(uint32_t wait_ms = portMAX_DELAY);

    // *************************************************************************
    // ***   Unlock display line   *********************************************
    // *************************************************************************
    Result UnlockDisplayLine(void);

    // *************************************************************************
    // ***   Update display   **************************************************
    // *************************************************************************
    Result UpdateDisplay(void);

    // *************************************************************************
    // ***   Set Update Mode   *************************************************
    // *************************************************************************
    void SetUpdateMode(bool is_vertical = false);

    // *************************************************************************
    // ***   GetScreenW   ******************************************************
    // *************************************************************************
    inline int32_t GetScreenW(void) {return width;}

    // *************************************************************************
    // ***   GetScreenH   ******************************************************
    // *************************************************************************
    inline int32_t GetScreenH(void) {return height;}

    // *************************************************************************
    // ***   Get Touch X and Y coordinate   ************************************
    // *************************************************************************
    bool GetTouchXY(int32_t& x, int32_t& y);

    // *************************************************************************
    // ***   Check touch   *****************************************************
    // *************************************************************************
    bool IsTouch();

    // *************************************************************************
    // ***   Calibrate Touchscreen   *******************************************
    // *************************************************************************
    void TouchCalibrate();

  private:
    // Display FPS/touch coordinates
    static const bool DISPLAY_DEBUG_INFO = true;
    
    // Display driver object
    ILI9341 tft = TFT_HSPI;
    // Display SPI handle
    SPI_HandleTypeDef* tft_hspi = TFT_HSPI;

    // Touchscreen driver object
    XPT2046 touch = TOUCH_HSPI;
    // Touchscreen SPI handle
    SPI_HandleTypeDef* touch_hspi = TOUCH_HSPI;

    // Pointer to first object in list
    VisObject* object_list = nullptr;
    // Pointer to last object in list
    VisObject* object_list_last = nullptr;

    // Update mode: true - vertical, false = horizontal
    bool update_mode = false;
    // Variables for update screen mode
    int32_t width = 0;
    int32_t height = 0;
    // Double Screen Line buffer
    uint16_t scr_buf[2][ILI9341::GetMaxLine()];

    // Touch coordinates and state
    bool is_touch = false;
    int32_t tx = 0;
    int32_t ty = 0;

    // FPS multiplied to 10
    volatile uint32_t fps_x10 = 0U;
    // Buffer for print FPS string
    char str[32] = {"       "};
    // FPS string
    String fps_str;

    // Semaphore for update screen
    RtosSemaphore screen_update;
    // Mutex to synchronize when drawing lines
    RtosMutex line_mutex;
    // Mutex to synchronize when drawing frames
    RtosMutex frame_mutex;
    // Mutex for synchronize when reads touch coordinates
    RtosMutex touchscreen_mutex;

    // *************************************************************************
    // ** Private constructor. Only GetInstance() allow to access this class. **
    // *************************************************************************
    DisplayDrv() : AppTask(DISPLAY_DRV_TASK_STACK_SIZE, DISPLAY_DRV_TASK_PRIORITY,
                           "DisplayDrv") {};
};

#endif
