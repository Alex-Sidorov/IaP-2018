#pragma once
#pragma comment (lib, "Setupapi.lib")

#include <stdio.h>
#include <windows.h>
#include <setupapi.h>
#include <regstr.h>
#include <string>

CONST DWORD SIZE_DATA = 300;
CONST INT SIZE_ID = 4;

CONST INT ERROR_SETUP_CLASS_DEVS = -1;
CONST INT ERROR_GET_DEVS_PROPERTY = -2;
CONST INT ERROR_DELETE_CLASS_DEVS = -3;

bool setup_class_devs(HDEVINFO &dev_info, SP_DEVINFO_DATA &dev_info_data);
bool delete_class_devs(HDEVINFO &dev_info);

void printf_info(TCHAR* name_dev, std::wstring &str, int index);
std::wstring parse_info(TCHAR* info);

void error_delete_class_devs();
void error_get_devs_property();
void error_setup_class_devs();
