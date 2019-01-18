//******************************************************************************
//  @file Mlx90614.cpp
//  @author Nicolai Shlapunov
//
//  @details DevCore: Bosch BME280 library, implementation
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
#include "Mlx90614.h"

// *****************************************************************************
// ***   Initialize   **********************************************************
// *****************************************************************************
Result Mlx90614::Initialize(uint8_t addr)
{
  Result result = Result::RESULT_OK;

  // Save I2C sensor address
  i2c_addr = addr;

  result = iic.Enable();

  // TODO: read data from sensor

  if(result.IsGood())
  {
    RtosTick::DelayMs(100U);
  }

  return result;
}

// ******************************************************************************
// ***   GetAmbientTemperature_x100   *******************************************
// ******************************************************************************
Result Mlx90614::GetAmbientTemperature_x100(int32_t& temp_x100)
{
  Result result = Result::RESULT_OK;

  // Clear temperature for clear upper bits
  temp_x100 = 0;
  // Read object temperature
  result = ReadReg(RAM_TA, *((uint16_t*)&temp_x100));
  // Normalize temperature
  temp_x100 *= 2; // Resolution is 0.02 degree C
  temp_x100 -= 27315; // Result in Kelvins, so convert it to Celsius

  return result;
}

// ******************************************************************************
// ***   GetObjectTemperature_x100   ********************************************
// ******************************************************************************
Result Mlx90614::GetObjectTemperature_x100(int32_t& temp_x100, ObjectType obj)
{
  Result result = Result::RESULT_OK;

  // Clear temperature for clear upper bits
  temp_x100 = 0;
  // Read object temperature
  result = ReadReg((obj == OBJECT1) ? RAM_TOBJ1 : RAM_TOBJ2, *((uint16_t*)&temp_x100));
  // Normalize temperature
  temp_x100 *= 2; // Resolution is 0.02 degree C
  temp_x100 -= 27315; // Result in Kelvins, so convert it to Celsius

  return result;
}

// ******************************************************************************
// ***   Read register value(16-bit unsigned)   *********************************
// ******************************************************************************
Result Mlx90614::ReadReg(uint8_t reg, uint16_t& value)
{
  Result result = Result::RESULT_OK;

  // Read data
  result = iic.Transfer(i2c_addr, &reg, sizeof(reg), (uint8_t*)&value, sizeof(value));

  // If the read succeeded, check error flag
  if (value & 0x8000)
  {
    value = 0U;
    result = Result::ERR_BUSY;
  }

  return result;
}

// ******************************************************************************
// ***   Write register value(16-bit signed)   **********************************
// ******************************************************************************
Result Mlx90614::WriteReg(uint8_t reg, uint16_t value)
{
  Result result = Result::RESULT_OK;
//  uint8_t buf[2];
//  buf[0] = reg;
//  buf[1] = value;
//  return iic.Write(i2c_addr, buf, sizeof(buf));
  return result;
}
