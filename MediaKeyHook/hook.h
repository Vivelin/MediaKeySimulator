#include <Windows.h>

LRESULT CALLBACK KeyboardProc(
	__in	int nCode,
	__in	WPARAM wParam,
	__in	LPARAM lParam
);

UINT CALLBACK SendKeyboardInput(WORD wVk);