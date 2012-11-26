#include "hook.h"

BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpvReserved) {
	return true;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode < 0) 	return CallNextHookEx(0, nCode, wParam, lParam);

	if ((lParam & (1 << 31)) && (lParam & (1 << 30))) { //up now and down before
		bool bCtrl = GetKeyState(VK_CONTROL) < 0;
		bool bAlt = GetKeyState(VK_MENU) < 0;
		if (bCtrl && bAlt) {
			switch (wParam) {
			case VK_HOME:
				SendKeyboardInput(VK_MEDIA_PLAY_PAUSE);
				return -1;
			case VK_END:
				SendKeyboardInput(VK_MEDIA_STOP);
				return -1;
			case VK_PRIOR:
				SendKeyboardInput(VK_MEDIA_PREV_TRACK);
				return -1;
			case VK_NEXT:
				SendKeyboardInput(VK_MEDIA_NEXT_TRACK);
				return -1;
			case VK_UP:
				SendKeyboardInput(VK_VOLUME_UP);
				return -1;
			case VK_DOWN:
				SendKeyboardInput(VK_VOLUME_DOWN);
				return -1;
			}
		}
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}

UINT CALLBACK SendKeyboardInput(WORD wVk) {
	KEYBDINPUT ki;
	ki.wVk = wVk;
	ki.wScan = 0;
	ki.dwFlags = 0;
	ki.time = 0;
	ki.dwExtraInfo = (ULONG_PTR)GetMessageExtraInfo();

	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki = ki;

	return SendInput(1, &input, sizeof(INPUT));
}