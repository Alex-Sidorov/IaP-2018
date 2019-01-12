#include "library.h"


int main() {
	system("chcp 1251");
	bool hide_mode = false;
	while (true)
	{
		system("cls");
		get_info();
		print_menu();
		if (_kbhit())
		{
			switch (_getch())
			{
			case 'q':
			{
				ShowWindow(GetConsoleWindow(), SW_SHOW);
				hide_mode = false;
				break;
			}
			case 'w':
			{
				ShowWindow(GetConsoleWindow(), SW_HIDE);
				hide_mode = true;
				break;
			}
			case 'e':
			{
				if (!take_image(hide_mode))
				{
					return -1;
				}
				break;
			}
			case 'r':
			{
				if (!take_video(hide_mode))
				{
					return -1;
				}
				break;
			}
			case 't':
			{
				return 0;
			}
			}
		}
	}
}