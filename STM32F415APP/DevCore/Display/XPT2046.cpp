//******************************************************************************
//  @file XPT2046.cpp
//  @author Nicolai Shlapunov
//
//  @details DevCore: XPT2046 Low Level Driver Class, implementation
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
#include <XPT2046.h>

// *****************************************************************************
// ***   Init touchscreen   ****************************************************
// *****************************************************************************
void XPT2046::Init(void)
{
  // Pull down CS
  HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_RESET);
  // Send ON command
  SpiWrite(TON);
  // Send empty byte for skip answer
  SpiWrite(EMP);
  // Send empty byte for skip answer
  SpiWrite(EMP);
  // Pull up CS
  HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_SET);
}

// *****************************************************************************
// ***   If touched - return true.   *******************************************
// *****************************************************************************
bool XPT2046::IsTouch(void)
{
  // Check T_IRQ input and return state
  return(HAL_GPIO_ReadPin(T_IRQ_GPIO_Port, T_IRQ_Pin) == GPIO_PIN_RESET);
}

// *****************************************************************************
// ***   Get X and Y coordinates. If touched - return true.   ******************
// *****************************************************************************
bool XPT2046::GetRawXY(int32_t& x, int32_t& y)
{
  // Return value
  bool ret = false;
  // If touch present
  if(HAL_GPIO_ReadPin(T_IRQ_GPIO_Port, T_IRQ_Pin) == GPIO_PIN_RESET)
  {
    // Pull down CS
    HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_RESET);
    // Request X coordinate
    SpiWrite(CHX);
    // Receive High byte for X
    x = SpiWriteRead(EMP) << 8;
    // Receive Low byte for X
    x |= SpiWriteRead(EMP);
    // Shift, because result have only 12 bits, 3 because answer started from
    // second rise edge
    x >>= 3;
    // Pull up CS
    HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_SET);

    // Pull down CS
    HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_RESET);
    // Request Y coordinate
    SpiWrite(CHY);
    // Receive High byte for Y
    y = SpiWriteRead(EMP) << 8;
    // Receive Low byte for Y
    y |= SpiWriteRead(EMP);
    // Shift, because result have only 12 bits, 3 because answer started from
    // second rise edge
    y >>= 3;
    // Pull up CS
    HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_SET);

    // Touch present
    ret = true;
  }
  // Return result
  return ret;
}

// *****************************************************************************
// ***   Get X and Y coordinates. If touched - return true.   ******************
// *****************************************************************************
bool XPT2046::GetXY(int32_t& x, int32_t& y)
{
  // Return value
  bool ret = GetRawXY(x, y);
  // If touch present
  if(ret)
  {
    // Calculate X
    x = ((x * COEF) / kx) + bx;
    // Calculate Y
    y = ((y * COEF) / ky) + by;
  }
  // Return touch state
  return ret;
}

// *****************************************************************************
// ***   SetCalibrationConsts   ************************************************
// *****************************************************************************
void XPT2046::SetCalibrationConsts(int32_t nkx, int32_t nky, int32_t nbx, int32_t nby)
{
  // Save calibration constants
  kx = nkx;
  ky = nky;
  bx = nbx;
  by = nby;
}

// *****************************************************************************
// ***   Write byte to SPI   ***************************************************
// *****************************************************************************
inline void XPT2046::SpiWrite(uint8_t c)
{
  // Call HAL function for send byte by SPI
  (void) HAL_SPI_Transmit(hspi, &c, sizeof(c), 1U);
}

// *****************************************************************************
// ***   Write and read byte to/from SPI   *************************************
// *****************************************************************************
inline uint8_t XPT2046::SpiWriteRead(uint8_t c)
{
  // Temporary variable for receive byte
  uint8_t rcv;
  // Call HAL function for send/receive byte by SPI
  (void) HAL_SPI_TransmitReceive(hspi, &c, &rcv, sizeof(uint8_t), 1U);
  // Return received byte
  return rcv;
}
