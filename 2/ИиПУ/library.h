#pragma once

#include <iostream>
#include <Windows.h>
#include <string>
#include <ntddscsi.h>

CONST INT KB = 1024;
CONST INT ERROR_GET_HANDLE_HDD = -1;
CONST INT ERROR_ALLOCATION = -2;
CONST INT ERROR_GET_INFO = -3;
CONST INT ERROR_GET_MODE = -4;

HANDLE get_handle_hdd();
void read_info_of_memory();

bool if_PIO(std::string &str, HANDLE &handle_hdd, STORAGE_PROPERTY_QUERY &storageProtertyQuery);
bool if_DMA(std::string &str, HANDLE &handle_hdd);
bool get_mode(std::string &str, HANDLE &handle_hdd, STORAGE_PROPERTY_QUERY &storageProtertyQuery);

void printf_info_of_hdd(STORAGE_DEVICE_DESCRIPTOR *device_descriptor);
void printf_info_of_space(char disk, _ULARGE_INTEGER &disk_space, _ULARGE_INTEGER &free_space);
void printf_info_of_mode(std::string &mode);