#ifndef LIB_H
#define LIB_H

#ifdef LIB_EXPORTS
	#define DLL_SPEC extern "C" __declspec(dllexport)
#else
	#define DLL_SPEC extern "C" __declspec(dllimport)
#endif

// Получить текущее время системы в виде строки
DLL_SPEC char* getCurrentTime();
// Получить название процессора в виде строки
DLL_SPEC char* getProcName();
// Получить информацию о кэше 3-го уровня в виде строки
DLL_SPEC char* getL3Cache();

#endif