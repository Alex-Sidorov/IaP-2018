#include "library.h"

char* interface_types[] =
{
	"UNKNOWN",
	"SCSI",
	"ATAPI",
	"ATA",
	"1394",
	"SSA",
	"FIBRE",
	"USB",
	"RAID",
	"ISCSI",
	"SAS",
	"SATA",
	"SD",
	"MMC",
	"VIRTUAL",
	"FILE BACKED VIRTUAL",
	"SPACES",
	"NVME",
	"UNKNOWN" ,
	"UNKNOWN"
};

void printf_info_of_space(char disk, _ULARGE_INTEGER &disk_space, _ULARGE_INTEGER &free_space)
{
	std::cout << std::endl << "Disk " << disk << " ALL:" << disk_space.QuadPart / (KB*KB) << " GB | FREE:" << free_space.QuadPart / (KB*KB) <<
		" GB | USED:" << disk_space.QuadPart / (KB*KB) - free_space.QuadPart / (KB*KB) << " GB";
}

void printf_info_of_mode(std::string &mode)
{
	std::cout << std::endl << "MODE:" << mode ? mode : "NOTHING";
	std::cout << std::endl;
}

void read_info_of_memory()
{
	int count_logical_drives = GetLogicalDrives();

	TCHAR temp[5];
	_ULARGE_INTEGER disk_space;
	_ULARGE_INTEGER free_space;

	disk_space.QuadPart = 0;
	free_space.QuadPart = 0;

	for (char value = 'A'; value < 'Z'; value++)
	{
		if ((count_logical_drives >> value - 'A') & 0x00000001 && value != 'F')
		{
			wsprintf(temp, L"%c:\\\\", value);
			GetDiskFreeSpaceEx(temp, 0, &disk_space, &free_space);
			printf_info_of_space(value, disk_space, free_space);
		}

	}

}

HANDLE get_handle_hdd()
{
	return CreateFile(L"\\\\.\\PHYSICALDRIVE0", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, NULL);
}

void printf_info_of_hdd(STORAGE_DEVICE_DESCRIPTOR *device_descriptor)
{
	std::string temp = reinterpret_cast<char*>(device_descriptor) + device_descriptor->ProductIdOffset;
	
	temp[temp.find(' ')] = '\0';
	std::cout << "VENDER:" << temp.c_str() << std::endl;
	std::cout << "MODEL:" << temp.substr(temp.find('\0')) << std::endl;
	std::cout << "FIRMWARE:" << reinterpret_cast<char*>(device_descriptor) + device_descriptor->ProductRevisionOffset << std::endl;
	temp = reinterpret_cast<char*>(device_descriptor) + device_descriptor->SerialNumberOffset;
	std::cout << "SERIAL NUMBER:" << temp.substr(temp.find_first_not_of(' ')) << std::endl;
	std::cout << "INTERFACES TYPE:" << interface_types[device_descriptor->BusType] << std::endl;
}

bool if_PIO(std::string &str, HANDLE &handle_hdd, STORAGE_PROPERTY_QUERY &storage_device)
{
	STORAGE_ADAPTER_DESCRIPTOR adapter_descriptor;
	if (!DeviceIoControl(handle_hdd, IOCTL_STORAGE_QUERY_PROPERTY, &storage_device, sizeof(storage_device), &adapter_descriptor, sizeof(STORAGE_ADAPTER_DESCRIPTOR), NULL, NULL))
	{
		return false;
	}
	if (adapter_descriptor.AdapterUsesPio)
	{
		str += "PIO";
	}
	return true;
}

bool if_DMA(std::string &str, HANDLE &handle_hdd)
{
	TCHAR buffer[512 + sizeof(ATA_PASS_THROUGH_EX)] = { 0 };

	ATA_PASS_THROUGH_EX *ATA = reinterpret_cast<ATA_PASS_THROUGH_EX*>(buffer);
	ATA->Length = sizeof(ATA_PASS_THROUGH_EX);
	ATA->TimeOutValue = 1;

	if (!DeviceIoControl(handle_hdd, IOCTL_ATA_PASS_THROUGH, ATA, sizeof(buffer), ATA, sizeof(buffer), NULL, NULL)) {

		return false;
	}

	if (ATA->AtaFlags == ATA_FLAGS_USE_DMA)
	{
		str += " DMA";
	}
	return true;
}

bool get_mode(std::string &str, HANDLE &handle_hdd, STORAGE_PROPERTY_QUERY &storageProtertyQuery)
{
	if (!if_PIO(str, handle_hdd, storageProtertyQuery) || !if_DMA(str, handle_hdd))
	{
		return false;
	}
	else
	{
		return true;
	}
}