//******************************************************************************
//  @file Eeprom24.cpp
//  @author Nicolai Shlapunov
//
//  @details DevCore: EEPROM 24C*** driver, implementation
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
#include "Eeprom24.h"

#include "string.h" // for memcpy()

// *****************************************************************************
// ***   Public: Init   ********************************************************
// *****************************************************************************
Result Eeprom24::Init()
{
  Result result = Result::RESULT_OK;
  iic.SetTxTimeout(10U);
  iic.SetRxTimeout(100U);
  return result;
}

// *****************************************************************************
// ***   Public: Read   ********************************************************
// *****************************************************************************
Result Eeprom24::Read(uint16_t addr, uint8_t* rx_buf_ptr, uint16_t size)
{
  Result result = Result::ERR_NULL_PTR;

  // Check input parameters
  if(rx_buf_ptr != nullptr)
  {
    // Transfer: write two bytes address then read data
    result = iic.Transfer(I2C_ADDR, (uint8_t*)&addr, sizeof(addr), rx_buf_ptr, size);
  }

  return result;
}

// *****************************************************************************
// ***   Public: Write   *******************************************************
// *****************************************************************************
Result Eeprom24::Write(uint16_t addr, uint8_t* tx_buf_ptr, uint16_t size)
{
  Result result = Result::ERR_NULL_PTR;

  // Check input parameters
  if(tx_buf_ptr != nullptr)
  {
    // Clear result to enter in to cycle
    result = Result::RESULT_OK;
    // Allocate buffer for address + data
    uint8_t buf[2U + PAGE_SIZE_BYTES];
    // Cycle for write pages
    while(size && result.IsGood())
    {
      // Get data size
      uint8_t data_size = size < PAGE_SIZE_BYTES ? size : PAGE_SIZE_BYTES;
      // For the first page
      if((addr % PAGE_SIZE_BYTES) != 0U)
      {
        // Calculate data size from start address to the end of current page
        data_size = PAGE_SIZE_BYTES - (addr % PAGE_SIZE_BYTES);
        // If size less than remaining page bytes - use size
        data_size = size < data_size ? size : data_size;
      }
      // Decrease number of remaining bytes
      size -= data_size;
      // Store address
      *((uint16_t*)buf) = addr;
      // Copy data
      memcpy(buf + 2U, tx_buf_ptr, data_size);
      // Transfer
      result = iic.Write(I2C_ADDR, buf, 2U + data_size);

      // Wait until writing finished
      if(result.IsGood())
      {
        // Check device response
        result = iic.IsDeviceReady(I2C_ADDR);
        // Clear repetition counter for tracking timeout
        repetition_cnt = 0U;
        // Wait until write operation finished
        while(result.IsBad())
        {
          // Delay 1 ms for start writing
          RtosTick::DelayMs(1U);
          // Check is device ready
          result = iic.IsDeviceReady(I2C_ADDR);
          // Check timeout
          if(repetition_cnt > WRITING_TIMEOUT_MS)
          {
            result = Result::ERR_I2C_TIMEOUT;
            break;
          }
        }
      }
    }
  }

  return result;
}

