#include "Menus.h"

using namespace std;

void main()
{
	mainMenu();
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

string getString(string msg)
{
	system("cls");
	string tmp;
	cout << msg << endl;
	getline(cin, tmp);
	return tmp;
}

string getPassword(string msg)
{
	string password = "";
	char a = '1';

	system("cls");
	cout << msg << endl;

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

void getCharacter(string msg)
{
	system("cls");
	cout << msg << endl;
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

Credentials::Credentials() {}

void Car::getBrand()
{
	string tmp;
	do
	{
		bool isOk = true;
		tmp = getString("Введте марку автомобиля");
		for (auto i : tmp)
			if (!isalpha(i))
			{
				getCharacter("Название должно состоять из английских букв. Для повторного ввода нажмите любую клавишу");
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
	string tmp;
	do
	{
		bool isOk = true;
		tmp = getString("Введте модель автомобиля");
		for (auto i : tmp)
			if (!(isalnum(i) || i == ' '))
			{
				getCharacter("Название должно состоять из английских букв и цифр. Для повторного ввода нажмите любую клавишу");
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
	string tmp;
	do
	{
		bool isOk = true;
		tmp = getString("Введите цвет автомобиля");
		for (auto i : tmp)
			if (!(i >= -64 && i < 0))
			{
				getCharacter("Цвет должен состоять из русских букв. Для повторного ввода нажмите любую клавишу   " + to_string((int)i));
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
	year = to_string(iYear);
	month = to_string(iMonth);
	day = to_string(enterDay(iMonth, iYear));
}

int8_t Date::enterDay(int8_t month, int16_t year)
{
	int8_t tmp;
	do
	{
		tmp = getInt("Введите день продажи");
		if (dayCorrect(month, year, tmp)) return tmp;
		else getCharacter("Введенного дня не существует. Для повторного ввода дня нажмите любую клавишу");
	} while (true);
}

int8_t Date::enterMonth()
{
	int8_t tmp;
	do
	{
		tmp = getInt("Введите месяц продажи");
		if (tmp > 0 && tmp < 13) return tmp;
		else getCharacter("Месяц должен быть от 1 до 12. Для повторного ввода нажмите любую клавишу");
	} while (true);
}

int16_t Date::enterYear()
{
	int16_t tmp;
	do
	{
		tmp = getInt("Введите год продажи");
		if (tmp >= 1000 && tmp <= 9999) return tmp;
		else getCharacter("Год должен состоять из 4 цифр. Для повторного ввода нажмите любую клавишу");
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

int getInt(string msg)
{
	int a;
	system("cls");
	cout << msg << endl;
	while (true) {
		cin >> a;
		if (cin.peek() == '\n') {
			cin.get();
			return a;
		}
		else {
			system("cls");
			cout << "Повторите ввод (ожидается целое число):" << endl;
			cin.clear();
			while (cin.get() != '\n') {}
		}
	}
}
