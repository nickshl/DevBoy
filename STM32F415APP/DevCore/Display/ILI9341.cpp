//******************************************************************************
//  @file ILI9341.cpp
//  @author Nicolai Shlapunov
//
//  @details DevCore: ILI9341 Low Level Driver Class, implementation
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
#include "ILI9341.h"

// *****************************************************************************
// ***   Defines   *************************************************************
// *****************************************************************************

// Commands definitions

#define CMD_NOP        0x00 // No Operation
#define CMD_SWRESET    0x01 // Software Reset
#define CMD_RDDID      0x04 // Read Display Identification Information
#define CMD_RDDST      0x09 // Read Display Status

#define CMD_RDMODE     0x0A // Read Display Power Mode
#define CMD_RDMADCTL   0x0B // Read Display MADCTL
#define CMD_RDPIXFMT   0x0C // Read Display Pixel Format
#define CMD_RDIMGFMT   0x0D // Read Display Image Format
#define CMD_RDSIGMOD   0x0E // Read Display Signal Mode
#define CMD_RDSELFDIAG 0x0F // Read Display Self-Diagnostic Result

#define CMD_SLPIN      0x10 // Enter Sleep Mode
#define CMD_SLPOUT     0x11 // Sleep OUT
#define CMD_PTLON      0x12 // Partial Mode ON
#define CMD_NORON      0x13 // Normal Display Mode ON

#define CMD_INVOFF     0x20 // Display Inversion OFF
#define CMD_INVON      0x21 // Display Inversion ON
#define CMD_GAMMASET   0x26 // Gamma Set
#define CMD_DISPOFF    0x28 // Display OFF
#define CMD_DISPON     0x29 // Display ON

#define CMD_CASET      0x2A // Column Address Set
#define CMD_PASET      0x2B // Page Address Set
#define CMD_RAMWR      0x2C // Memory Write
#define CMD_GSET       0x2D // Color SET
#define CMD_RAMRD      0x2E // Memory Read

#define CMD_PTLAR      0x30 // Partial Area
#define CMD_VSCRDEF    0x33 // Vertical Scrolling Definition
#define CMD_TELOFF     0x34 // Tearing Effect Line OFF
#define CMD_TELON      0x35 // Tearing Effect Line ON
#define CMD_MADCTL     0x36 // Memory Access Control
#define CMD_VSAADDR    0x37 // Vertical Scrolling Start Address
#define CMD_IDLMOFF    0x38 // Idle Mode OFF
#define CMD_IDLMON     0x39 // Idle Mode ON
#define CMD_PIXFMT     0x3A // Pixel Format Set

#define CMD_RGBISC     0xB0 // RGB Interface Signal Control
#define CMD_FRMCTR1    0xB1 // Frame Control (In Normal Mode)
#define CMD_FRMCTR2    0xB2 // Frame Control (In Idle Mode)
#define CMD_FRMCTR3    0xB3 // Frame Control (In Partial Mode)
#define CMD_INVCTR     0xB4 // Display Inversion Control
#define CMD_BLKPC      0xB5 // Blanking Porch Control
#define CMD_DFUNCTR    0xB6 // Display Function Control

#define CMD_PWCTR1     0xC0 // Power Control 1
#define CMD_PWCTR2     0xC1 // Power Control 2
#define CMD_VMCTR1     0xC5 // VCOM Control 1
#define CMD_VMCTR2     0xC7 // VCOM Control 2
#define CMD_PWCTRA     0xCB // Power control A
#define CMD_PWCTRB     0xCF // Power control B

#define CMD_NVMEMWR    0xD0 // NV Memory Write
#define CMD_NVMEMPK    0xD1 // NV Memory Protection Key
#define CMD_NVMEMSR    0xD2 // NV Memory Status Read
#define CMD_READID4    0xD3 // Read ID4

#define CMD_RDID1      0xDA // Read ID1
#define CMD_RDID2      0xDB // Read ID2
#define CMD_RDID3      0xDC // Read ID3

#define CMD_GMCTRP1    0xE0 // Positive Gamma Correction
#define CMD_GMCTRN1    0xE1 // Negative Gamma Correction
#define CMD_DGCTRL1    0xE2 // Digital Gamma Control 1
#define CMD_DGCTRL2    0xE3 // Digital Gamma Control 2
#define CMD_DRVTMCA    0xE8 // Driver timing control A
#define CMD_DRVTMCB    0xEA // Driver timing control B
#define CMD_PWONSC     0xED // Power on sequence control

#define CMD_EN3G       0xF2 // Enable 3 gamma control
#define CMD_INTCTRL    0xF6 // Interface Control
#define CMD_PUMPRC     0xF7 // Pump ratio control

// Memory Access Control register bits definitions

#define MADCTL_MY  0x80 // Row Address Order
#define MADCTL_MX  0x40 // Column Address Order
#define MADCTL_MV  0x20 // Row / Column Exchange
#define MADCTL_ML  0x10 // Vertical Refresh Order
#define MADCTL_BGR 0x08 // BGR Order
#define MADCTL_RGB 0x00 // RGB Order (No BGR bit)
#define MADCTL_MH  0x04 // Horizontal Refresh ORDER

// *****************************************************************************
// ***   Constructor   *********************************************************
// *****************************************************************************
ILI9341::ILI9341(SPI_HandleTypeDef* in_hspi) : hspi(in_hspi) {};

// *****************************************************************************
// ***   Write byte to SPI   ***************************************************
// *****************************************************************************
inline void ILI9341::SpiWrite(uint8_t c)
{
  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // Pull down CS
  HAL_SPI_Transmit(hspi, &c, sizeof(c), 1U);
  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // Pull up CS
}

// *****************************************************************************
// ***   Write byte stream to SPI   ********************************************
// *****************************************************************************
void ILI9341::SpiWriteStream(uint8_t* data, uint32_t n)
{
  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // Pull down CS
  HAL_SPI_Transmit_DMA(hspi, data, n);
}

// *****************************************************************************
// ***   Write command to SPI   ************************************************
// *****************************************************************************
inline void ILI9341::WriteCommand(uint8_t c)
{
  HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET); // Command
  SpiWrite(c);
}

// *****************************************************************************
// ***   Write data to SPI   ***************************************************
// *****************************************************************************
inline void ILI9341::WriteData(uint8_t c)
{
  HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET); // Data
  SpiWrite(c);
} 

// *****************************************************************************
// ***   Write data steram to SPI   ********************************************
// *****************************************************************************
void ILI9341::WriteDataStream(uint8_t* data, uint32_t n)
{
  // Data
  HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
  // Pull down CS
  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
  // Send data to screen
  HAL_SPI_Transmit_DMA(hspi, data, n);
}

// *****************************************************************************
// ***   Check SPI transfer status   *******************************************
// *****************************************************************************
bool ILI9341::IsTransferComplete(void)
{
  return (hspi->State != HAL_SPI_STATE_BUSY_TX);
//  return (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE) == SET);
}

// *****************************************************************************
// ***   Pull up CS line for LCD  **********************************************
// *****************************************************************************
void ILI9341::StopTransfer(void)
{
  // Pull up CS
  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

// *****************************************************************************
// ***   Init screen   *********************************************************
// *****************************************************************************
void ILI9341::Init(void)
{
//  // Reset sequence. Used only if GPIO pin used as LCD reset.
//  HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);  
//  HAL_Delay(5);
//  HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);  
//  HAL_Delay(20);
//  HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);  
//  HAL_Delay(150);

  // Exit Sleep
  WriteCommand(CMD_SWRESET);
  // Delay for execute previous command
  HAL_Delay(100U);

  // Power control
  WriteCommand(CMD_PWCTR1);
  WriteData(0x23); // VRH[5:0] // 25

  // Power control
  WriteCommand(CMD_PWCTR2);
  WriteData(0x10); // SAP[2:0]; BT[3:0] // 11

  // VCM control 1
  WriteCommand(CMD_VMCTR1);
  WriteData(0x2B);
  WriteData(0x2B);

  // VCM control 2 
  WriteCommand(CMD_VMCTR2);
  WriteData(0xC0);

  // Pixel Format Set
  WriteCommand(CMD_PIXFMT);
  WriteData(0x55);

  // Frame Control (In Normal Mode)
  WriteCommand(CMD_FRMCTR1);
  WriteData(0x00);
  WriteData(0x18);

  // Power control A
  WriteCommand(CMD_PWCTRA);
  WriteData(0x39);
  WriteData(0x2C);
  WriteData(0x00);
  WriteData(0x34);
  WriteData(0x02);

  // Power control B
  WriteCommand(CMD_PWCTRB);
  WriteData(0x00);
  WriteData(0XC1);
  WriteData(0X30);

  // Power on sequence control
  WriteCommand(CMD_PWONSC);
  WriteData(0x64);
  WriteData(0x03);
  WriteData(0X12);
  WriteData(0X81);

  // Driver timing control A
  WriteCommand(CMD_DRVTMCA);
  WriteData(0x85);
  WriteData(0x00);
  WriteData(0x78);

  // Driver timing control B
  WriteCommand(CMD_DRVTMCB);
  WriteData(0x00);
  WriteData(0x00);

  // Pump ratio control
  WriteCommand(CMD_PUMPRC);
  WriteData(0x20);

  // Memory Access Control
  WriteCommand(CMD_MADCTL);
  WriteData(0x48);

  // Display Function Control 
  WriteCommand(CMD_DFUNCTR);
  WriteData(0x08);
  WriteData(0x82);
  WriteData(0x27);

  // Enable 3 gamma control - Disable 3 Gamma Function 
  WriteCommand(CMD_EN3G);
  WriteData(0x00);

  // Gamma Set - Gamma curve selected
  WriteCommand(CMD_GAMMASET);
  WriteData(0x01);

  // Positive Gamma Correction
  WriteCommand(CMD_GMCTRP1);
  WriteData(0x0F);
  WriteData(0x31);
  WriteData(0x2B);
  WriteData(0x0C);
  WriteData(0x0E);
  WriteData(0x08);
  WriteData(0x4E);
  WriteData(0xF1);
  WriteData(0x37);
  WriteData(0x07);
  WriteData(0x10);
  WriteData(0x03);
  WriteData(0x0E);
  WriteData(0x09);
  WriteData(0x00);

  // Negative Gamma Correction
  WriteCommand(CMD_GMCTRN1);
  WriteData(0x00);
  WriteData(0x0E);
  WriteData(0x14);
  WriteData(0x03);
  WriteData(0x11);
  WriteData(0x07);
  WriteData(0x31);
  WriteData(0xC1);
  WriteData(0x48);
  WriteData(0x08);
  WriteData(0x0F);
  WriteData(0x0C);
  WriteData(0x31);
  WriteData(0x36);
  WriteData(0x0F);

  // Interface Control
  WriteCommand(CMD_INTCTRL);
  WriteData(0x01);
  WriteData(0x00);
  WriteData(0x01 << 5);

  // Exit Sleep
  WriteCommand(CMD_SLPOUT);
  // Delay for execute previous command
  HAL_Delay(120U);
  // Display on
  WriteCommand(CMD_DISPON);
}

// *****************************************************************************
// ***   Set output window   ***************************************************
// *****************************************************************************
void ILI9341::SetAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  WriteCommand(CMD_CASET); // Column address set
  WriteData(x0 >> 8);
  WriteData(x0 & 0xFF); // XSTART 
  WriteData(x1 >> 8);
  WriteData(x1 & 0xFF); // XEND

  WriteCommand(CMD_PASET); // Row address set
  WriteData(y0 >> 8);
  WriteData(y0);        // YSTART
  WriteData(y1 >> 8);
  WriteData(y1);        // YEND

  WriteCommand(CMD_RAMWR); // write to RAM
  
  // Prepare for write data
  HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET); // Data
}

// *****************************************************************************
// ***   Pass 8-bit (each) R,G,B, get back 16-bit packed color   ***************
// *****************************************************************************
uint16_t ILI9341::GetColor565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// *****************************************************************************
// ***   Set screen orientation   **********************************************
// *****************************************************************************
void ILI9341::SetRotation(uint8_t m)
{
  WriteCommand(CMD_MADCTL);
  rotation = m % 4; // can't be higher than 3
  switch (rotation)
  {
    case 0:
      WriteData(MADCTL_BGR);
      width  = TFT_HEIGHT;
      height = TFT_WIDTH;
      break;

    case 1:
      WriteData(MADCTL_MV | MADCTL_BGR);
      width  = TFT_WIDTH;
      height = TFT_HEIGHT;
      break;

    case 2: // Y: up -> down 
      WriteData(MADCTL_MX | MADCTL_MY | MADCTL_BGR);
      width  = TFT_HEIGHT;
      height = TFT_WIDTH;
      break;

    case 3: // X: left -> right 
      WriteData(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
      width  = TFT_WIDTH;
      height = TFT_HEIGHT;
      break;

    default:
      break;
  }
}

// *****************************************************************************
// ***   Write color to screen   ***********************************************
// *****************************************************************************
void ILI9341::PushColor(uint16_t color)
{
  HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET); // Data

  SpiWrite(color >> 8);
  SpiWrite(color);
}

// *****************************************************************************
// ***   Draw one pixel on  screen   *******************************************
// *****************************************************************************
void ILI9341::DrawPixel(int16_t x, int16_t y, uint16_t color)
{
  if((x < 0) ||(x >= width) || (y < 0) || (y >= height)) return;

  SetAddrWindow(x,y,x+1,y+1);

  HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET); // Data

  SpiWrite(color >> 8);
  SpiWrite(color);
}

// *****************************************************************************
// ***   Draw vertical line   **************************************************
// *****************************************************************************
void ILI9341::DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  // Rudimentary clipping
  if((x >= width) || (y >= height)) return;

  if((y+h-1) >= height) h = height-y;

  SetAddrWindow(x, y, x, y+h-1);

  uint8_t hi = color >> 8, lo = color;

  HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET); // Data

  while (h--)
  {
    SpiWrite(hi);
    SpiWrite(lo);
  }
}

// *****************************************************************************
// ***   Draw horizontal line   ************************************************
// *****************************************************************************
void ILI9341::DrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  if((x >= width) || (y >= height)) return;
  if((x+w-1) >= width)  w = width-x;

  SetAddrWindow(x, y, x+w-1, y);

  uint8_t hi = color >> 8, lo = color;

  HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET); // Data

  while (w--)
  {
    SpiWrite(hi);
    SpiWrite(lo);
  }
}

// *****************************************************************************
// ***   Fill full screen   ****************************************************
// *****************************************************************************
void ILI9341::FillScreen(uint16_t color)
{
  FillRect(0, 0, width, height, color);
}

// *****************************************************************************
// ***   Fill rectangle on screen   ********************************************
// *****************************************************************************
void ILI9341::FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  if((x >= width) || (y >= height)) return;
  if((x + w - 1) >= width)  w = width  - x;
  if((y + h - 1) >= height) h = height - y;

  SetAddrWindow(x, y, x+w-1, y+h-1);

  uint8_t hi = color >> 8, lo = color;

  HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET); // Data

  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // Pull down CS
  for(y=h; y>0; y--)
  {
    for(x=w; x>0; x--)
    {
       // Wait until TXE flag is set to send data
       while(__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE) == RESET);
       hspi->Instance->DR = hi;
       while(__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE) == RESET);
       hspi->Instance->DR = lo;
    }
  }
  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET); // Pull up CS
}

// *****************************************************************************
// ***   Invert display   ******************************************************
// *****************************************************************************
void ILI9341::InvertDisplay(bool invert)
{
  WriteCommand(invert ? CMD_INVON : CMD_INVOFF);
}

// *****************************************************************************
// ***   Read data from SPI   **************************************************
// *****************************************************************************
inline uint8_t ILI9341::SpiRead(void)
{
  // Result variable
  uint8_t r = 0;
  // Pull down CS
  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
  // Receive data
  HAL_SPI_Receive(hspi, &r, sizeof(r), 100U);
  // Pull up CS
  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
  // Return result
  return r;
}

// *****************************************************************************
// ***   Read data from display   **********************************************
// *****************************************************************************
inline uint8_t ILI9341::ReadData(void)
{
  // Data
  HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET); // Data
  // Receive data
  uint8_t r = SpiRead();
  // Return result
  return r;
}

// *****************************************************************************
// ***   Send read command ad read result   ************************************
// *****************************************************************************
uint8_t ILI9341::ReadCommand(uint8_t c)
{
  // Set command mode
  HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET); // Command
  SpiWrite(c);

  // Set data mode
  HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET); // Data
  // Receive data
  uint8_t r = SpiRead();

  // Return result
  return r;
}
