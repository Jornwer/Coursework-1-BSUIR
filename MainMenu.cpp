#include "Menus.h"

using namespace std;

void init()
{
	copyUserFile();
	copyCatalogFile();
	copyAdminFile();
}

void drawMenu(vector<string> msg, int8_t& row)
{
	for (int i = 0; i < msg.size(); ++i)
	{
		cout << msg[i];
		if (row == i) cout << "   <--";
	}
	cout << "\n\n";
}

void mainMenu()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int8_t row = 0;

	while (true)
	{
		system("cls");
		drawMenu({"Войти как администратор","\n\nВойти как пользователь","\n\nВыход"}, row);

		char a = ' ';
		a = _getch();

		if (a == 72) row = (row + 2) % 3;
		else if (a == 80) row = (row + 1) % 3;
		else if (a == 13)
		{
			if (row == 0) ;
			else if (row == 1)userHaveAccount();
			else if (row == 2)
			{
				system("cls");
				break;
			}
		}
	}
}