#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <strsafe.h>

void Die(LPTSTR lpszFunction) 
{ 
	// Retrieve the system error message for the last-error code
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dwError = GetLastError(); 

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );

	// Display the error message and exit the process
	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)); 
	StringCchPrintf((LPTSTR)lpDisplayBuf, 
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error 0x%x: %s"), 
		lpszFunction, dwError, lpMsgBuf); 
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Media Key Simulator"), MB_ICONSTOP); 

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dwError); 
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	HOOKPROC hkprcKeyboard;
	static HINSTANCE hinstDll;
	static HHOOK hhookKeyboard;

	hinstDll = LoadLibrary("MediaKeyHook.dll");
	if (!hinstDll) Die("LoadLibrary");

	hkprcKeyboard = (HOOKPROC)GetProcAddress(hinstDll, "KeyboardProc");
	if (!hkprcKeyboard) Die("GetProcAddress");

	hhookKeyboard = SetWindowsHookEx(WH_KEYBOARD, hkprcKeyboard, hinstDll, 0);
	if (!hhookKeyboard) Die("SetWindowsHookEx");
	
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(hhookKeyboard);
	return EXIT_SUCCESS;
}