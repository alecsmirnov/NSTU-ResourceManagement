#ifndef WINLOCALTIME_H
#define WINLOCALTIME_H

#include <Windows.h>

// Структура представления времени системы
struct Time {
	WORD hour;			// Часы
	WORD minute;		// Минуты
	WORD second;		// Секунды
};

// Статический класс для получения текущего времени
class WinLocalTime {
public:
	// Получить текущее время системы (часы, минуты, секунды)
	static Time getCurrentTime() {
		SYSTEMTIME time;
		GetLocalTime(&time);

		return { time.wHour, time.wMinute, time.wSecond };
	}
};

#endif