//******************************************************************************
//  @file Image.cpp
//  @author Nicolai Shlapunov
//
//  @details DevCore: Image Visual Object Class, implementation
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
#include "Image.h"

// *****************************************************************************
// *****************************************************************************
// ***   Image   ***************************************************************
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// ***   Constructor   *********************************************************
// *****************************************************************************
Image::Image(int32_t x, int32_t y, const ImageDesc& img_dsc) : img_description(img_dsc)
{
  x_start = x;
  y_start = y;
  width = img_dsc.width;
  height = img_dsc.height;
  x_end = x + width - 1;
  y_end = y + height - 1;
  bits_per_pixel = img_dsc.bits_per_pixel;
  img = img_dsc.img;
  palette = img_dsc.palette;
  transparent_color = img_dsc.transparent_color;
  hor_mirror = false;
}

// *****************************************************************************
// ***   Put line in buffer   **************************************************
// *****************************************************************************
void Image::DrawInBufW(uint16_t* buf, int32_t n, int32_t line, int32_t start_x)
{
  // Draw only if needed
  if((line >= y_start) && (line <= y_end))
  {
    // Find idx in the image buffer
    uint32_t idx = (line - y_start) * width;
    // Find start x position
    int32_t start = x_start - start_x;
    // Prevent write in memory before buffer
    if(start < 0)
    {
      // Minus minus - plus
      idx -= start;
      start = 0;
    }
    // Find start x position
    int32_t end = x_end - start_x;
    // Prevent buffer overflow
    if(end >= n) end = n - 1;
    // Delta for cycle increment/decrement
    int32_t delta = 1;
    // Flip horizontally if needed
    if(hor_mirror)
    {
      idx += end - start;
      // Set delta to minus one for decrement cycle
      delta = -1;
    }
    // Draw image
    if(bits_per_pixel == 16)
    {
      // Get pointer to 16-bit image data
      uint16_t* p_img = (uint16_t*)img;
      // Pixels data copy cycle
      for(int32_t i = start; i <= end; i++)
      {
        // Get pixel data
        uint16_t data = p_img[idx];
        // Change index
        idx += delta;
        // If not transparent - output to buffer
        if(data != transparent_color) buf[i] = data;
      }
    }
    else
    {
      // Get pointer to 8-bit image data
      uint8_t* p_img = (uint8_t*)img;
      // Pixels data copy cycle
      for(int32_t i = start; i <= end; i++)
      {
        // Get pixel data
        uint16_t data = palette[p_img[idx]];
        // Change index
        idx += delta;
        // If not transparent - output to buffer
        if(data != transparent_color) buf[i] = data;
      }
    }
  }
}

// *****************************************************************************
// ***   Put line in buffer   **************************************************
// *****************************************************************************
void Image::DrawInBufH(uint16_t* buf, int32_t n, int32_t row, int32_t start_y)
{
  // Draw only if needed
  if((row >= x_start) && (row <= x_end))
  {
    // Find start x position
    int32_t start = y_start - start_y;
    // Prevent write in memory before buffer
    if(start < 0) start = 0;
    // Find start x position
    int32_t end = y_end - start_y;
    // Prevent buffer overflow
    if(end >= n) end = n - 1;
    // Have sense draw only if end pointer in buffer
    if(end > 0)
    {
      // Not implemented yet
    }
  }
}

// *****************************************************************************
// ***   Set Image function   **************************************************
// *****************************************************************************
void Image::SetImage(const ImageDesc& img_dsc, bool semaphore_taken)
{
  if(semaphore_taken == false) LockVisObject();
  width = img_dsc.width;
  height = img_dsc.height;
  x_end = x_start + width - 1;
  y_end = y_start + height - 1;
  bits_per_pixel = img_dsc.bits_per_pixel;
  img = img_dsc.img;
  palette = img_dsc.palette;
  transparent_color = img_dsc.transparent_color;
  if(semaphore_taken == false) UnlockVisObject();
}

// *****************************************************************************
// *****************************************************************************
// ***   Image8   **************************************************************
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// ***   Constructor   *********************************************************
// *****************************************************************************
Image8::Image8(int32_t x, int32_t y, int32_t w, int32_t h, const uint8_t* p_img, const uint16_t* p_palette)
{
  x_start = x;
  y_start = y;
  x_end = x + w - 1;
  y_end = y + h - 1;
  width = w;
  height = h;
  img = p_img;
  palette = p_palette;
}

// *****************************************************************************
// ***   Put line in buffer   **************************************************
// *****************************************************************************
void Image8::DrawInBufW(uint16_t* buf, int32_t n, int32_t line, int32_t start_x)
{
  // Draw only if needed
  if((line >= y_start) && (line <= y_end))
  {
    // Find start x position
    int32_t start = x_start - start_x;
    // Prevent write in memory before buffer
    if(start < 0) start = 0;
    // Find start x position
    int32_t end = x_end - start_x;
    // Prevent buffer overflow
    if(end >= n) end = n - 1;
    // Have sense draw only if end pointer in buffer
    if(x_end > 0)
    {
      int idx = (line - y_start) * width;
      for(int32_t i = start; i <= end; i++)
      {
        buf[i] = palette[img[idx++]];
      }
    }
  }
}

// *****************************************************************************
// ***   Put line in buffer   **************************************************
// *****************************************************************************
void Image8::DrawInBufH(uint16_t* buf, int32_t n, int32_t row, int32_t start_y)
{
  // Draw only if needed
  if((row >= x_start) && (row <= x_end))
  {
    // Find start x position
    int32_t start = y_start - start_y;
    // Prevent write in memory before buffer
    if(start < 0) start = 0;
    // Find start x position
    int32_t end = y_end - start_y;
    // Prevent buffer overflow
    if(end >= n) end = n - 1;
    // Have sense draw only if end pointer in buffer
    if(end > 0)
    {
      // Not implemented yet
    }
  }
}

// *****************************************************************************
// *****************************************************************************
// ***   Image16   *************************************************************
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// ***   Constructor   *********************************************************
// *****************************************************************************
Image16::Image16(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t* p_img)
{
  x_start = x;
  y_start = y;
  x_end = x + w - 1;
  y_end = y + h - 1;
  width = w;
  height = h;
  img = p_img;
}

// *****************************************************************************
// ***   Put line in buffer   **************************************************
// *****************************************************************************
void Image16::DrawInBufW(uint16_t* buf, int32_t n, int32_t line, int32_t start_x)
{
  // Draw only if needed
  if((line >= y_start) && (line <= y_end))
  {
    // Find start x position
    int32_t start = x_start - start_x;
    // Prevent write in memory before buffer
    if(start < 0) start = 0;
    // Find start x position
    int32_t end = x_end - start_x;
    // Prevent buffer overflow
    if(end >= n) end = n - 1;
    // Have sense draw only if end pointer in buffer
    if(x_end > 0)
    {
      int idx = (line - y_start) * width;
      for(int32_t i = start; i <= end; i++)
      {
        buf[i] = img[idx++];
      }
    }
  }
}

// *****************************************************************************
// ***   Put line in buffer   **************************************************
// *****************************************************************************
void Image16::DrawInBufH(uint16_t* buf, int32_t n, int32_t row, int32_t start_y)
{
  // Draw only if needed
  if((row >= x_start) && (row <= x_end))
  {
    // Find start x position
    int32_t start = y_start - start_y;
    // Prevent write in memory before buffer
    if(start < 0) start = 0;
    // Find start x position
    int32_t end = y_end - start_y;
    // Prevent buffer overflow
    if(end >= n) end = n - 1;
    // Have sense draw only if end pointer in buffer
    if(end > 0)
    {
      // Not implemented yet
    }
  }
}

// *****************************************************************************
// *****************************************************************************
// ***   Palettes   ************************************************************
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// ***   Palette with 8R-8G-4B levels (bits 3-3-2)   ***************************
// *****************************************************************************
const uint16_t PALETTE_884[256] = {
0x0000, 0x0020, 0x0040, 0x0068, 0x0088, 0x00B0, 0x00D0, 0x00F8, 0x0001, 0x0021, 0x0041, 0x0069, 0x0089, 0x00B1, 0x00D1, 0x00F9, 
0x2002, 0x2022, 0x2042, 0x206A, 0x208A, 0x20B2, 0x20D2, 0x20FA, 0x4003, 0x4023, 0x4043, 0x406B, 0x408B, 0x40B3, 0x40D3, 0x40FB, 
0x6004, 0x6024, 0x6044, 0x606C, 0x608C, 0x60B4, 0x60D4, 0x60FC, 0x8005, 0x8025, 0x8045, 0x806D, 0x808D, 0x80B5, 0x80D5, 0x80FD, 
0xA006, 0xA026, 0xA046, 0xA06E, 0xA08E, 0xA0B6, 0xA0D6, 0xA0FE, 0xE007, 0xE027, 0xE047, 0xE06F, 0xE08F, 0xE0B7, 0xE0D7, 0xE0FF, 
0x0A00, 0x0A20, 0x0A40, 0x0A68, 0x0A88, 0x0AB0, 0x0AD0, 0x0AF8, 0x0A01, 0x0A21, 0x0A41, 0x0A69, 0x0A89, 0x0AB1, 0x0AD1, 0x0AF9, 
0x2A02, 0x2A22, 0x2A42, 0x2A6A, 0x2A8A, 0x2AB2, 0x2AD2, 0x2AFA, 0x4A03, 0x4A23, 0x4A43, 0x4A6B, 0x4A8B, 0x4AB3, 0x4AD3, 0x4AFB, 
0x6A04, 0x6A24, 0x6A44, 0x6A6C, 0x6A8C, 0x6AB4, 0x6AD4, 0x6AFC, 0x8A05, 0x8A25, 0x8A45, 0x8A6D, 0x8A8D, 0x8AB5, 0x8AD5, 0x8AFD, 
0xAA06, 0xAA26, 0xAA46, 0xAA6E, 0xAA8E, 0xAAB6, 0xAAD6, 0xAAFE, 0xEA07, 0xEA27, 0xEA47, 0xEA6F, 0xEA8F, 0xEAB7, 0xEAD7, 0xEAFF, 
0x1400, 0x1420, 0x1440, 0x1468, 0x1488, 0x14B0, 0x14D0, 0x14F8, 0x1401, 0x1421, 0x1441, 0x1469, 0x1489, 0x14B1, 0x14D1, 0x14F9, 
0x3402, 0x3422, 0x3442, 0x346A, 0x348A, 0x34B2, 0x34D2, 0x34FA, 0x5403, 0x5423, 0x5443, 0x546B, 0x548B, 0x54B3, 0x54D3, 0x54FB, 
0x7404, 0x7424, 0x7444, 0x746C, 0x748C, 0x74B4, 0x74D4, 0x74FC, 0x9405, 0x9425, 0x9445, 0x946D, 0x948D, 0x94B5, 0x94D5, 0x94FD, 
0xB406, 0xB426, 0xB446, 0xB46E, 0xB48E, 0xB4B6, 0xB4D6, 0xB4FE, 0xF407, 0xF427, 0xF447, 0xF46F, 0xF48F, 0xF4B7, 0xF4D7, 0xF4FF, 
0x1F00, 0x1F20, 0x1F40, 0x1F68, 0x1F88, 0x1FB0, 0x1FD0, 0x1FF8, 0x1F01, 0x1F21, 0x1F41, 0x1F69, 0x1F89, 0x1FB1, 0x1FD1, 0x1FF9, 
0x3F02, 0x3F22, 0x3F42, 0x3F6A, 0x3F8A, 0x3FB2, 0x3FD2, 0x3FFA, 0x5F03, 0x5F23, 0x5F43, 0x5F6B, 0x5F8B, 0x5FB3, 0x5FD3, 0x5FFB, 
0x7F04, 0x7F24, 0x7F44, 0x7F6C, 0x7F8C, 0x7FB4, 0x7FD4, 0x7FFC, 0x9F05, 0x9F25, 0x9F45, 0x9F6D, 0x9F8D, 0x9FB5, 0x9FD5, 0x9FFD, 
0xBF06, 0xBF26, 0xBF46, 0xBF6E, 0xBF8E, 0xBFB6, 0xBFD6, 0xBFFE, 0xFF07, 0xFF27, 0xFF47, 0xFF6F, 0xFF8F, 0xFFB7, 0xFFD7, 0xFFFF};

// *****************************************************************************
// ***   Palette with 7R-7G-5B levels (245 colors)   ***************************
// *****************************************************************************
const uint16_t PALETTE_775[256] = {
0x0000, 0x0028, 0x0050, 0x0078, 0x00A0, 0x00C8, 0x00F8, 0x4001, 0x4029, 0x4051, 0x4079, 0x40A1, 0x40C9, 0x40F9, 0xA002, 0xA02A, 
0xA052, 0xA07A, 0xA0A2, 0xA0CA, 0xA0FA, 0xE003, 0xE02B, 0xE053, 0xE07B, 0xE0A3, 0xE0CB, 0xE0FB, 0x4005, 0x402D, 0x4055, 0x407D, 
0x40A5, 0x40CD, 0x40FD, 0x8006, 0x802E, 0x8056, 0x807E, 0x80A6, 0x80CE, 0x80FE, 0xE007, 0xE02F, 0xE057, 0xE07F, 0xE0A7, 0xE0CF, 
0xE0FF, 0x0700, 0x0728, 0x0750, 0x0778, 0x07A0, 0x07C8, 0x07F8, 0x4701, 0x4729, 0x4751, 0x4779, 0x47A1, 0x47C9, 0x47F9, 0xA702, 
0xA72A, 0xA752, 0xA77A, 0xA7A2, 0xA7CA, 0xA7FA, 0xE703, 0xE72B, 0xE753, 0xE77B, 0xE7A3, 0xE7CB, 0xE7FB, 0x4705, 0x472D, 0x4755, 
0x477D, 0x47A5, 0x47CD, 0x47FD, 0x8706, 0x872E, 0x8756, 0x877E, 0x87A6, 0x87CE, 0x87FE, 0xE707, 0xE72F, 0xE757, 0xE77F, 0xE7A7, 
0xE7CF, 0xE7FF, 0x0F00, 0x0F28, 0x0F50, 0x0F78, 0x0FA0, 0x0FC8, 0x0FF8, 0x4F01, 0x4F29, 0x4F51, 0x4F79, 0x4FA1, 0x4FC9, 0x4FF9, 
0xAF02, 0xAF2A, 0xAF52, 0xAF7A, 0xAFA2, 0xAFCA, 0xAFFA, 0xEF03, 0xEF2B, 0xEF53, 0xEF7B, 0xEFA3, 0xEFCB, 0xEFFB, 0x4F05, 0x4F2D, 
0x4F55, 0x4F7D, 0x4FA5, 0x4FCD, 0x4FFD, 0x8F06, 0x8F2E, 0x8F56, 0x8F7E, 0x8FA6, 0x8FCE, 0x8FFE, 0xEF07, 0xEF2F, 0xEF57, 0xEF7F, 
0xEFA7, 0xEFCF, 0xEFFF, 0x1700, 0x1728, 0x1750, 0x1778, 0x17A0, 0x17C8, 0x17F8, 0x5701, 0x5729, 0x5751, 0x5779, 0x57A1, 0x57C9, 
0x57F9, 0xB702, 0xB72A, 0xB752, 0xB77A, 0xB7A2, 0xB7CA, 0xB7FA, 0xF703, 0xF72B, 0xF753, 0xF77B, 0xF7A3, 0xF7CB, 0xF7FB, 0x5705, 
0x572D, 0x5755, 0x577D, 0x57A5, 0x57CD, 0x57FD, 0x9706, 0x972E, 0x9756, 0x977E, 0x97A6, 0x97CE, 0x97FE, 0xF707, 0xF72F, 0xF757, 
0xF77F, 0xF7A7, 0xF7CF, 0xF7FF, 0x1F00, 0x1F28, 0x1F50, 0x1F78, 0x1FA0, 0x1FC8, 0x1FF8, 0x5F01, 0x5F29, 0x5F51, 0x5F79, 0x5FA1, 
0x5FC9, 0x5FF9, 0xBF02, 0xBF2A, 0xBF52, 0xBF7A, 0xBFA2, 0xBFCA, 0xBFFA, 0xFF03, 0xFF2B, 0xFF53, 0xFF7B, 0xFFA3, 0xFFCB, 0xFFFB, 
0x5F05, 0x5F2D, 0x5F55, 0x5F7D, 0x5FA5, 0x5FCD, 0x5FFD, 0x9F06, 0x9F2E, 0x9F56, 0x9F7E, 0x9FA6, 0x9FCE, 0x9FFE, 0xFF07, 0xFF2F, 
0xFF57, 0xFF7F, 0xFFA7, 0xFFCF, 0xFFFF, 0x0000, 0x0028, 0x0050, 0x0078, 0x00A0, 0x00C8, 0x00F8, 0x4001, 0x4029, 0x4051, 0x4079};

// *****************************************************************************
// ***   Palette with 6R-7G-6B levels (252 colors)   ***************************
// *****************************************************************************
const uint16_t PALETTE_676[256] = {
0x0000, 0x0030, 0x0060, 0x0090, 0x00C0, 0x00F8, 0x4001, 0x4031, 0x4061, 0x4091, 0x40C1, 0x40F9, 0xA002, 0xA032, 0xA062, 0xA092, 
0xA0C2, 0xA0FA, 0xE003, 0xE033, 0xE063, 0xE093, 0xE0C3, 0xE0FB, 0x4005, 0x4035, 0x4065, 0x4095, 0x40C5, 0x40FD, 0x8006, 0x8036, 
0x8066, 0x8096, 0x80C6, 0x80FE, 0xE007, 0xE037, 0xE067, 0xE097, 0xE0C7, 0xE0FF, 0x0600, 0x0630, 0x0660, 0x0690, 0x06C0, 0x06F8, 
0x4601, 0x4631, 0x4661, 0x4691, 0x46C1, 0x46F9, 0xA602, 0xA632, 0xA662, 0xA692, 0xA6C2, 0xA6FA, 0xE603, 0xE633, 0xE663, 0xE693, 
0xE6C3, 0xE6FB, 0x4605, 0x4635, 0x4665, 0x4695, 0x46C5, 0x46FD, 0x8606, 0x8636, 0x8666, 0x8696, 0x86C6, 0x86FE, 0xE607, 0xE637, 
0xE667, 0xE697, 0xE6C7, 0xE6FF, 0x0C00, 0x0C30, 0x0C60, 0x0C90, 0x0CC0, 0x0CF8, 0x4C01, 0x4C31, 0x4C61, 0x4C91, 0x4CC1, 0x4CF9, 
0xAC02, 0xAC32, 0xAC62, 0xAC92, 0xACC2, 0xACFA, 0xEC03, 0xEC33, 0xEC63, 0xEC93, 0xECC3, 0xECFB, 0x4C05, 0x4C35, 0x4C65, 0x4C95, 
0x4CC5, 0x4CFD, 0x8C06, 0x8C36, 0x8C66, 0x8C96, 0x8CC6, 0x8CFE, 0xEC07, 0xEC37, 0xEC67, 0xEC97, 0xECC7, 0xECFF, 0x1200, 0x1230, 
0x1260, 0x1290, 0x12C0, 0x12F8, 0x5201, 0x5231, 0x5261, 0x5291, 0x52C1, 0x52F9, 0xB202, 0xB232, 0xB262, 0xB292, 0xB2C2, 0xB2FA, 
0xF203, 0xF233, 0xF263, 0xF293, 0xF2C3, 0xF2FB, 0x5205, 0x5235, 0x5265, 0x5295, 0x52C5, 0x52FD, 0x9206, 0x9236, 0x9266, 0x9296, 
0x92C6, 0x92FE, 0xF207, 0xF237, 0xF267, 0xF297, 0xF2C7, 0xF2FF, 0x1800, 0x1830, 0x1860, 0x1890, 0x18C0, 0x18F8, 0x5801, 0x5831, 
0x5861, 0x5891, 0x58C1, 0x58F9, 0xB802, 0xB832, 0xB862, 0xB892, 0xB8C2, 0xB8FA, 0xF803, 0xF833, 0xF863, 0xF893, 0xF8C3, 0xF8FB, 
0x5805, 0x5835, 0x5865, 0x5895, 0x58C5, 0x58FD, 0x9806, 0x9836, 0x9866, 0x9896, 0x98C6, 0x98FE, 0xF807, 0xF837, 0xF867, 0xF897, 
0xF8C7, 0xF8FF, 0x1F00, 0x1F30, 0x1F60, 0x1F90, 0x1FC0, 0x1FF8, 0x5F01, 0x5F31, 0x5F61, 0x5F91, 0x5FC1, 0x5FF9, 0xBF02, 0xBF32, 
0xBF62, 0xBF92, 0xBFC2, 0xBFFA, 0xFF03, 0xFF33, 0xFF63, 0xFF93, 0xFFC3, 0xFFFB, 0x5F05, 0x5F35, 0x5F65, 0x5F95, 0x5FC5, 0x5FFD, 
0x9F06, 0x9F36, 0x9F66, 0x9F96, 0x9FC6, 0x9FFE, 0xFF07, 0xFF37, 0xFF67, 0xFF97, 0xFFC7, 0xFFFF, 0x0000, 0x0030, 0x0060, 0x0090};
