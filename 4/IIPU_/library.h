#pragma once

#include <iostream>
#include "conio.h"
#include <string>
#include <Windows.h>
#include <Vfw.h>
#include <dshow.h>

#pragma comment(lib, "vfw32")
#pragma comment(lib, "strmiids")

using std::cout;
using std::wcout;
using std::cin;
using std::endl;
using std::wstring;
using std::string;


CONST INT SIZE_TEMP_STR = 100;
CONST INT LEFT_RANGE_CAPTURE_DRIVER = 0;
CONST INT RIGHT_RANGE_CAPTURE_DRIVER = 9;
CONST INT ONE_SECOND = 1000;

CONST INT ERROR_MAIN = -1;

void print_menu();
void input_str(string &str);
int input_int_value();
bool create_file_video(string &name_file);
bool take_video(bool hide_mode);
bool take_image(bool hide_mode);
bool get_info();
bool get_requeste(wstring &info, IPropertyBag *property_bag, LPCOLESTR str);