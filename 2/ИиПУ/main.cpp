#include "library.h"

int main()
{
	HANDLE handle_hdd = get_handle_hdd();
	if (handle_hdd == INVALID_HANDLE_VALUE)
	{
		return ERROR_GET_HANDLE_HDD;
	}

	STORAGE_DEVICE_DESCRIPTOR *device_descriptor = nullptr;
	try
	{
		 device_descriptor = new STORAGE_DEVICE_DESCRIPTOR[KB];
	}
	catch (std::bad_alloc)
	{
		return ERROR_ALLOCATION;
	}

	device_descriptor->Size = KB;

	STORAGE_PROPERTY_QUERY storage_device;
	storage_device.QueryType = PropertyStandardQuery;
	storage_device.PropertyId = StorageDeviceProperty;
	
	if (!DeviceIoControl(handle_hdd, IOCTL_STORAGE_QUERY_PROPERTY, &storage_device, sizeof(storage_device), device_descriptor, KB, NULL, NULL))
	{
		delete device_descriptor;
		return ERROR_GET_INFO;
	}
	printf_info_of_hdd(device_descriptor);
	
	std::string mode;
	if (!get_mode(mode, handle_hdd, storage_device))
	{
		delete device_descriptor;
		return ERROR_GET_MODE;
	}
	else
	{
		printf_info_of_mode(mode);
	}

	read_info_of_memory();

	std::cout << std::endl;
	system("pause");
	delete device_descriptor;
	return 0;
}