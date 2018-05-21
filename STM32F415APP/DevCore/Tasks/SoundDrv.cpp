//******************************************************************************
//  @file SoundDrv.cpp
//  @author Nicolai Shlapunov
//
//  @details DevCore: Sound Driver Class, implementation
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

// *****************************************************************************
// ***   Includes   ************************************************************
// *****************************************************************************
#include "SoundDrv.h"

// *****************************************************************************
// ***   Get Instance   ********************************************************
// *****************************************************************************
SoundDrv& SoundDrv::GetInstance(void)
{
  // This class is static and declared here
  static SoundDrv sound_drv;
  // Return reference to class
  return sound_drv;
}

// *****************************************************************************
// ***   Init Display Driver Task   ********************************************
// *****************************************************************************
void SoundDrv::InitTask(TIM_HandleTypeDef* htm)
{
  // Save timer handle
  htim = htm;
  // Create task
  CreateTask("SoundDrv", SOUND_DRV_TASK_STACK_SIZE, SOUND_DRV_TASK_PRIORITY);
}

// *****************************************************************************
// ***   Sound Driver Setup   **************************************************
// *****************************************************************************
void SoundDrv::Setup(void* pvParameters)
{
  // Init time variable
  last_wake_time = xTaskGetTickCount();
}

// *****************************************************************************
// ***   Sound Driver Loop   ***************************************************
// *****************************************************************************
bool SoundDrv::Loop(void* pvParameters)
{
  // Flag
  bool is_playing = false;
  // Take mutex before start playing sound
  xSemaphoreTake(melody_mutex, portMAX_DELAY);
  // Delay for playing one frequency
  uint32_t current_delay_ms = delay_ms;
  // If no current melody or melody size is zero - skip playing
  if((sound_table != nullptr) && (sound_table_size != 0U))
  {
    // Set flag that still playing sound
    is_playing = true;
    // If frequency greater than 18 Hz
    if(((uint32_t)sound_table[sound_table_position] >> 4U) > 0x12U)
    {
      Tone(sound_table[sound_table_position] >> 4U);
    }
    else
    {
      // Otherwise "play" silence
      Tone(0U);
    }

    // Get retry counter from table and calculate delay
    current_delay_ms *= sound_table[sound_table_position] & 0x0FU;

    // Increase array index
    sound_table_position++;
    // If end of melody reached
    if(sound_table_position >= sound_table_size)
    {
      // If set repeat flag
      if(repeat == true)
      {
        // Reset index for play melody from beginning
        sound_table_position = 0U;
      }
      else
      {
        // Otherwise stop playing sound
        StopSound();
      }
    }
  }
  // Give mutex after start playing sound
  xSemaphoreGive(melody_mutex);

  // Pause until next tick
  vTaskDelayUntil(&last_wake_time, current_delay_ms);

  // Using semaphore here helps block this task while task wait request for
  // sound playing.
  if(is_playing == false)
  {
    // Wait semaphore for start play melody
    xSemaphoreTake(sound_update, portMAX_DELAY);
  }

  // Always run
  return true;
}

// *****************************************************************************
// ***   Beep function   *******************************************************
// *****************************************************************************
void SoundDrv::Beep(uint16_t freq, uint16_t del, bool pause_after_play)
{
  // Take mutex before beeping - prevent play melody during beeping.
  xSemaphoreTake(melody_mutex, portMAX_DELAY);
  // Start play tone
  Tone(freq);
  // Delay
  vTaskDelay(del);
  // Stop play tone
  Tone(0);
  // If flag is set
  if(pause_after_play == true)
  {
    // Delay with same value as played sound
    vTaskDelay(del);
  }
  // Give mutex after beeping
  xSemaphoreGive(melody_mutex);
}

// *****************************************************************************
// ***   Play sound function   *************************************************
// *****************************************************************************
void SoundDrv::PlaySound(const uint16_t* melody, uint16_t size, uint16_t temp_ms, bool rep)
{
  // Parameters: pointer to melody table, size of melody and repetition flag.
  // Format of sounds: 0x***#, where *** frequency, # - delay in temp_ms intervals

  // If pointer is not nullptr, if size & freq time greater than zero
  if((melody != nullptr) && (size > 0U) && (temp_ms > 0U))
  {
    // If already playing any melody
    if(IsSoundPlayed() == true)
    {
      // Stop it first
      StopSound();
    }

    // Take mutex before start playing melody
    xSemaphoreTake(melody_mutex, portMAX_DELAY);
    // Set repeat flag for melody
    repeat = rep;
    // Set time for one frequency
    delay_ms = temp_ms;
    // Set initial index for melody
    sound_table_position = 0;
    // Set melody size
    sound_table_size = size;
    // Set melody pointer
    sound_table = melody;
    // Give mutex after start playing melody
    xSemaphoreGive(melody_mutex);
    
    // Give semaphore for start play melody
    xSemaphoreGive(sound_update);
  }
}

// *****************************************************************************
// ***   Stop sound function   *************************************************
// *****************************************************************************
void SoundDrv::StopSound(void)
{
  // Take mutex before stop playing sound
  xSemaphoreTake(melody_mutex, portMAX_DELAY);
  // Clear sound table pointer
  sound_table = nullptr;
  // Clear sound table size
  sound_table_size = 0;
  // Clear sound table index
  sound_table_position = 0;
  // Set time for one frequency
  delay_ms = 100U;
  // Set repeat flag for melody
  repeat = false;
  // Stop sound
  Tone(0);
  // Give mutex after stop playing sound
  xSemaphoreGive(melody_mutex);
}

// *****************************************************************************
// ***   Mute sound function   *************************************************
// *****************************************************************************
void SoundDrv::Mute(bool mute_flag)
{
  // Set mute flag
  mute = mute_flag;
  // If mute flag is set - call Tone() for stop tone
  if(mute == true)
  {
    Tone(0U);
  }
}

// *****************************************************************************
// ***   Is sound played function   ********************************************
// *****************************************************************************
bool SoundDrv::IsSoundPlayed(void)
{
  // Return variable, false by default
  bool ret = false;
  // If sound_table is not nullptr - we still playing melody. No sense to use
  // mutex here - get pointer is atomic operation.
  if(sound_table != nullptr)
  {
    ret = true;
  }
  // Return result
  return ret;
}

// *****************************************************************************
// ***   Process Button Input function   ***************************************
// *****************************************************************************
void SoundDrv::Tone(uint16_t freq)
{
  // FIX ME: rewrite comment
  // “аймер запускаетс€ с параметрами:
  // Clock source: System Clock
  // Mode: CTC top = OCR2
  // OC2 output: Toggle on compare match
  // » с разными делител€ми дл€ разных частот, потому как:
  // при делителе 64 невозможно получить частоту ниже ~750√ц
  // при делителе 256 на частотах > ~1500√ц высока погрешность генерации
  // ƒеление на 4 аргументов дл€ того, что бы AVR_Clock_Freq/x не превысило word
  // ƒеление на два в конце, потому как нужен полупериод
  // ≈сли звук отключен - таймер останавливаетс€ и снимаетс€ напр€жение с пищалки
  if((freq > 11) && (mute == false))
  {
    // Calculate prescaler
    uint32_t prescaler = (HAL_RCC_GetHCLKFreq()/100U) / freq;
    // Set the Prescaler value
    htim->Instance->PSC = (uint32_t)prescaler;
    // Generate an update event to reload the Prescaler and the repetition
    // counter(only for TIM1 and TIM8) value immediately
    htim->Instance->EGR = TIM_EGR_UG;
    // Start timer in Output Compare match mode
    (void) HAL_TIM_OC_Start(htim, channel);
  }
  else
  {
    // Stop timer
    HAL_TIM_OC_Stop(htim, channel);
    // Clear Speaker output pin for decrease power consumer
    HAL_GPIO_WritePin(SPEAKER_GPIO_Port, SPEAKER_Pin, GPIO_PIN_RESET);
  }
}
