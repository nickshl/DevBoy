//******************************************************************************
//  @file GraphDemo.h
//  @details Application: Gario Application Class, header
//******************************************************************************

#ifndef Gario_h
#define Gario_h

// *****************************************************************************
// ***   Includes   ************************************************************
// *****************************************************************************
#include "DevCfg.h"
#include "AppTask.h"
#include "DisplayDrv.h"
#include "InputDrv.h"
#include "SoundDrv.h"
#include "UiEngine.h"

// *****************************************************************************
// ***   Local const variables   ***********************************************
// *****************************************************************************

// Tick for delay. For proper work, delay should be provide movement not more
// one pixel at a tick. Otherwise collision check system will worn improperly.
static const int32_t TICK_MS = 5;

// Y axis acceleration constants
static const int32_t Y_ACCEL_S = 1200;
static const int32_t Y_ACCEL_CONST = (Y_ACCEL_S * (TICK_MS * TICK_MS)) / 2;
static const int32_t JUMP_SPEED = Y_ACCEL_CONST * 500/TICK_MS;

// X axis speed constants
static const int32_t X_SPEED_MIN_S = 100;
static const int32_t X_SPEED_MAX_S = 200;
static const int32_t X_SPEED_MIN = X_SPEED_MIN_S * TICK_MS;
static const int32_t X_SPEED_MAX = X_SPEED_MAX_S * TICK_MS;

// Animation speed should be divisible by TICK_MS
static const int32_t X_SPRITE_ANIM_SPEED = (100 / TICK_MS) * TICK_MS;

// *****************************************************************************
// ***   Defines   *************************************************************
// *****************************************************************************
#define BG_Z (100)

// *****************************************************************************
// ***   Application Class   ***************************************************
// *****************************************************************************
class Gario : public AppTask
{
  public:
    // *************************************************************************
    // ***   Get Instance   ****************************************************
    // *************************************************************************
    static Gario& GetInstance(void);

    // *************************************************************************
    // ***   Application Loop   ************************************************
    // *************************************************************************
    virtual Result Loop();

  private:
    // Display driver instance
    DisplayDrv& display_drv = DisplayDrv::GetInstance();
    // Input driver instance
    InputDrv& input_drv = InputDrv::GetInstance();
    // Sound driver instance
    SoundDrv& sound_drv = SoundDrv::GetInstance();

    // Time variable
    uint32_t time_ms = 0U;

    // *************************************************************************
    // ***   Private constructor   *********************************************
    // *************************************************************************
    Gario() : AppTask(APPLICATION_TASK_STACK_SIZE, APPLICATION_TASK_PRIORITY,
                      "Gario") {};
};

// *****************************************************************************
// ***   Gario Sprite Class   **************************************************
// *****************************************************************************
class GarioSprite : public Image
{
  public:
    // *************************************************************************
    // ***   Constructor   *****************************************************
    // *************************************************************************
    GarioSprite(int32_t x, int32_t y, TiledMap& tiled_map_t);

    // *************************************************************************
    // ***   Process function   ************************************************
    // *************************************************************************
    bool Process(int32_t dx, int32_t dy, int32_t tick_ms, int32_t time_ms);

    // *************************************************************************
    // ***   Jump function   ***************************************************
    // *************************************************************************
    void Jump() {y_speed = -JUMP_SPEED; y_speed_rest = 0; jump = true;}
    
    // *************************************************************************
    // ***   Die function   ****************************************************
    // *************************************************************************
    void Die() {is_die = true; time_to_die = 2000;}
    
    // *************************************************************************
    // ***   Is alive function   ***********************************************
    // *************************************************************************
    bool IsAlive() {return !is_die;}

  private:
    // Tile map
    TiledMap& tile_map;
    // Die flag
    bool is_die = false;
    // Time to die
    int32_t time_to_die = 0;
    // Variable for switch frames
    int32_t move_idx = 0;
    // X position of sprite in the tilemap
    int32_t x_map_pos = 0;
    // Horizontal speed
    int32_t x_speed = 0;
    // Rest of speed(hi-res)
    int32_t x_speed_rest = 0;
    // Y position of sprite in the tilemap
    int32_t y_map_pos = 0;
    // Vertical speed
    int32_t y_speed = 0;
    // Rest of speed(hi-res)
    int32_t y_speed_rest = 0;
    // Jump after enemy
    bool jump = false;
};

// *****************************************************************************
// ***   Enemy Sprite Class   **************************************************
// *****************************************************************************
class EnemySprite : public Image
{
  public:
    // *************************************************************************
    // ***   Constructor   *****************************************************
    // *************************************************************************
    EnemySprite(int32_t x, int32_t y, TiledMap& tiled_map_t);

    // *************************************************************************
    // ***   Process function   ************************************************
    // *************************************************************************
    void Process(int32_t dx, int32_t dy, int32_t tick_ms, int32_t time_ms);

    // *************************************************************************
    // ***   Die function   ****************************************************
    // *************************************************************************
    void Die() {is_die = true; time_to_die = 2000;}
    
    // *************************************************************************
    // ***   Is alive function   ***********************************************
    // *************************************************************************
    bool IsAlive() {return !is_die;}

  private:
    // Tile map
    TiledMap& tile_map;
    // Variable for switch frames
    int32_t move_idx = 0;
    // Active flag
    bool is_active = false;
    // Die flag
    bool is_die = false;
    // Time to die
    int32_t time_to_die = 0;
    // X position of sprite in the tilemap
    int32_t x_map_pos = 0;
    // Horizontal speed
    int32_t x_speed = -X_SPEED_MIN/2;
    // Rest of speed(hi-res)
    int32_t x_speed_rest = 0;
    // Y position of sprite in the tilemap
    int32_t y_map_pos = 0;
    // Vertical speed
    int32_t y_speed = 0;
    // Rest of speed(hi-res)
    int32_t y_speed_rest = 0;
};

#endif
