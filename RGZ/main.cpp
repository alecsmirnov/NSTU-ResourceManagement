#include <string>

#include "WinAPISettings.h"

// Предоставляемая программой информация 
namespace Information {
	static char* cur_time  = NULL;		// Текущее время
	static char* proc_name = NULL;		// Имя процессора
	static char* cache 	   = NULL;		// Кэш L3
}

// Потоковая функция
static DWORD WINAPI InfoThread(LPVOID) {
	// Подключение библиотеки
	if (HINSTANCE lib_inst = LoadLibrary(TEXT("Lib.dll")); lib_inst) {
		// Объявление псевдонима указателя на DLL-функцию
		using DLLFunction = char* (*)();

		// Загрузка функций DLL-библиотеки
		auto getProcName = (DLLFunction)GetProcAddress(lib_inst, "getProcName");
		auto getL3Cache  = (DLLFunction)GetProcAddress(lib_inst, "getL3Cache");
		auto getCurTime  = (DLLFunction)GetProcAddress(lib_inst, "getCurrentTime");

		Information::proc_name = getProcName();
		Information::cache 	   = getL3Cache();
		Information::cur_time  = getCurTime();

		FreeLibrary(lib_inst);
	}

	ExitThread(EXIT_SUCCESS);
}

// Функция обработки сообщений для окна
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
	switch (msg) {
		case WM_CREATE: {
			// Создание потока с вызовом потоковой функции
			HANDLE info_thread = CreateThread(NULL, DEFAULT_STACK_SIZE, InfoThread, 
											  NO_THREAD_ARGUMENTS, IMMEDIATE_START, NO_THREAD_ID);
			// Ожидание завершения потока
			WaitForSingleObject(info_thread, INFINITE);

			CloseHandle(info_thread);
			break;
		}
		case WM_PAINT: {
			// Подготовка окна для рисования
			PAINTSTRUCT paint_struct;
			HDC hdc = BeginPaint(hwnd, &paint_struct);

			// Создание шрифта для отоюражения информации на окне
			HFONT hfont = CreateFont(FONT_HEIGHT, CHAR_WIDTH, FONT_ESCAPEMENT, FONT_ORIENTATION, 
									 FW_THIN, TRUE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
									 CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT(FONT_NAME));
			// Применение шрифта
			SelectObject(hdc, hfont);
			// Установка цвета шрифта
			SetTextColor(hdc, RGB(FONT_COLOR_R, FONT_COLOR_G, FONT_COLOR_R));

			// Проверка полученной информации из DLL функций
			if (Information::proc_name && Information::cache && Information::cur_time) {
				// Положение первой строки на окне (Процессора)
				TextOut(hdc, LINE_1_X, LINE_1_Y, "Processor:", strlen("Processor:"));
				TextOut(hdc, LINE_1_X + LINE_WIDTH, LINE_1_Y, Information::proc_name, 
													   strlen(Information::proc_name));

				// Положение второй строки на окне (L3 Кэша)
				TextOut(hdc, LINE_2_X, LINE_2_Y, "L3 Cache:", strlen("L3 Cache:"));
				TextOut(hdc, LINE_2_X + LINE_WIDTH, LINE_2_Y, Information::cache, 
													   strlen(Information::cache));

				// Положение третей строки на окне (Текущего времени)
				TextOut(hdc, LINE_3_X, LINE_3_Y, "Current Time:", strlen("Current Time:"));
				TextOut(hdc, LINE_3_X + LINE_WIDTH, LINE_3_Y, Information::cur_time, 
													   strlen(Information::cur_time));
			}
			else
				// Проверка поддержки CPUID инструкций и вывод сообщения на экран
				if (!Information::proc_name && Information::cache)
					TextOut(hdc, LINE_1_X, LINE_1_Y, "Error: CPUID not supported by processor!", 
											  strlen("Error: CPUID not supported by processor!"));
				else 
					// Вывод на экран сообщения об отсутствии DLL файла
					TextOut(hdc, LINE_1_X, LINE_1_Y, "Error: DLL file not found!", 
											  strlen("Error: DLL file not found!"));

			delete Information::proc_name;
			delete Information::cache;
			delete Information::cur_time;

			EndPaint(hwnd, &paint_struct);
			break;
		}
		case WM_DESTROY: {
			// Указание система на запрос зевершения работы окна
			PostQuitMessage(EXIT_SUCCESS);
			break;
		}
	}

	return DefWindowProc(hwnd, msg, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE h_inst, HINSTANCE h_prev_inst, LPSTR cmd_line, int cmd_show) {
	// Создание и инициализация класса окна
	WNDCLASS wcl;
	LPCSTR class_name = "InfoWindow";

	wcl.hInstance	  = h_inst;
	wcl.lpszClassName = class_name;
	wcl.lpfnWndProc	  = WindowProc;
	wcl.style		  = CS_HREDRAW | CS_VREDRAW;
	wcl.hIcon		  = LoadIcon(NULL, IDI_APPLICATION);
	wcl.hCursor		  = LoadCursor(NULL, IDC_ARROW);
	wcl.lpszMenuName  = NULL;
	wcl.cbClsExtra	  = 0;
	wcl.cbWndExtra	  = 0;
	wcl.hbrBackground = static_cast<HBRUSH>(GetStockObject(DEFAULT_PALETTE));
	RegisterClass(&wcl);
	
	// Создание окна
	HWND hwnd = CreateWindow(class_name, WINDOW_TITLE, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
							 WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, HWND_DESKTOP, NULL, h_inst, NULL);

	// Отображение и обновление окна
	ShowWindow(hwnd, cmd_show);
	UpdateWindow(hwnd);

	// Получение и обработка сообщения
	MSG msg;
	while (GetMessage(&msg, NO_PROCESSING_WINDOW, MSG_NUM_MAX, MSG_NUM_MIN)) {	
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}