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

Credentials::Credentials(string login, string password)
{
	this->login = login;
	this->password = password;
}

bool operator==(const Car& l, const Car& r)
{
	return (l.brand == r.brand && l.color == r.color && l.date == r.date && l.model == r.model && l.price == r.price);
}

bool operator==(const Date& l, const Date& r)
{
	return (l.day == r.day && l.year == r.year && l.month == r.month);
}

bool dayCorrect(int8_t day, int8_t month, int16_t year)
{
	if (day > 31 || day == 0 || month == 0 || month > 12 || year == 0) return false;
	if (day < 29) return true;
	if (month == 2)
	{
		if (day == 29)
		{
			if (year % 4) return false;
			if (!(year % 400)) return true;
			if (!(year % 100)) return false;
				
			return true;
		}
		return false;
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

bool operator==(const Credentials& l, const Credentials& r)
{
	return (l.login == r.login && l.password == r.password);
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

int Date::countDays() const
{
	return 372 * stringToInt(year) + 31 * stringToInt(month) + stringToInt(day);
}

void displayDate(string date)
{
	int8_t index = 0;
	wstring str = L"ДД.ММ.ГГГГ      ";
	for (int8_t i = 0; i < 16; ++i)
	{
		if (index + 1 > date.size() || index == 8) wcout << str[i];
		else
		{
			if (i == 2 || i == 5) cout << '.';
			else cout << date[index++];
		}
	}
}

bool operator<(const Date& l, const Date& r)
{
	return l.countDays() < r.countDays();
}

bool operator>(const Date& l, const Date& r)
{
	return l.countDays() > r.countDays();
}

Date::Date(string str)
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

int stringToInt(string str)
{
	int res = 0;
	if (str.size() < 10)
	{
		for (auto i : str)
			if (i >= '0' && i <= '9') res = 10 * res + i - '0';
			else return -1;
		return res;
	}
	return -1;
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
	case 1: login = getString(L"Неправильный логин и/или пароль. Повторите ввод логи и пароля. Для выхода введите exit"); break;
	case 2: login = getString(L"Аккаунт с таким логином уже существует. Повторите ввод логина и пароля. Для выхода введите exit"); break;
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

		string symbols = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
		for (auto a : password)
		{
			if (!((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || a == '_' || (a >= '0' && a <= '9')))
			{
				error = 1;
				password = getPassword(L"Пароль содержит недопустимые символы. Для выхода введите exit");
				break;
			}
			for (auto b : symbols)
				if (a == b)
				{
					error = 1;
					password = getPassword(L"Пароль содержит недопустимые символы. Для выхода введите exit");
					break;
				}
		}

		if (password.size() < 8)
		{
			error = 2;
			password = getPassword(L"Пароль должен содержать минимум 8 символов. Для выхода введите exit");
		}

	} while (error);

	return password;
}

bool carCorrect(Car car, string date)
{
	if (car.brand.empty() || car.color.empty() || car.model.empty() || car.price.empty() || date.size() != 8)
	{
		getCharacter(L"Данные введены неправильно. Для повторного ввода данных нажмите любую клавишу");
		return false;
	}

	if (car.price.size() > 9)
	{
		getCharacter(L"Стоимость машины должна быть меньше 1000000000. Для продолжения нажмите любую клавишу");
		return false;
	}

	if (stringToInt(car.price) == 0)
	{
		getCharacter(L"Стоимость машины должна быть больше 0. Для продолжения намите любую клавишу");
		return false;
	}

	Date tmp(date);
	if (!dayCorrect(stringToInt(tmp.day), stringToInt(tmp.month), stringToInt(tmp.year)))
	{
		getCharacter(L"Введенной даты не существует. Для продолжения нажмите любую клавишу");
		return false;
	}
	
	return true;
}

bool carCorrect(Car car)
{
	string date = "";
	date += car.date.day;
	date += car.date.month;
	date += car.date.year;

	if (car.brand.empty() && car.color.empty() && car.model.empty() && car.price.empty() && date.size() == 0) return true;

	if (car.brand.empty() || car.color.empty() || car.model.empty() || car.price.empty() || date.size() != 8) return false;

	if (stringToInt(car.price) == 0 || stringToInt(car.price) == -1) return false;

	for (auto a : date)
		if (a < '0' || a > '9') return false;

	if (!dayCorrect(stringToInt(car.date.day), stringToInt(car.date.month), stringToInt(car.date.year))) return false;

	return true;
}