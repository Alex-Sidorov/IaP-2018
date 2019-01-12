#include "library.h"

void print_menu()
{
	cout << "q.Открыть окно" << endl << "w.Скрыть окно" << endl << "r.Запись видео" << endl << "e.Запись изображения" << endl << "t.Выход" << endl;
}
bool get_requeste(wstring &info, IPropertyBag *property_bag, LPCOLESTR str)
{
	VARIANT var;
	VariantInit(&var);

	if (!FAILED(property_bag->Read(str, &var, 0)))
	{
		info = var.bstrVal;
	}
	else
	{
		VariantClear(&var);
		return false;
	}
	VariantClear(&var);
	return true;
}

bool get_info()
{
	if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
	{
		return false;
	}

	ICreateDevEnum *dev_enum;
	if (FAILED(CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dev_enum))))
	{
		return false;
	}

	IEnumMoniker *moniker_enum;
	dev_enum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &moniker_enum, 0);
	dev_enum->Release();

	IMoniker *moniker = nullptr;
	moniker_enum->Next(1, &moniker, NULL);
	if (!moniker)
	{
		return false;
	}

	IPropertyBag *property_bag = nullptr;
	if (FAILED(moniker->BindToStorage(0, 0, IID_PPV_ARGS(&property_bag))))
	{
		return false;
	}
	moniker->Release();

	wstring temp;
	if (!get_requeste(temp, property_bag, L"FriendlyName"))
	{
		return false;
	}

	wcout << "Имя устройства: " << temp << endl;
	if (!get_requeste(temp, property_bag, L"DevicePath"))
	{
		return false;
	}
	wcout << "DeviceID: " << temp.substr(temp.find(L"vid_") + 4, 4) << endl;
	wcout << "VendorID: " << temp.substr(temp.find(L"pid_") + 4, 4) << endl;
	return true;
}

void input_str(string &str)
{
	do
	{
		try
		{
			std::getline(cin, str);
		}
		catch (std::bad_alloc)
		{
			str.clear();
		}

	} while (str.empty());
}

bool take_image(bool hide_mode)
{
	TCHAR name[SIZE_TEMP_STR];
	TCHAR version[SIZE_TEMP_STR];
	auto index = -1;

	for (auto i = LEFT_RANGE_CAPTURE_DRIVER; i <= RIGHT_RANGE_CAPTURE_DRIVER; i++)
	{
		if (capGetDriverDescription(i, name, SIZE_TEMP_STR, version, SIZE_TEMP_STR))
		{
			index = i;
		}
	}
	if (index == -1)
	{
		return false;
	}

	auto window = capCreateCaptureWindow(LPTSTR("My window"), WS_DISABLED, 0, 0, 0, 0, 0, 0);
	if (!capDriverConnect(window, index))
	{
		return false;
	}

	string name_file;
	if (hide_mode)
	{
		name_file = "1.png";
	}
	else
	{
		cout << "Введите название файла для сохранения изображения." << endl;
		input_str(name_file);
		name_file += ".png";
		cout << "Нажмите любую кнопку для получения изображения." << endl;
		_getch();
	}
	if (capFileSaveDIB(window, name_file.c_str()))
	{
		cout << "Изображение " << name_file << " сохранено." << endl;
	}
	else
	{
		return false;
	}
	capCaptureAbort(window);
	capDriverDisconnect(window);
	return true;
}

bool create_file_video(string &name_file)
{
	cout << "Введите название файла для сохранения видео." << endl;
	input_str(name_file);
	name_file += ".avi";

	if (CreateFileA(name_file.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL) == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	return true;
}

int input_int_value()
{
	int value = 0;
	do
	{
		rewind(stdin);
		cin >> value;

		if (std::cin.fail() || value <= 0)
		{
			cin.clear();
			continue;
		}
		else
		{
			return value;
		}

	} while (1);
}

bool take_video(bool hide_mode)
{
	if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
	{
		return false;
	}

	ICaptureGraphBuilder2 *builder = nullptr;
	IGraphBuilder *graph = nullptr;
	if (FAILED(CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, reinterpret_cast<void**>(&builder))))
	{
		return false;
	}

	if (FAILED(CoCreateInstance(CLSID_FilterGraph, 0, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, reinterpret_cast<void**>(&graph))))
	{
		return false;
	}
	builder->SetFiltergraph(graph);

	ICreateDevEnum *dev_enum = nullptr;
	if (FAILED(CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dev_enum))))
	{
		return false;
	}

	IEnumMoniker *enum_moniker;
	dev_enum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &enum_moniker, 0);
	dev_enum->Release();

	IMoniker *moniker = nullptr;
	IBaseFilter *filter = nullptr;

	enum_moniker->Next(1, &moniker, NULL);
	moniker->BindToObject(0, 0, IID_IBaseFilter, reinterpret_cast<void**>(&filter));
	graph->AddFilter(filter, L"Capture Filter");

	string name_file;
	if (hide_mode)
	{
		name_file = "1.avi";
	}
	else if (!create_file_video(name_file))
	{
		return false;
	}
	IBaseFilter* mux = nullptr;
	wstring w_name_file(name_file.begin(), name_file.end());
	builder->SetOutputFileName(&MEDIASUBTYPE_Avi, w_name_file.c_str(), &mux, NULL);
	builder->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, filter, NULL, mux);
	mux->Release();
	IMediaControl* control = nullptr;
	graph->QueryInterface(IID_IMediaControl, reinterpret_cast<void**>(&control));

	int lenght = 0;
	if (hide_mode)
	{
		lenght = 5;
	}
	else
	{
		cout << "Продолжительность видео: ";
		lenght = input_int_value();
	}

	control->Run();
	Sleep(lenght * ONE_SECOND);
	control->Stop();
	control->Release();
	return true;
}