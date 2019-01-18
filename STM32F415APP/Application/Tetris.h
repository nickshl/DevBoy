//******************************************************************************
//  @file Tetris.h
//  @author Nicolai Shlapunov
//
//  @details Application: Tetris Application Class, header
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

#ifndef Tetris_h
#define Tetris_h

// *****************************************************************************
// ***   Includes   ************************************************************
// *****************************************************************************
#include "DevCfg.h"
#include "AppTask.h"
#include "DisplayDrv.h"
#include "InputDrv.h"
#include "SoundDrv.h"

// *****************************************************************************
// ***   Local const variables   ***********************************************
// *****************************************************************************

// *****************************************************************************
// ***   Defines   *************************************************************
// *****************************************************************************
// Bucket width(with border)
static const int32_t WIDTH = 12;
// Bucket height(with low border)
static const int32_t HEIGHT = 24;
// Cube size in pixels
static const int32_t CUBE_SIZE = 10;

// Colors for shapes
static const uint16_t colors[8] = {COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_MAGENTA, COLOR_YELLOW, COLOR_CYAN, COLOR_DARKGREY};

// *****************************************************************************
// ***   Application Class   ***************************************************
// *****************************************************************************
class Tetris : public AppTask
{
  public:
	// *************************************************************************
    // ***   Get Instance   ****************************************************
    // *************************************************************************
    static Tetris& GetInstance(void);

    // *************************************************************************
    // ***   Tetris Loop   *****************************************************
    // *************************************************************************
    virtual Result Loop();

  private:
    // Game over flag
    bool game_over = false;
    // Round flag
    bool round = true;

    // Last left encoder value
    int32_t last_enc1_val = 0;
    // Last right encoder value
    int32_t last_enc2_val = 0;
    // Last left encoder button state
    bool enc1_btn_val = false;
    // Last left encoder button state
    bool enc1_btn_back_val = false;
    // Last right encoder button back state
    bool enc2_btn_val = false;
    // Last right encoder button back state
    bool enc2_btn_back_val = false;

    // Display driver instance
    DisplayDrv& display_drv = DisplayDrv::GetInstance();
    // Input driver instance
    InputDrv& input_drv = InputDrv::GetInstance();
    // Sound driver instance
    SoundDrv& sound_drv = SoundDrv::GetInstance();
    
    // *************************************************************************
    // ** Private constructor. Only GetInstance() allow to access this class. **
    // *************************************************************************
    Tetris() : AppTask(APPLICATION_TASK_STACK_SIZE, APPLICATION_TASK_PRIORITY,
                       "Tetris") {};
};

// *****************************************************************************
// ***   TetrisShape Class   ***************************************************
// *****************************************************************************
class TetrisShape : public VisObject
{
  public:
    // *************************************************************************
    // ***   Constructor   *****************************************************
    // *************************************************************************
    TetrisShape();

    // *************************************************************************
    // ***   RotateShape   *****************************************************
    // *************************************************************************
    void RotateShape(int8_t dir);

    // *************************************************************************
    // ***   DrawShapeIntoBuffer   *********************************************
    // *************************************************************************
    void DrawShapeIntoBuffer(uint8_t * buf, int32_t width, int32_t height);

    // *************************************************************************
    // ***   CheckShapeCollisionIntoBuffer   ***********************************
    // *************************************************************************
    bool CheckShapeCollisionIntoBuffer(uint8_t * buf, int32_t width, int32_t height);

    // *************************************************************************
    // ***   PopulateShapeArray   **********************************************
    // *************************************************************************
    void PopulateShapeArray(uint8_t shape_type, uint8_t color_idx);

    // *************************************************************************
    // ***   PopulateShapeArray   **********************************************
    // *************************************************************************
    void PopulateShapeArray(TetrisShape& shape);

    // *************************************************************************
    // ***   Put line in buffer   **********************************************
    // *************************************************************************
    void DrawInBufW(uint16_t* buf, int32_t n, int32_t line, int32_t start_x);

    // *************************************************************************
    // ***   Put line in buffer   **********************************************
    // *************************************************************************
    void DrawInBufH(uint16_t* buf, int32_t n, int32_t row, int32_t start_y) {};

    // Public variables for access from Tetris class

    // Shape X position on screen
    int8_t shapeTopLeftX = 0;
    // Shape Y position on screen
    int8_t shapeTopLeftY= 0;

  private:
    // Current shape
    bool shapeArray[4*4] = {false};
    // Current number of shape
    int8_t shapeNum = 0;
    // Current rotation index of shape
    int8_t shapeRotate = 0;
    // Current rotation index of shape
    int8_t shapeColorIdx = 0;

    // Static array contains all shapes
    static const bool shapesArray[7][4*4];
    // Static shape rotating matrixes
    static const int8_t shapeRtArray[4][4*4];
};

// *****************************************************************************
// ***   TetrisBucket Class   **************************************************
// *****************************************************************************
class TetrisBucket : public VisObject
{
  public:
    // *************************************************************************
    // ***   Constructor   *****************************************************
    // *************************************************************************
    TetrisBucket();

    // *************************************************************************
    // ***   CheckShapeCollisionIntoBucket   ***********************************
    // *************************************************************************
    bool CheckShapeCollisionIntoBucket(TetrisShape& ts);

    // *************************************************************************
    // ***   PutShapeIntoBucket   **********************************************
    // *************************************************************************
    void PutShapeIntoBucket(TetrisShape& ts);

    // *************************************************************************
    // ***   RemoveFullLines   *************************************************
    // *************************************************************************
    void RemoveFullLines();

    // *************************************************************************
    // ***   InitBucket   ******************************************************
    // *************************************************************************
    void InitBucket();

    // *************************************************************************
    // ***   GetScore   ********************************************************
    // *************************************************************************
    uint32_t GetScore() {return score;}

    // *************************************************************************
    // ***   Put line in buffer   **********************************************
    // *************************************************************************
    void DrawInBufW(uint16_t* buf, int32_t n, int32_t line, int32_t start_x);

    // *************************************************************************
    // ***   Put line in buffer   **********************************************
    // *************************************************************************
    void DrawInBufH(uint16_t* buf, int32_t n, int32_t row, int32_t start_y) {};

private:
  // Bucket buffer
  uint8_t bucket[WIDTH*HEIGHT] = {0U};
  // Score. Calculated in bucket, in function RemoveFullLines().
  uint32_t score = 0U;

  // ***************************************************************************
  // ***   Copy from end to start(for overlapped blocks)   *********************
  // ***************************************************************************
  static void memcpyback(uint8_t * dst, uint8_t * src, int32_t num);
};

#endif
