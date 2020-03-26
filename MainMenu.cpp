#include "Menus.h"

using namespace std;

int8_t row = 0;
string arrow = "  <--";

void init()
{
	copyUserFile();
}

void drawMainMenu()
{
	system("cls");

	cout << "\n\n";
	cout << "   Войти как администратор";
	if (row == 0) cout << arrow;
	cout << "\n\n";

	cout << "   Войти как пользователь";
	if (row == 1) cout << arrow;
	cout << "\n\n";

	cout << "   Выход";
	if (row == 2) cout << arrow;
	cout << "\n";
}

void mainMenu()
{
	setlocale(0, "");

	while (true)
	{

		drawMainMenu();

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