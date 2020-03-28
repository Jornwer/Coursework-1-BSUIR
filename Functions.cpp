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
		drawMenu({"����� ��� �������������","\n\n����� ��� ������������","\n\n�����"}, row);

		int8_t a = getCharCode();

		if (a == VK_UP) row = (row + 2) % 3;
		else if (a == VK_DOWN) row = (row + 1) % 3;
		else if (a == 13)
		{
			if (row == 0);
			else if (row == 1) userHaveAccount();
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
		tmp = getString("������ ����� ����������");
		for (auto i : tmp)
			if (!isalpha(i))
			{
				getCharacter("�������� ������ �������� �� ���������� ����. ��� ���������� ����� ������� ����� �������");
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
		tmp = getString("������ ������ ����������");
		for (auto i : tmp)
			if (!(isalnum(i) || i == ' '))
			{
				getCharacter("�������� ������ �������� �� ���������� ���� � ����. ��� ���������� ����� ������� ����� �������");
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
		tmp = getString("������� ���� ����������");
		for (auto i : tmp)
			if (!(i >= -64 && i < 0))
			{
				getCharacter("���� ������ �������� �� ������� ����. ��� ���������� ����� ������� ����� �������   " + to_string((int)i));
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
		tmp = getInt("������� ���� �������");
		if (dayCorrect(month, year, tmp)) return tmp;
		else getCharacter("���������� ��� �� ����������. ��� ���������� ����� ��� ������� ����� �������");
	} while (true);
}

int8_t Date::enterMonth()
{
	int8_t tmp;
	do
	{
		tmp = getInt("������� ����� �������");
		if (tmp > 0 && tmp < 13) return tmp;
		else getCharacter("����� ������ ���� �� 1 �� 12. ��� ���������� ����� ������� ����� �������");
	} while (true);
}

int16_t Date::enterYear()
{
	int16_t tmp;
	do
	{
		tmp = getInt("������� ��� �������");
		if (tmp >= 1000 && tmp <= 9999) return tmp;
		else getCharacter("��� ������ �������� �� 4 ����. ��� ���������� ����� ������� ����� �������");
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
			cout << "��������� ���� (��������� ����� �����):" << endl;
			cin.clear();
			while (cin.get() != '\n') {}
		}
	}
}

ostream& operator<<(ostream& out, const Date& date)
{
	out << date.day << '.' << date.month << '.' << date.year;
	return out;
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