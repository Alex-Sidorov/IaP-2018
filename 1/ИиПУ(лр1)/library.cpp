#include "library.h"

bool setup_class_devs(HDEVINFO &dev_info, SP_DEVINFO_DATA &dev_info_data)
{
	dev_info = SetupDiGetClassDevs(NULL, REGSTR_KEY_PCIENUM, NULL, DIGCF_ALLCLASSES);

	if (dev_info == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	dev_info_data.cbSize = sizeof(SP_DEVINFO_DATA);
	return true;
}

bool delete_class_devs(HDEVINFO &dev_info)
{
	if (SetupDiDestroyDeviceInfoList(dev_info))
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::wstring parse_info(TCHAR* info)//\\pci\\ven_4848&dev_
{
	std::wstring temp = info;
	std::wstring result = L"  VenderID:" + temp.substr(temp.find_first_of(L'_') + 1, SIZE_ID);

	temp = temp.substr(temp.find_first_of(L'&') + 1);
	result += L"\n";

	result += L"  DeviceID:" + temp.substr(temp.find_first_of(L'_') + 1, SIZE_ID);

	return result;
}

void printf_info(TCHAR *name_dev, std::wstring &str, int index)
{	
	wprintf(L"%d.Name:%s\n%s\n\n", index, name_dev, str.c_str());
}

void error_setup_class_devs()
{
	wprintf(L"%d", GetLastError);
}

void error_delete_class_devs()
{
	wprintf(L"%d", GetLastError);
}

void error_get_devs_property()
{
	wprintf(L"%d", GetLastError);
}