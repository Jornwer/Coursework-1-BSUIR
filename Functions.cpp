#include "Menus.h"

using namespace std;

void main()
{
	locale::global(locale(""));
	mainMenu();
}

void drawMenu(vector<wstring> msg, int8_t& row)
{
	for (int i = 0; i < msg.size(); ++i)
	{
		wcout << msg[i];
		if (row == i) wcout << L"   <--";
	}
	wcout << L"\n\n";;
}

void mainMenu()
{
	int8_t row = 0;

	while (true)
	{
		system("cls");
		drawMenu({L"Войти как администратор",L"\n\nВойти как пользователь",L"\n\nВыход"}, row);

		int8_t a = getCharCode();

		if (a == VK_UP) row = (row + 2) % 3;
		else if (a == VK_DOWN) row = (row + 1) % 3;
		else if (a == 13)
		{
			if (row == 0) adminHaveAccount();
			else if (row == 1) userHaveAccount();
			else if (row == 2)
			{
				system("cls");
				break;
			}
		}
	}
}

string getString(wstring msg)
{
	system("cls");
	string tmp;
	wcout << msg << endl;
	getline(cin, tmp);
	return tmp;
}

string getPassword(wstring msg)
{
	string password = "";
	char a = '1';

	system("cls");
	wcout << msg << endl;

	while ((a = _getch()) != 13)
	{
		if ((int)a == 8)
		{
			if (password.empty()) continue;
			
			password.erase(password.size() - 1, password.size());

			cout << '\b' << ' ' << '\b';
		}
		else
		{
			password += a;
			cout << '*';
		}
	}
	return password;
}

void getCharacter(wstring msg)
{
	system("cls");
	wcout << msg << endl;
	char a = _getch();
}

bool Credentials::operator==(Credentials a)
{
	return (this->login == a.login && this->password == a.password) ? true : false;
}

Credentials::Credentials(string login, string password)
{
	this->login = login;
	this->password = password;
}

void Car::getBrand()
{
	wstring tmp;
	do
	{
		bool isOk = true;
		tmp = getWstring(L"Введите марку автомобиля");
		for (auto i : tmp)
			if (!isalpha(i))
			{
				getCharacter(L"Название должно состоять из английских букв. Для повторного ввода нажмите любую клавишу");
				isOk = false;
				break;
			}
		if (isOk && tmp.size())
		{
			brand = tmp;
			return;
		}
	} while (true);
}

void Car::getModel()
{
	wstring tmp;
	do
	{
		bool isOk = true;
		tmp = getWstring(L"Введите модель автомобиля");
		for (auto i : tmp)
			if (!(isalnum(i) || i == ' '))
			{
				getCharacter(L"Название должно состоять из английских букв и цифр. Для повторного ввода нажмите любую клавишу");
				isOk = false;
				break;
			}
		if (isOk && tmp.size())
		{
			model = tmp;
			return;
		}
	} while (true);
}

void Car::getColor()
{
	wstring tmp;
	do
	{
		bool isOk = true;
		tmp = getWstring(L"Введите цвет автомобиля");
		for (auto i : tmp)
			if (!(i >= -64 && i < 0))
			{
				getCharacter(L"Цвет должен состоять из русских букв. Для повторного ввода нажмите любую клавишу   " + to_wstring((int)i));
				isOk = false;
				break;
			}
		if (isOk && tmp.size())
		{
			color = tmp;
			return;
		}
	} while (true);
}

bool Car::operator==(Car a)
{
	if (a.brand == brand && a.color == color && a.date == date && a.model == model && a.price == price) return true;
	else return false;
}

bool Date::operator==(Date a)
{
	if (a.day == day && a.year == year && a.month == month) return true;
	else return false;
}

void Date::getDate()
{
	int iYear = enterYear(), iMonth = enterMonth();
	year = to_wstring(iYear);
	month = to_wstring(iMonth);
	day = to_wstring(enterDay(iMonth, iYear));
}

int8_t Date::enterDay(int8_t month, int16_t year)
{
	int8_t tmp;
	do
	{
		tmp = getInt(L"Введите день продажи");
		if (dayCorrect(month, year, tmp)) return tmp;
		else getCharacter(L"Введенного дня не существует. Для повторного ввода дня нажмите любую клавишу");
	} while (true);
}

int8_t Date::enterMonth()
{
	int8_t tmp;
	do
	{
		tmp = getInt(L"Введите месяц продажи");
		if (tmp > 0 && tmp < 13) return tmp;
		else getCharacter(L"Месяц должен быть от 1 до 12. Для повторного ввода нажмите любую клавишу");
	} while (true);
}

int16_t Date::enterYear()
{
	int16_t tmp;
	do
	{
		tmp = getInt(L"Введите год продажи");
		if (tmp >= 1000 && tmp <= 9999) return tmp;
		else getCharacter(L"Год должен состоять из 4 цифр. Для повторного ввода нажмите любую клавишу");
	} while (true);
}

bool Date::dayCorrect(int8_t month, int16_t year, int8_t day)
{
	if (day > 31 || !day) return false;
	if (day < 29) return true;
	if (month == 2)
	{
		if (year % 4) return false;
		if (!year % 100)
			if (year % 400) return false;
		return true;
	}
	if (month <= 7)
	{
		if (month % 2) return true;
		else if (day != 31) return true;
		else return false;
	}
	else
	{
		if (!month % 2) return true;
		else if (day != 31) return true;
		else return false;
	}
}

int getInt(wstring msg)
{
	int a;
	system("cls");
	wcout << msg << endl;
	while (true) {
		cin >> a;
		if (cin.peek() == '\n') {
			cin.get();
			return a;
		}
		else {
			system("cls");
			wcout << "Повторите ввод (ожидается целое число):" << endl;
			cin.clear();
			while (cin.get() != '\n') {}
		}
	}
}

int8_t getCharCode()
{
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD rec;
	DWORD numRead;
	while (ReadConsoleInput(hIn, &rec, 1, &numRead) && numRead == 1) {
		if (rec.EventType == KEY_EVENT && rec.Event.KeyEvent.bKeyDown) {
			return rec.Event.KeyEvent.wVirtualKeyCode;
		}
	}
}

int Date::countDays()
{
	return 372 * wstringToInt(year) + 31 * wstringToInt(month) + wstringToInt(day);
}

void displayDate(wstring date)
{
	int8_t index = 0;
	wstring str = L"ДД.ММ.ГГГГ      ";
	for (int8_t i = 0; i < 16; ++i)
	{
		if (index + 1 > date.size() || index == 8) wcout << str[i];
		else
		{
			if (i == 2 || i == 5) wcout << L'.';
			else wcout << date[index++];
		}
	}
}

bool Date::operator<(Date date)
{
	return (countDays() < date.countDays() ? true : false);
}

bool Date::operator>(Date date)
{
	return (countDays() > date.countDays() ? true : false);
}

Date::Date(wstring str)
{
	if (str.size() == 8)
	{
		day += str[0];
		day += str[1];
		month += str[2];
		month += str[3];
		year += str[4];
		year += str[5];
		year += str[6];
		year += str[7];
	}
}

Date::Date()
{
}

int wstringToInt(wstring str)
{
	int res = 0;
	if (str.size() < 10)
	{
		for (auto i : str)
			res = 10 * res + i - '0';
		return res;
	}
	return -1;
}

wstring getWstring(wstring msg)
{
	system("cls");
	wstring tmp;
	wcout << msg << endl;
	getline(wcin, tmp);
	return tmp;
}

string enterLogin(int8_t mode, bool& haveAccess, bool& leave)
{
	string login;
	int8_t error;
	switch (mode)
	{
	case 0: login = (haveAccess ? getString(L"Введите логин. Для выхода введите exit")
		: getString(L"Введите заново логин. Для выхода введите exit"));
		break;
	case 1: login = getString(L"Неправильный логин и/или пароль. Повторите ввод. Для выхода введите exit"); break;
	case 2: login = getString(L"Аккаунт с таким логином уже существует. Повторите ввод. Для выхода введите exit"); break;
	}

	do
	{
		error = 0;

		if (login == "exit")
		{
			leave = true;
			break;
		}

		if (login.size() < 4)
		{
			error = 1;
			login = getString(L"Введите логин из 4 или более символов. Для выхода введите exit");
		}
		if (login[0] == ' ') { error = 2; continue; }
		for (auto a : login)
		{
			if (!((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || (a >= '0' && a <= '9') || (a >= 'а' && a <= 'я') || (a >= 'А' && a <= 'Я')))
			{
				error = 2;
				login = getString(L"Логин содержит недопустимые символы. Для выхода введите exit");
				break;
			}
		}

	} while (error);

	return login;
}

string enterPassword(bool& leave)
{
	int8_t error;
	string password;

	password = getPassword(L"Введите пароль. Для выхода введите exit");

	do {
		error = 0;

		if (password == "exit")
		{
			leave = true;
			return "";
		}

		for (auto a : password)
			if (!((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || a == '_' || (a >= '0' && a <= '0')))
			{
				error = 1;
				password = getPassword(L"Пароль содержит недопустимые символы. Для выхода введите exit");
				break;
			}

		if (password.size() < 8)
		{
			error = 2;
			password = getPassword(L"Пароль должен содержать минимум 8 символов. Для выхода введите exit");
		}

	} while (error);

	return password;
}