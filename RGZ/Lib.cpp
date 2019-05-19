#include "Lib.h"

#include <cstring>

#include "WinLocalTime.h"
#include "ProcInfo.h"

// Двузначное обозначение формата времени
constexpr uint8_t TWO_DIGIT = 10;

// Формирование динамической C-строки по STL строке 
static char* getCStringFromSTDString(std::string str) {
	char* cstr = NULL;

	if (!str.empty()) {
		cstr = new char[str.length() + 1];
		strcpy(cstr, str.c_str());
	}

	return cstr;
}

// Получить текущее время системы в виде строки
DLL_SPEC char* getCurrentTime() {
	std::string cur_time_str;

	// Формирование представления текущего времени
	auto cur_time = WinLocalTime::getCurrentTime();
	cur_time_str = std::to_string(cur_time.hour) + ":";
	if (cur_time.minute < TWO_DIGIT) cur_time_str += "0";
	cur_time_str += std::to_string(cur_time.minute);

	return getCStringFromSTDString(cur_time_str);
}

// Получить название процессора в виде строки
DLL_SPEC char* getProcName() {
	return getCStringFromSTDString(ProcInfo::getProcName());
}

// Получить информацию о кэше 3-го уровня в виде строки
DLL_SPEC char* getL3Cache() {
	std::string cache_str;

	// Проверка существования кэша 3-го уровня
	if (auto cache = ProcInfo::getL3Cache(); !cache.empty())
		cache_str = std::to_string(cache.getSize())		+ " Bytes, line size: " +
					std::to_string(cache.getLineSize()) + " Bytes";
	else
		cache_str = "not found!";

	return getCStringFromSTDString(cache_str);
}