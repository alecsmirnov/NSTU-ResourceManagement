#ifndef WINAPISETTINGS_H
#define WINAPISETTINGS_H

#include <cstdint>

#include <Windows.h>

// Заголовок окна
constexpr LPCSTR WINDOW_TITLE = "Info";

// Положение и размер окна
constexpr int WINDOW_X		= 200;				// Горизонтальная позиция окна
constexpr int WINDOW_Y		= 200;				// Вертикальная позиция окна
constexpr int WINDOW_WIDTH  = 480;				// Ширина окна
constexpr int WINDOW_HEIGHT = 110;				// Высота окна

// Обработка сообщений
constexpr uint8_t NO_PROCESSING_WINDOW = 0;		// Отсутствия окна для обработки
constexpr uint8_t MSG_NUM_MAX = 0;				// Номер максимального сообщения по умолчанию
constexpr uint8_t MSG_NUM_MIN = 0;				// Номер минимального сообщения по умолчанию

// Название шрифта
constexpr char FONT_NAME[] = "UTF-8";

// Параметры шрифта
constexpr uint8_t FONT_HEIGHT	   = 18;		// Высота шрифта
constexpr uint8_t CHAR_WIDTH	   = 0;			// Средняя ширина символа
constexpr uint8_t FONT_ESCAPEMENT  = 0;			// Угол наклона шрифта
constexpr uint8_t FONT_ORIENTATION = 0;			// Ориентация (угол наклона) шрифта

// Параметры цвета шрифта (RGB)
constexpr uint8_t FONT_COLOR_R = 0;				// Красный цвет
constexpr uint8_t FONT_COLOR_G = 0;				// Зелёный цвет
constexpr uint8_t FONT_COLOR_B = 0;				// Синий цвет

// Положение текста окна
constexpr int LINE_WIDTH = 100;					// Ширина линий 
constexpr int LINE_1_X	 = 5;					// Горизонтальная позиция 1-й линии (Процессора)
constexpr int LINE_1_Y	 = 5;					// Вертикальная позиция 1-й линии
constexpr int LINE_2_X	 = 5;					// Горизонтальная позиция 2-й линии (L3 Кэша)
constexpr int LINE_2_Y	 = 25;					// Вертикальная позиция 2-й линии
constexpr int LINE_3_X	 = 5;					// Горизонтальная позиция 3-й линии (Времени)
constexpr int LINE_3_Y	 = 45;					// Вертикальная позиция 3-й линии

// Параметры потоковой функции
constexpr uint8_t DEFAULT_STACK_SIZE  = 0;		// Стандартный размер стека потока
constexpr void*   NO_THREAD_ARGUMENTS = NULL;	// Отсутствие входных параметров потока 
constexpr uint8_t IMMEDIATE_START	  = 0;		// Немедленный старт потока
constexpr uint8_t NO_THREAD_ID		  = 0;		// Отсутствие Id потока

#endif