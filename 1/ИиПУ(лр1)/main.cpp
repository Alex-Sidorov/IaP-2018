#include "library.h"

int main()
{
	setlocale(LC_ALL(NULL),"RUS");

	HDEVINFO dev_info = 0;
	SP_DEVINFO_DATA dev_info_data;
	if (!setup_class_devs(dev_info, dev_info_data))
	{
		error_setup_class_devs();
		return ERROR_SETUP_CLASS_DEVS;
	}

	for (DWORD index = 0; SetupDiEnumDeviceInfo(dev_info, index, &dev_info_data); index++)
	{
		TCHAR info[SIZE_DATA];
		TCHAR name_dev[SIZE_DATA];
		
		if (!SetupDiGetDeviceRegistryProperty(dev_info, &dev_info_data, SPDRP_DEVICEDESC, NULL, reinterpret_cast<PBYTE>(name_dev), SIZE_DATA, NULL))
		{
			error_get_devs_property();
			return ERROR_GET_DEVS_PROPERTY;
		}
 
		if (!SetupDiGetDeviceRegistryProperty(dev_info, &dev_info_data, SPDRP_HARDWAREID, NULL, reinterpret_cast<PBYTE>(info), SIZE_DATA, NULL))
		{
			error_get_devs_property();
			return ERROR_GET_DEVS_PROPERTY;
		}

		printf_info(name_dev,parse_info(info), index + 1);
	}


	if (!delete_class_devs(dev_info))
	{
		error_delete_class_devs();
		return ERROR_DELETE_CLASS_DEVS;
	}
	system("pause");
	return 0;
}