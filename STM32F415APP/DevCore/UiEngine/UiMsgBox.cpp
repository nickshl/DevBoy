//******************************************************************************
//  @file UiEngine.cpp
//  @author Nicolai Shlapunov
//
//  @details DevCore: UI Message Box Class, implementation
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
#include "UiEngine.h"

// *****************************************************************************
// ***   Public: MsgBox constructor   ******************************************
// *****************************************************************************
UiMsgBox::UiMsgBox(const char* msg_in, const char* hdr_in,
                   String::FontType msg_fnt_in, String::FontType hdr_fnt_in,
                   uint16_t center_x_in, uint16_t center_y_in,
                   uint16_t width_in, uint16_t color_in)
{
  // Save input params
  msg = msg_in;            // Message
  msg_fnt = msg_fnt_in;    // Message font
  hdr = hdr_in;            // Header string
  hdr_fnt = hdr_fnt_in;    // Header font
  center_x = center_x_in;  // X position of MsgBox center
  center_y = center_y_in;  // Y position of MsgBox center
  width = width_in;        // MsgBox width
  color = color_in;        // Color

  // Переменная для циклов
  uint8_t i;
  // Переменные для хранения размеров окна
  int16_t X, Y, W, H, StrW;
  // Вспомогательный указатель
  char* p;
  // Массив указателей на строки
  char* line[MAX_MSGBOX_LINES];
  // Массив длинн строк
  uint8_t length[MAX_MSGBOX_LINES];
  // Счетчик количества строк
  uint8_t count = 0;

  // Проверки
  if(msg == nullptr) return;                       // Если не передали основную строку - выходим
  if(msg_fnt == String::FONTS_MAX) msg_fnt = String::FONT_8x12;    // Если не передали шрифт - устанавливаем по умолчанию
  if(hdr_fnt == String::FONTS_MAX) hdr_fnt = String::FONT_4x6; // Если не передали шрифт - устанавливаем по умолчанию

  // Копируем строку в буфер, который будем резать на строки
  strcpy(str_buf, msg);
  
  // Устанавливаем первую строку на начало буфера
  line[count] = str_buf;

  // Обнуляем высоту сообщения
  H = 0;
  // Задаем ширину сообщения
  W = String::GetFontW(msg_fnt) * width;

  // В этом цикле буфер режем на строки
  while(count < MAX_MSGBOX_LINES)
  {
    // устанавливаем указатель p на место в строке содержащее первое включение символа '\n'
    p = strchr(line[count], (int)'\n');

    // Если такой символ в строке не найден
    if(p == nullptr)
    {
      // устанавливаем размер строки с помощью strlen
      length[count] = strlen(line[count]);
    }
    else // Иначе
    {
      // Вычисляем длинну строки по разности указателей
      length[count] = p - line[count];
      // И меняем символ '\n' на '\0', таким образом заканчивая строку
      *p = '\0';
    }

    // Вычисляем ширину строки в пикселях
    StrW = String::GetFontW(msg_fnt) * length[count];
    // Если ширина текущей строки больше предыдущих - запоминаем её
    if(W < StrW) W = StrW;
    // Добавляем высоту строки
    H += String::GetFontH(msg_fnt);

    // Если символ '\n' не был найден - эта строка последняя - выходим из цикла
    if(p == nullptr) break;

    // Увеличиваем счетчик строк
    count++;
    // Устанавливаем указатель на начало следующей строки
    line[count] = p + 1;
  }

  if(hdr != nullptr)
  {
    // Вычисляем ширину заголовка в пикселях
    StrW = String::GetFontW(hdr_fnt) * strlen(hdr);
    // Если ширина заголовка больше ширины информационных строк - запоминаем её
    if(W < StrW) W = StrW;
  }

  // Добавляем к размерам окна пустое место справа и слева шириной в 1 символ
  W += String::GetFontW(msg_fnt) * 2;
  // Добавляем к размерам окна пустое место сверку и снизу высотой в 0.5 символа
  H += String::GetFontH(msg_fnt);

  // Вычисляем положение окна относительно переданного центра
  X = center_x - W / 2;
  // Вычисляем положение окна относительно переданного центра
  Y = center_y - H / 2;

  // Заголовок, если присутствует
  if(hdr != nullptr)
  {
    // Сдвигаем окно вниз на половину величины заголовка
    Y += String::GetFontH(hdr_fnt) / 2;
    // Очищаем рамку вокруг заголовка
    box[box_cnt++].SetParams(X - 1, Y - String::GetFontH(hdr_fnt) - 2, W + 1, String::GetFontH(hdr_fnt) + 3, COLOR_MAGENTA, false);
    // Очищаем место под заголовок
    box[box_cnt++].SetParams(X, Y - String::GetFontH(hdr_fnt) - 1, W, String::GetFontH(hdr_fnt) + 1, COLOR_MAGENTA, true);
    // Выводим заголовок
    string[str_cnt++].SetParams(hdr, X + 1, Y - String::GetFontH(hdr_fnt), COLOR_YELLOW, hdr_fnt);
  }

  // Очищаем место под основное сообщение. Делается запас по краям
  // в 1 пиксель, что бы на рамка смотрелась на любом фоне
  box[box_cnt++].SetParams(X - 1, Y - 1, W + 2, H + 2, COLOR_BLACK, false);
  // Ресуем рамку вокруг основного сообщения
  box[box_cnt++].SetParams(X, Y, W, H, COLOR_MAGENTA, false);

  // В этом цикле выводим получившиеся строки на экран
  for(i = 0; i < count + 1; i++)
  {
    // Если длинна строки равна 0 - пропускаем этап рисования
    if(length[i] != 0)
    {
      // Находим положение по оси X для вывода строки
      X = center_x - (String::GetFontW(msg_fnt) * length[i]) / 2;
      // Выводим строку
      string[str_cnt++].SetParams(line[i], X, Y + String::GetFontH(msg_fnt)/2 + i * String::GetFontH(msg_fnt), COLOR_YELLOW, msg_fnt);
    }
  }
}

// *************************************************************************
// ***   Public: Destructor   **********************************************
// *************************************************************************
UiMsgBox::~UiMsgBox()
{
  // Hide before destruct for memory clean up
  Hide();
}

// *****************************************************************************
// ***   Show MsgBox   *********************************************************
// *****************************************************************************
void UiMsgBox::Show(uint32_t z)
{
  for(uint32_t i = 0; i < box_cnt; i++)
  {
    box[i].Show(z);
  }
  for(uint32_t i = 0; i < str_cnt; i++)
  {
    string[i].Show(z + 1U);
  }
}

// *****************************************************************************
// ***   Hide MsgBox   *********************************************************
// *****************************************************************************
void UiMsgBox::Hide(void)
{
  // Delete boxes
  for(uint32_t i = 0; i < box_cnt; i++)
  {
    box[i].Hide();
  }
  // Delete strings
  for(uint32_t i = 0; i < str_cnt; i++)
  {
    string[i].Hide();
  }
}

// *****************************************************************************
// ***   Show and Hide Msg box after pause   ***********************************
// *****************************************************************************
void UiMsgBox::Run(uint32_t delay)
{
  Show();
  DisplayDrv::GetInstance().UpdateDisplay();
  vTaskDelay(delay);
  Hide();
}
