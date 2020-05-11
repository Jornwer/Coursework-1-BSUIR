#include "Menus.h"

using namespace std;

void main()
{
	locale::global(locale(""));
	mainMenu();
	Catalog Catalog;
}

void drawMenu(vector<wstring> msg, int8_t& row)
{
	for (int i = 0; i < msg.size(); ++i)
	{
		wcout << msg[i];
		if (row == i) wcout << L"   <--";
	}
	wcout << L"\n\n" << flush;
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
	wcout << msg << L'\n';
	getline(cin, tmp);
	return tmp;
}

string getPassword(wstring msg)
{
	string password = "";
	char a = '1';

	system("cls");
	wcout << msg << L'\n';

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
	wcout << msg << L'\n';
	char a = _getch();
}

Credentials::Credentials(string login, string password)
{
	this->login = login;
	this->password = password;
}

bool operator==(const Deal& l, const Deal& r)
{
	return (l.car.brand == r.car.brand && l.car.color == r.car.color && l.date == r.date 
		&& l.car.model == r.car.model && l.car.price == r.car.price
		&& l.buyerName == r.buyerName && l.buyerSurname == r.buyerSurname && l.seller == r.seller);
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
	case 1: login = getString(L"Неправильный логин и/или пароль. Повторите ввод логина и пароля. Для выхода введите exit"); break;
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

		if (login[0] == ' ')
		{
			error = 1;
			login = getString(L"Логин не может начинаться с пробела . Повторите ввод логина. Для выхода введите exit");
		}

		if (login.size() < 4)
		{
			error = 1;
			if (login != "")
				login = getString(L"Длина логина должна быть больше 3 символов. Повторите ввод логина. Для выхода введите exit");
			else
				getline(cin, login);
			continue;
		}

		if (login.size() > 16)
		{
			error = 1;
			login = getString(L"Длина логина должна быть меньше 17 символов. Повторите ввод логина. Для выхода введите exit");
		}

		for (auto a : login)
		{
			if (!((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || (a >= '0' && a <= '9') || a == ' '))
			{
				error = 2;
				login = getString(L"Логин содержит недопустимые символы. Повторите ввод логина. Для выхода введите exit");
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
		
		if (password.size() < 8)
		{
			error = 2;
			if (password != "")
				password = getPassword(L"Пароль должен содержать минимум 8 символов. Повторите ввод пароля. Для выхода введите exit");
			else
				getline(cin, password);
			continue;
		}

		string symbols = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
		for (auto a : password)
		{
			if (!((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || (a >= '0' && a <= '9')))
			{
				error = 1;
				password = getPassword(L"Пароль содержит недопустимые символы. Повторите ввод пароля. Для выхода введите exit");
				break;
			}
			for (auto b : symbols)
				if (a == b)
				{
					error = 1;
					password = getPassword(L"Пароль содержит недопустимые символы. Повторите ввод пароля. Для выхода введите exit");
					break;
				}
		}
	} while (error);

	return password;
}

bool dealCorrect(Deal deal, string date)
{
	if (deal.car.brand.empty() || deal.car.color.empty() || deal.car.model.empty()
			|| deal.car.price.empty() || deal.buyerName.empty() || deal.buyerSurname.empty())
	{
		getCharacter(L"Одно из полей пустое. Для повторного ввода данных нажмите любую клавишу");
		return false;
	}

	if (date.size() != 8)
	{
		getCharacter(L"Дата должна состоять из 8 цифр. Для повторного ввода данных нажмите любую клавишу");
		return false;
	}

	if (deal.car.price.size() > 9)
	{
		getCharacter(L"Стоимость машины должна быть меньше 1000000000. Для продолжения нажмите любую клавишу");
		return false;
	}

	if (stringToInt(deal.car.price) == 0)
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

	time_t now = time(0);
	tm* ltm = localtime(&now);
	string currTime = (ltm->tm_mday < 10 ? "0" + to_string(ltm->tm_mday) : to_string(ltm->tm_mday))
		+ (ltm->tm_mon < 9 ? "0" + to_string(1 + ltm->tm_mon) : to_string(1 + ltm->tm_mon))
		+ to_string(1900 + ltm->tm_year);

	if (tmp > Date(currTime))
	{
		getCharacter(L"Введенная дата больше настоящей даты. Для продолжения нажмите любую клавишу");
		return false;
	}

	if (tmp < Date("01011990"))
	{
		getCharacter(L"Введенная дата дожна быть больше 01.01.1990. Для продолжения нажмите любую клавишу");
		return false;
	}
	
	return true;
}

bool dealCorrect(Deal deal)
{
	string date = deal.date.day + deal.date.month + deal.date.year;

	if (deal.car.brand.empty() && deal.car.color.empty() && deal.car.model.empty() && deal.car.price.empty()
		&& date.size() == 0 && deal.buyerName.empty() && deal.buyerSurname.empty() && deal.seller.empty()) return true;

	if (deal.date.day.size() != 2 || deal.date.month.size() != 2 || deal.date.year.size() != 4) return false;

	if (deal.car.brand.empty() || deal.car.color.empty() || deal.car.model.empty() || deal.car.price.empty()
		|| deal.buyerName.empty() || deal.buyerSurname.empty() || deal.seller.empty()) return false;

	if (stringToInt(deal.car.price) == 0 || stringToInt(deal.car.price) == -1) return false;

	if (deal.car.price.size() > 9) return false;

	for (auto str : vector<string>{ deal.car.color, deal.car.brand, deal.car.model, deal.buyerName, deal.buyerSurname, deal.seller })
		for (auto i : str)
			if (!((i >= 'a' && i <= 'z') || (i >= 'A' && i <= 'Z') || (i >= '0' && i <= '9')))
				return false;

	for (auto a : date)
		if (a < '0' || a > '9') return false;

	if (!dayCorrect(stringToInt(deal.date.day), stringToInt(deal.date.month), stringToInt(deal.date.year))) return false;

	time_t now = time(0);
	tm* ltm = localtime(&now);

	string currTime = (ltm->tm_mday < 10 ? "0" + to_string(ltm->tm_mday) : to_string(ltm->tm_mday)) 
					+ (ltm->tm_mon < 9 ? "0" + to_string(1 + ltm->tm_mon) : to_string(1 + ltm->tm_mon))
					+ to_string(1900 + ltm->tm_year);
	if (deal.date > Date(currTime)) return false;

	if (deal.date < Date("01011990")) return false;

	return true;
}

bool stringContainString(string& stringForSearch, string& searchingString)
{
	if (stringForSearch.size() < searchingString.size()) return false;
	for (int i = 0; i < searchingString.size(); ++i)
		if (stringForSearch[i] != searchingString[i])
			return false;
	return true;
}

bool comparePairs(pair<string, int> l, pair<string, int> r)
{
	return l.second > r.second;
}